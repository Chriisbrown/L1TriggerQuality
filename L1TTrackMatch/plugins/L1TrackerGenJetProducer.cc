///////////////////////////////////////////////////////////////////////////
//                                                                       //
// Producer of TkJet,                                          //
// Cluster L1 tracks using fastjet                                       //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "L1Trigger/TrackTrigger/interface/StubPtConsistency.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"

#include "DataFormats/L1TCorrelator/interface/TkJet.h"
#include "DataFormats/L1TCorrelator/interface/TkJetFwd.h"
#include "DataFormats/L1TVertex/interface/Vertex.h"
#include "DataFormats/Math/interface/LorentzVector.h"

// L1 tracks
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"

// geometry
#include "Geometry/Records/interface/TrackerTopologyRcd.h"

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "RecoJets/JetProducers/plugins/VirtualJetProducer.h"
#include <string>
#include "TMath.h"
#include "TH1.h"

using namespace l1t;
using namespace edm;
using namespace std;


//////////////////////////////
//                          //
//     CLASS DEFINITION     //
//                          //
//////////////////////////////

class L1TrackerGenJetProducer : public edm::EDProducer
{
public:

  typedef std::vector< TrackingParticle > L1TTTrackCollectionType;

  explicit L1TrackerGenJetProducer(const edm::ParameterSet&);
  ~L1TrackerGenJetProducer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  //virtual void beginRun(edm::Run&, edm::EventSetup const&);
  //virtual void endRun(edm::Run&, edm::EventSetup const&);
  //virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  //virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  // member data

  // track selection criteria
  float TRK_ZMAX;       // [cm]
  float TRK_CHI2MAX;    // maximum track chi2
  float TRK_PTMIN;      // [GeV]
  float TRK_ETAMAX;     // [rad]
  unsigned int   TRK_NSTUBMIN;   // minimum number of stubs
  int   TRK_NSTUBPSMIN; // minimum number of stubs in PS modules
  int L1Tk_nPar;
  double DeltaZ0Cut;    // save with |L1z-z0| < maxZ0
  double CONESize;      // Use anti-kt with this cone size
  bool doPtComp;
  bool doTightChi2;
  double BendConsistency;

  std::string outputname;


  //need PVtx here
  const edm::EDGetTokenT< std::vector< TrackingParticle > > TrackingParticleToken_;
  const edm::EDGetTokenT< std::vector< TrackingVertex > > TrackingVertexToken_;
};

//////////////
// constructor
L1TrackerGenJetProducer::L1TrackerGenJetProducer(const edm::ParameterSet& iConfig) :
TrackingParticleToken_(consumes< std::vector< TrackingParticle > > (iConfig.getParameter<edm::InputTag>("L1TrackingTrackInputTag"))),
TrackingVertexToken_(consumes< std::vector< TrackingVertex > >(iConfig.getParameter<edm::InputTag>("L1TrackingVertexInputTag")))
{

  
  L1Tk_nPar   =(int)iConfig.getParameter<int>("L1Tk_nPar");
  TRK_ZMAX    = (float)iConfig.getParameter<double>("TRK_ZMAX");
  TRK_CHI2MAX = (float)iConfig.getParameter<double>("TRK_CHI2MAX");
  TRK_PTMIN   = (float)iConfig.getParameter<double>("TRK_PTMIN");
  TRK_ETAMAX  = (float)iConfig.getParameter<double>("TRK_ETAMAX");
  TRK_NSTUBMIN   = (unsigned int)iConfig.getParameter<int>("TRK_NSTUBMIN");
  TRK_NSTUBPSMIN = (int)iConfig.getParameter<int>("TRK_NSTUBPSMIN");
  DeltaZ0Cut =(float)iConfig.getParameter<double>("DeltaZ0Cut");
  CONESize =(float)iConfig.getParameter<double>("CONESize");
  doPtComp     = iConfig.getParameter<bool>("doPtComp");
  doTightChi2 = iConfig.getParameter<bool>("doTightChi2");
  BendConsistency=iConfig.getParameter<double>("BendConsistency");

  outputname = iConfig.getParameter<std::string>("L1TrkJetTag");
  produces<TkJetCollection>(outputname);
}

