// -*- C++ -*-
//
// Package:    L1TrackTrigger
// Class:      L1WP2ElectronMatchAlgo
// 
/**\class L1WP2ElectronMatchAlgo 

 Description: Producer of a TkEm

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  C. Caillol
//         Created:  Jun 4 2019
// $Id$
//
//
// -*- C++ -*-
//
//
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

#include "DataFormats/L1TCorrelator/interface/TkElectron.h"
#include "DataFormats/L1TCorrelator/interface/TkElectronFwd.h"
#include "DataFormats/L1TCorrelator/interface/TkEm.h"
#include "DataFormats/L1TCorrelator/interface/TkEmFwd.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" 
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

// Matching Algorithm
#include "L1Trigger/L1TTrackMatch/interface/L1TkElectronTrackMatchAlgo.h"
#include "L1Trigger/L1TTrackMatch/interface/L1TkElectronEtComparator.h"
#include "L1Trigger/L1TTrackMatch/interface/pTFrom2Stubs.h"

#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"

#include "DataFormats/L1TCorrelator/interface/TkPrimaryVertex.h"

#include <string>
#include "TMath.h"


using namespace l1t ;

//
// class declaration
//

class L1WP2ElectronProducer : public edm::EDProducer {
   public:

   typedef TTTrack< Ref_Phase2TrackerDigi_ >  L1TTTrackType;
   typedef std::vector< L1TTTrackType > L1TTTrackCollectionType;

      explicit L1WP2ElectronProducer(const edm::ParameterSet&);
      ~L1WP2ElectronProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      //virtual void beginRun(edm::Run&, edm::EventSetup const&);
      //virtual void endRun(edm::Run&, edm::EventSetup const&);
      //virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      //virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      float isolationTk(const edm::Handle<L1TTTrackCollectionType> & trkHandle, int match_index);
      float isolationVtx(const edm::Handle<L1TTTrackCollectionType> & trkHandle, float zvtx, float eta, float phi);
      double getPtScaledCut(double pt, std::vector<double>& parameters);

      // ----------member data ---------------------------
	std::string label;
         
	float ETmin; 	// min ET in GeV of L1EG objects

	float DRmin;
	float DRmax;
	float PTMINTRA;
	bool PrimaryVtxConstrain;	// use the primary vertex (default = false)
	float DeltaZ;      	// | z_track - z_ref_track | < DeltaZ in cm. 
				// Used only when PrimaryVtxConstrain = True.
	float IsoCut;
	bool RelativeIsolation;

        float trkQualityChi2;
	bool useTwoStubsPT;
        float trkQualityPtMin; 
        std::vector<double> dPhiCutoff;
        std::vector<double> dRCutoff;
        float dEtaCutoff;
  
        const edm::EDGetTokenT< EGammaBxCollection > egToken;
        const edm::EDGetTokenT< std::vector< TTTrack< Ref_Phase2TrackerDigi_ > > > trackToken;
        const edm::EDGetTokenT<TkPrimaryVertexCollection> vertexToken;
} ;


//
// constructors and destructor
//
L1WP2ElectronProducer::L1WP2ElectronProducer(const edm::ParameterSet& iConfig) :
  egToken(consumes< EGammaBxCollection >(iConfig.getParameter<edm::InputTag>("L1EGammaInputTag"))),
  trackToken(consumes< std::vector<TTTrack< Ref_Phase2TrackerDigi_> > > (iConfig.getParameter<edm::InputTag>("L1TrackInputTag"))),
  vertexToken(consumes< TkPrimaryVertexCollection >(iConfig.getParameter<edm::InputTag>("L1VertexInputTag")))
  {

   label = iConfig.getParameter<std::string>("label");  // label of the collection produced
							// e.g. EG or IsoEG if all objects are kept
							// EGIsoTrk or IsoEGIsoTrk if only the EG or IsoEG
							// objects that pass a cut RelIso < IsoCut are written
							// in the new collection.

   ETmin = (float)iConfig.getParameter<double>("ETmin");

   // parameters for the calculation of the isolation :
   PTMINTRA = (float)iConfig.getParameter<double>("PTMINTRA");
   DRmin = (float)iConfig.getParameter<double>("DRmin");
   DRmax = (float)iConfig.getParameter<double>("DRmax");
   DeltaZ = (float)iConfig.getParameter<double>("DeltaZ");

   // cut applied on the isolation (if this number is <= 0, no cut is applied)
   IsoCut = (float)iConfig.getParameter<double>("IsoCut");
   RelativeIsolation = iConfig.getParameter<bool>("RelativeIsolation");

   // parameters to select tracks to match with L1EG
   trkQualityChi2  = (float)iConfig.getParameter<double>("TrackChi2");
   trkQualityPtMin = (float)iConfig.getParameter<double>("TrackMinPt");
   useTwoStubsPT   = iConfig.getParameter<bool>("useTwoStubsPT");
   dPhiCutoff      = iConfig.getParameter< std::vector<double> >("TrackEGammaDeltaPhi"); 
   dRCutoff        = iConfig.getParameter< std::vector<double> >("TrackEGammaDeltaR"); 
   dEtaCutoff      = (float)iConfig.getParameter<double>("TrackEGammaDeltaEta"); 

   produces<TkEmCollection>(label);
}

L1WP2ElectronProducer::~L1WP2ElectronProducer() {
}

// ------------ method called to produce the data  ------------
void
L1WP2ElectronProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::unique_ptr<TkEmCollection> result(new TkEmCollection);
  
  // geometry needed to call pTFrom2Stubs
  edm::ESHandle<TrackerGeometry> geomHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get("idealForDigi", geomHandle);
  const TrackerGeometry* tGeom = geomHandle.product();  

	// the L1EGamma objects
  edm::Handle<EGammaBxCollection> eGammaHandle;
  iEvent.getByToken(egToken, eGammaHandle);  
  EGammaBxCollection eGammaCollection = (*eGammaHandle.product());
  EGammaBxCollection::const_iterator egIter;

   // the L1Tracks
   edm::Handle<L1TTTrackCollectionType> L1TTTrackHandle;
   iEvent.getByToken(trackToken, L1TTTrackHandle);
   L1TTTrackCollectionType::const_iterator trackIter;

  if( !eGammaHandle.isValid() ) {
    edm::LogError("L1WP2ElectronProducer")
      << "\nWarning: L1EmCollection not found in the event. Exit"
      << std::endl;
    return;
  }
  if (!L1TTTrackHandle.isValid() ) {
    edm::LogError("L1WP2ElectronProducer")
      << "\nWarning: L1TTTrackCollectionType not found in the event. Exit."
      << std::endl;
    return;
  }

  float zvtxL1tk = -999;
  edm::Handle<TkPrimaryVertexCollection> L1VertexHandle;
  iEvent.getByToken(vertexToken, L1VertexHandle);
  if(!L1VertexHandle.isValid() ) {
    edm::LogError("L1WP2ElectronProducer")
      << "\nWarning: L1VertexCollection not found in the event. Exit"
      << std::endl;
    return;
  }
  else {
    std::vector<TkPrimaryVertex>::const_iterator vtxIter = L1VertexHandle->begin();
    zvtxL1tk = vtxIter -> zvertex();
  }

  int ieg = 0;
  for (egIter = eGammaCollection.begin(0); egIter != eGammaCollection.end(0);  ++egIter){ // considering BX = only
    edm::Ref< EGammaBxCollection > EGammaRef( eGammaHandle, ieg );
    ieg ++; 

    float e_ele   = egIter->energy();
    float eta_ele = egIter->eta();
    float et_ele = 0;
    if (cosh(eta_ele) > 0.0) et_ele = e_ele/cosh(eta_ele);
    else et_ele = -1.0;
    if (ETmin > 0.0 && et_ele <= ETmin) continue;
    // match the L1EG object with a L1Track
    float drmin = 999;
    int itr = 0;
    int itrack = -1;

    for (trackIter = L1TTTrackHandle->begin(); trackIter != L1TTTrackHandle->end(); ++trackIter) {
      edm::Ptr< L1TTTrackType > L1TrackPtr( L1TTTrackHandle, itr) ;
      double trkPt_fit = trackIter->momentum().perp();
      double trkPt_stubs = pTFrom2Stubs::pTFrom2( trackIter, tGeom);
      double trkPt = trkPt_fit;
      if ( useTwoStubsPT ) trkPt = trkPt_stubs ;

      if ( trkPt > trkQualityPtMin && trackIter->chi2() < trkQualityChi2) {
	double dPhi = 99.;
	double dR = 99.;
	double dEta = 99.;   
	L1TkElectronTrackMatchAlgo::doMatch(egIter, L1TrackPtr, dPhi, dR, dEta); 

	if (fabs(dPhi) < getPtScaledCut(trkPt, dPhiCutoff) && dR < getPtScaledCut(trkPt, dRCutoff) && dR < drmin) {
	  drmin = dR;
	  itrack = itr;
	}
      }
      itr++;
    }

    float trkisol=0.0;
    const math::XYZTLorentzVector P4 = egIter -> p4() ;

    edm::Ptr< L1TTTrackType > matchedL1TrackPtr(L1TTTrackHandle, 0);
    if (itrack >= 0)  {
      edm::Ptr< L1TTTrackType > matchedL1TrackPtr_2(L1TTTrackHandle, itrack);      
      matchedL1TrackPtr=matchedL1TrackPtr_2;
      trkisol = isolationTk(L1TTTrackHandle, itrack);
    }
    else{
      trkisol = isolationVtx(L1TTTrackHandle,zvtxL1tk,egIter->eta(),egIter->phi());
    }

    if (RelativeIsolation && et_ele > 0.0) {   // relative isolation
      trkisol = trkisol  / et_ele;
    }
    
    TkEm trkEm( P4, 
      			  EGammaRef,
			  trkisol,
      			  trkisol );

    if (IsoCut <= 0) {
      // write the L1TkEm particle to the collection, 
      // irrespective of its relative isolation
      result -> push_back( trkEm );
    }	else {
      // the object is written to the collection only
      // if it passes the isolation cut
      if (trkisol <= IsoCut) result -> push_back( trkEm );
    }

  } // end loop over EGamma objects
  
  iEvent.put(std::move(result), label );

}

// ------------ method called once each job just before starting event loop  ------------
void
L1WP2ElectronProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1WP2ElectronProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
L1WP2ElectronProducer::beginRun(edm::Run& iRun, edm::EventSetup const& iSetup)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
L1WP2ElectronProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
L1WP2ElectronProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
L1WP2ElectronProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1WP2ElectronProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
// method to calculate isolation
float 
L1WP2ElectronProducer::isolationTk(const edm::Handle<L1TTTrackCollectionType> & trkHandle, int match_index) {
  edm::Ptr< L1TTTrackType > matchedTrkPtr (trkHandle, match_index) ; 
  L1TTTrackCollectionType::const_iterator trackIter;

  float sumPt = 0.0;
  int itr = 0;
  for (trackIter = trkHandle->begin(); trackIter != trkHandle->end(); ++trackIter) {
    if (itr != match_index) {   
      float dZ = fabs(trackIter->POCA().z() - matchedTrkPtr->POCA().z() );
    
      float phi1 = trackIter->momentum().phi();
      float phi2 = matchedTrkPtr->momentum().phi();
      float dPhi = reco::deltaPhi(phi1, phi2);
      float dEta = (trackIter->momentum().eta() - matchedTrkPtr->momentum().eta());
      float dR =  sqrt(dPhi*dPhi + dEta*dEta);
      
      if (dR > DRmin && dR < DRmax && dZ < DeltaZ && trackIter->momentum().perp() > PTMINTRA) {
	sumPt += trackIter->momentum().perp();
      }
    }
    itr++;
  }
  return sumPt;
}

float
L1WP2ElectronProducer::isolationVtx(const edm::Handle<L1TTTrackCollectionType> & trkHandle, float zvtx, float eta, float phi) {
  L1TTTrackCollectionType::const_iterator trackIter;

  float sumPt = 0.0;
  int itr = 0;
  for (trackIter = trkHandle->begin(); trackIter != trkHandle->end(); ++trackIter) {
    float dZ = fabs(trackIter->POCA().z() - zvtx);
    float phi1 = trackIter->momentum().phi();
    float phi2 = phi;
    float dPhi = reco::deltaPhi(phi1, phi2);
    float dEta = (trackIter->momentum().eta() - eta);
    float dR =  sqrt(dPhi*dPhi + dEta*dEta);

    if (dR < DRmax && dZ < DeltaZ && trackIter->momentum().perp() > PTMINTRA) {
      sumPt += trackIter->momentum().perp();
    }
    itr++;
  }
  return sumPt;
}

double
L1WP2ElectronProducer::getPtScaledCut(double pt, std::vector<double>& parameters){
  return (parameters[0] + parameters[1] * exp(parameters[2] * pt));
}
//define this as a plug-in
DEFINE_FWK_MODULE(L1WP2ElectronProducer);




