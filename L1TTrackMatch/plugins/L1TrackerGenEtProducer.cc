// Original Author:  Emmanuelle Perez,40 1-A28,+41227671915,
//         Created:  Tue Nov 12 17:03:19 CET 2013
//Modified by Emily MacDonald, 30 Nov 2018

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1TCorrelator/interface/TkEtMiss.h"
#include "DataFormats/L1TCorrelator/interface/TkEtMissFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "DataFormats/L1TVertex/interface/Vertex.h"

// detector geometry
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

using namespace l1t;

class L1TrackerEtMissProducer : public edm::EDProducer {
public:

  
  typedef < std::vector< TrackingParticle > L1TTTrackCollectionType;

  explicit L1TrackerEtMissProducer(const edm::ParameterSet&);
  ~L1TrackerEtMissProducer();

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  float maxZ0;	    // in cm
  float DeltaZ;	    // in cm
  float maxEta;
  float chi2dofMax;
  float bendchi2Max;
  float minPt;	    // in GeV
  int nStubsmin;
  int nStubsPSmin;  // minimum number of stubs in PS modules
  float maxPt;	    // in GeV
  int HighPtTracks; // saturate or truncate

  bool Purity_cut;
  bool MVA_cut;
  float Threshold;

  std::string outputname;

  const edm::EDGetTokenT< std::vector< TrackingParticle > > TrackingParticleToken_;
  const edm::EDGetTokenT< std::vector< TrackingVertexToken_ > > pvToken;
};

///////////////
//constructor//
///////////////
L1TrackerEtMissProducer::L1TrackerEtMissProducer(const edm::ParameterSet& iConfig) :
TrackingParticleToken_(consumes< std::vector< TrackingVertex > >(iConfig.getParameter<edm::InputTag>("L1TrackingVertexInputTag"))),
TrackingVertexToken_(consumes< std::vector< TrackingParticle > > (iConfig.getParameter<edm::InputTag>("L1TrackingTrackInputTag")))
{
  maxZ0 = (float)iConfig.getParameter<double>("maxZ0");
  DeltaZ = (float)iConfig.getParameter<double>("DeltaZ");
  chi2dofMax = (float)iConfig.getParameter<double>("chi2dofMax");
  bendchi2Max = (float)iConfig.getParameter<double>("bendchi2Max");
  minPt = (float)iConfig.getParameter<double>("minPt");
  nStubsmin = iConfig.getParameter<int>("nStubsmin");
  nStubsPSmin = iConfig.getParameter<int>("nStubsPSmin");
  maxPt = (float)iConfig.getParameter<double>("maxPt");
  maxEta = (float)iConfig.getParameter<double>("maxEta");
  HighPtTracks = iConfig.getParameter<int>("HighPtTracks");
  Threshold = (float)iConfig.getParameter<double>("MVAThreshold");
  Purity_cut = iConfig.getParameter<bool>("Cut");
  MVA_cut = iConfig.getParameter<bool>("MVACut");
  outputname = iConfig.getParameter<std::string>("L1METTag");

  produces<TkEtMissCollection>(outputname);
}

//////////////
//destructor//
//////////////
L1TrackerEtMissProducer::~L1TrackerEtMissProducer() {
}

////////////
//producer//
////////////
void L1TrackerEtMissProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  // Tracker Topology
  edm::ESHandle<TrackerTopology> tTopoHandle_;
  iSetup.get<TrackerTopologyRcd>().get(tTopoHandle_);
  const TrackerTopology* tTopo = tTopoHandle_.product();

  std::unique_ptr<TkEtMissCollection> METCollection(new TkEtMissCollection);

  edm::Handle< std::vector< TrackingParticle > > TrackingParticleHandle;
  edm::Handle< std::vector< TrackingVertex > > TrackingVertexHandle;
  iEvent.getByToken(TrackingParticleToken_, TrackingParticleHandle);
  iEvent.getByToken(TrackingVertexToken_, TrackingVertexHandle);


  L1TTTrackCollectionType::const_iterator trackIter;

  if( !L1VertexHandle.isValid() ) {
    LogError("L1TrackerEtMissProducer")<< "\nWarning: VertexCollection not found in the event. Exit"<< std::endl;
    return;
  }

  if( !L1TTTrackHandle.isValid() ) {
    LogError("L1TrackerEtMissProducer")<< "\nWarning: L1TTTrackCollection not found in the event. Exit"<< std::endl;
    return;
  }


  float sumPx = 0;
  float sumPy = 0;
  float etTot = 0;
  double sumPx_PU = 0;
  double sumPy_PU = 0;
  double etTot_PU = 0;

  float zVTX = L1VertexHandle->begin()->z0();

  for (trackIter = L1TTTrackHandle->begin(); trackIter != L1TTTrackHandle->end(); ++trackIter) {
    
    float pt = trackIter->momentum().perp();
    float phi = trackIter->momentum().phi();
    float eta = trackIter->momentum().eta();
    float z0  = trackIter->POCA().z();

    
    // construct deltaZ cut to be based on track eta
    if      ( fabs(eta)>=0   &&  fabs(eta)<0.7)  DeltaZ = 0.4;
    else if ( fabs(eta)>=0.7 &&  fabs(eta)<1.0)  DeltaZ = 0.6;
    else if ( fabs(eta)>=1.0 &&  fabs(eta)<1.2)  DeltaZ = 0.76;
    else if ( fabs(eta)>=1.2 &&  fabs(eta)<1.6)  DeltaZ = 1.0;
    else if ( fabs(eta)>=1.6 &&  fabs(eta)<2.0)  DeltaZ = 1.7;
    else if ( fabs(eta)>=2.0 &&  fabs(eta)<=2.4) DeltaZ = 2.2;

    if ( fabs(z0 - zVTX) <= DeltaZ) {
      sumPx += pt*cos(phi);
      sumPy += pt*sin(phi);
      etTot += pt ;
    }
    else {	// PU sums
      sumPx_PU += pt*cos(phi);
      sumPy_PU += pt*sin(phi);
      etTot_PU += pt ;
    }
  } // end loop over tracks

  float et = sqrt( sumPx*sumPx + sumPy*sumPy );
  double etmiss_PU = sqrt( sumPx_PU*sumPx_PU + sumPy_PU*sumPy_PU );

  math::XYZTLorentzVector missingEt( -sumPx, -sumPy, 0, et);

  int ibx = 0;
  METCollection->push_back( TkEtMiss( missingEt,
    TkEtMiss::kMET,
    etTot,
    etmiss_PU,
    etTot_PU,
    ibx ) );

  iEvent.put( std::move(METCollection), outputname);
} // end producer

void L1TrackerEtMissProducer::beginJob() {
}

void L1TrackerEtMissProducer::endJob() {
}

DEFINE_FWK_MODULE(L1TrackerEtMissProducer);
