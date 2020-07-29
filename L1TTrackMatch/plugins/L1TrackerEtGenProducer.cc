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

class L1TrackerEtGenProducer : public edm::EDProducer {
public:

  
  typedef std::vector< TrackingParticle > L1TTTrackCollectionType;

  explicit L1TrackerEtGenProducer(const edm::ParameterSet&);
  ~L1TrackerEtGenProducer();

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  float DeltaZ;	    // in cm

  std::string outputname;

  const edm::EDGetTokenT< std::vector< TrackingParticle > > TrackingParticleToken_;
  const edm::EDGetTokenT< std::vector< TrackingVertex > > TrackingVertexToken_;
};

///////////////
//constructor//
///////////////
L1TrackerEtGenProducer::L1TrackerEtGenProducer(const edm::ParameterSet& iConfig) :
TrackingParticleToken_(consumes< std::vector< TrackingParticle > > (iConfig.getParameter<edm::InputTag>("L1TrackingTrackInputTag")))
TrackingVertexToken_(consumes< std::vector< TrackingVertex > >(iConfig.getParameter<edm::InputTag>("L1TrackingVertexInputTag"))),
{
  DeltaZ = (float)iConfig.getParameter<double>("DeltaZ");
  outputname = iConfig.getParameter<std::string>("L1METTag");

  produces<TkEtMissCollection>(outputname);
}

//////////////
//destructor//
//////////////
L1TrackerEtGenProducer::~L1TrackerEtGenProducer() {
}

////////////
//producer//
////////////
void L1TrackerEtGenProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
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

  if( !TrackingVertexHandle.isValid() ) {
    LogError("L1TrackerEtGenProducer")<< "\nWarning: VertexCollection not found in the event. Exit"<< std::endl;
    return;
  }

  if( !TrackingParticleHandle.isValid() ) {
    LogError("L1TrackerEtGenProducer")<< "\nWarning: L1TTTrackCollection not found in the event. Exit"<< std::endl;
    return;
  }


  float sumPx = 0;
  float sumPy = 0;
  float etTot = 0;
  double sumPx_PU = 0;
  double sumPy_PU = 0;
  double etTot_PU = 0;

  float zVTX = TrackingVertexHandle->begin()->position().Z();

  for (trackIter = TrackingParticleHandle->begin(); trackIter != TrackingParticleHandle->end(); ++trackIter) {
    
    float pt = trackIter->pt();
    float phi = trackIter->phi();
    float eta = trackIter->eta();

    float tmp_tp_vz  = trackIter->vz();
    float tmp_tp_vx  = trackIter->vx();
    float tmp_tp_vy  = trackIter->vy();

    float tmp_tp_z0_prod = tmp_tp_vz;
    
    
    // ---------------------------------------------------------------------------------------------
    // get d0/z0 propagated back to the IP
    
    float tmp_tp_t = tan(2.0*atan(1.0)-2.0*atan(exp(-eta)));

    float delx = -tmp_tp_vx;
    float dely = -tmp_tp_vy;

    float A = 0.01*0.5696;
    float Kmagnitude = A / pt;
    
    float tmp_tp_charge = trackIter->charge();
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
    float tmp_tp_z0 = tmp_tp_vz+tmp_tp_t*delphi/(2.0*K);

    
    // construct deltaZ cut to be based on track eta
    if      ( fabs(eta)>=0   &&  fabs(eta)<0.7)  DeltaZ = 0.4;
    else if ( fabs(eta)>=0.7 &&  fabs(eta)<1.0)  DeltaZ = 0.6;
    else if ( fabs(eta)>=1.0 &&  fabs(eta)<1.2)  DeltaZ = 0.76;
    else if ( fabs(eta)>=1.2 &&  fabs(eta)<1.6)  DeltaZ = 1.0;
    else if ( fabs(eta)>=1.6 &&  fabs(eta)<2.0)  DeltaZ = 1.7;
    else if ( fabs(eta)>=2.0 &&  fabs(eta)<=2.4) DeltaZ = 2.2;

    if ( fabs(tmp_tp_z0 - zVTX) <= DeltaZ) {
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

void L1TrackerEtGenProducer::beginJob() {
}

void L1TrackerEtGenProducer::endJob() {
}

DEFINE_FWK_MODULE(L1TrackerEtGenProducer);