/////////////
// destructor
L1TrackerGenJetProducer::~L1TrackerGenJetProducer() {
}

///////////
// producer
void L1TrackerGenJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // ----------------------------------------------------------------------------------------------
  // output container
  // ----------------------------------------------------------------------------------------------

  std::unique_ptr<TkJetCollection> L1TrackerGenJets(new TkJetCollection);


  // ----------------------------------------------------------------------------------------------
  // retrieve input containers
  // ----------------------------------------------------------------------------------------------
  // L1 tracks
  edm::Handle< std::vector< TrackingParticle > > TrackingParticleHandle;
  iEvent.getByToken(TrackingParticleToken_, TrackingParticleHandle);
  L1TTTrackCollectionType::const_iterator iterL1Track;

  // Tracker Topology
  edm::ESHandle<TrackerTopology> tTopoHandle_;
  iSetup.get<TrackerTopologyRcd>().get(tTopoHandle_);
  const TrackerTopology* tTopo = tTopoHandle_.product();
  ESHandle<TrackerGeometry> tGeomHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get(tGeomHandle);

   
  edm::ESHandle<MagneticField> magneticFieldHandle;
  iSetup.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);  
  const MagneticField* theMagneticField = magneticFieldHandle.product();
  double mMagneticFieldStrength = theMagneticField->inTesla(GlobalPoint(0,0,0)).z();
  const TrackerGeometry* const theTrackerGeom = tGeomHandle.product();
  
  edm::Handle< std::vector< TrackingVertex > > TrackingVertexHandle;
  iEvent.getByToken(TrackingVertexToken_, TrackingVertexHandle);
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, CONESize);
  std::vector<fastjet::PseudoJet>  JetInputs;

  unsigned int this_l1track = 0;
  for ( iterL1Track = TrackingParticleHandle->begin(); iterL1Track != TrackingParticleHandle->end(); iterL1Track++ ) {
    ++this_l1track;

    float pt = iterL1Track->pt();
    float eta = iterL1Track->eta();
    float phi = iterL1Track->phi();

    float tmp_tp_vz  = iterL1Track->vz();
    float tmp_tp_vx  = iterL1Track->vx();
    float tmp_tp_vy  = iterL1Track->vy();

    float tmp_tp_z0_prod = tmp_tp_vz;
    
    
    // ---------------------------------------------------------------------------------------------
    // get d0/z0 propagated back to the IP
    
    float tmp_tp_t = tan(2.0*atan(1.0)-2.0*atan(exp(-eta)));

    float delx = -tmp_tp_vx;
    float dely = -tmp_tp_vy;

    float A = 0.01*0.5696;
    float Kmagnitude = A / pt;
    
    float tmp_tp_charge = iterL1Track->charge();
    float K = Kmagnitude * tmp_tp_charge;
    float d = 0;

    float tmp_tp_x0p = delx - (d + 1./(2. * K)*sin(phi));
    float tmp_tp_y0p = dely + (d + 1./(2. * K)*cos(phi));
    float tmp_tp_rp = sqrt(tmp_tp_x0p*tmp_tp_x0p + tmp_tp_y0p*tmp_tp_y0p);
    float tmp_tp_d0 = tmp_tp_charge*tmp_tp_rp - (1. / (2. * K));

    tmp_tp_d0 = tmp_tp_d0*(-1); //fix d0 sign

    static double pi = 4.0*atan(1.0);
    float delphi = phi-atan2(-K*tmp_tp_x0p,K*tmp_tp_y0p);
    if (delphi<-pi) delphi+=2.0*pi;
    if (delphi>pi) delphi-=2.0*pi;
    float z0 = tmp_tp_vz+tmp_tp_t*delphi/(2.0*K);


    if(z0>TRK_ZMAX)continue;
    if(eta>TRK_ETAMAX)continue;
    if(pt<TRK_PTMIN)continue;

      
    

    
    double DeltaZtoVtx=fabs(TrackingVertexHandle->begin()->position().Z()-z0);
    if(DeltaZtoVtx>DeltaZ0Cut)continue;

    fastjet::PseudoJet psuedoJet(iterL1Track->px(), iterL1Track->py(), iterL1Track->pz(), iterL1Track->p());
    JetInputs.push_back(psuedoJet);	//input tracks for clustering
    JetInputs.back().set_user_index(this_l1track-1);//save track index in the collection
  }
  fastjet::ClusterSequence cs(JetInputs,jet_def);//define the output jet collection
  std::vector<fastjet::PseudoJet> JetOutputs=fastjet::sorted_by_pt(cs.inclusive_jets(0));//Output Jet ollection pTOrdered

  for (unsigned int ijet=0;ijet<JetOutputs.size();++ijet) {
    math::XYZTLorentzVector jetP4(JetOutputs[ijet].px(),JetOutputs[ijet].py(),JetOutputs[ijet].pz(),JetOutputs[ijet].modp());
    float sumpt=0;
    float avgZ=0;
    std::vector< edm::Ptr< TrackingParticle > > L1TrackPtrs;
    std::vector<fastjet::PseudoJet> fjConstituents =fastjet::sorted_by_pt(cs.constituents(JetOutputs[ijet]));

    for(unsigned int i=0; i<fjConstituents.size(); ++i){
      auto index =fjConstituents[i].user_index();
      edm::Ptr< TrackingParticle > trkPtr(TrackingParticleHandle, index) ;
      L1TrackPtrs.push_back(trkPtr); //L1Tracks in the jet
      sumpt=sumpt+trkPtr->pt();

      float pt = trkPtr->pt();
      float eta = trkPtr->eta();
      float phi = trkPtr->phi();

      float tmp_tp_vz  = trkPtr->vz();
      float tmp_tp_vx  = trkPtr->vx();
      float tmp_tp_vy  = trkPtr->vy();

      float tmp_tp_z0_prod = tmp_tp_vz;
      
      
      // ---------------------------------------------------------------------------------------------
      // get d0/z0 propagated back to the IP
      
      float tmp_tp_t = tan(2.0*atan(1.0)-2.0*atan(exp(-eta)));

      float delx = -tmp_tp_vx;
      float dely = -tmp_tp_vy;

      float A = 0.01*0.5696;
      float Kmagnitude = A / pt;
      
      float tmp_tp_charge = iterL1Track->charge();
      float K = Kmagnitude * tmp_tp_charge;
      float d = 0;

      float tmp_tp_x0p = delx - (d + 1./(2. * K)*sin(phi));
      float tmp_tp_y0p = dely + (d + 1./(2. * K)*cos(phi));
      float tmp_tp_rp = sqrt(tmp_tp_x0p*tmp_tp_x0p + tmp_tp_y0p*tmp_tp_y0p);
      float tmp_tp_d0 = tmp_tp_charge*tmp_tp_rp - (1. / (2. * K));

      tmp_tp_d0 = tmp_tp_d0*(-1); //fix d0 sign

      static double pi = 4.0*atan(1.0);
      float delphi = phi-atan2(-K*tmp_tp_x0p,K*tmp_tp_y0p);
      if (delphi<-pi) delphi+=2.0*pi;
      if (delphi>pi) delphi-=2.0*pi;
      float z0 = tmp_tp_vz+tmp_tp_t*delphi/(2.0*K);
      

      avgZ=avgZ+pt*z0;
    }
    avgZ=avgZ/sumpt;
    edm::Ref< JetBxCollection > jetRef ;
    TkJet trkJet(jetP4, jetRef, L1TrackPtrs, avgZ);
    L1TrackerGenJets->push_back(trkJet);

  }


  iEvent.put( std::move(L1TrackerGenJets), outputname);

}


// ------------ method called once each job just before starting event loop  ------------
void
L1TrackerGenJetProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1TrackerGenJetProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
L1TrackerGenJetProducer::beginRun(edm::Run& iRun, edm::EventSetup const& iSetup)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
L1TrackerGenJetProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
L1TrackerGenJetProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
L1TrackerGenJetProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TrackerGenJetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TrackerGenJetProducer);
