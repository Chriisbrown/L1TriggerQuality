import FWCore.ParameterSet.Config as cms

l1TrackJetTree = cms.EDAnalyzer("L1TrackJetTreeProducer",
   genJetToken     = cms.untracked.InputTag("ak4GenJetsNoNu"),
   l1TrackJets = cms.InputTag("L1TrackerJets","L1TrackerJets"),
   l1CutTrackJets = cms.InputTag("L1TrackerJetsPurityCut","CutL1TrackerJets"),
   l1MVATrackJets = cms.InputTag("L1TrackerJetsMVACut","MVAL1TrackerJets"),




   maxL1Extra = cms.uint32(20)
)

runmenutree=cms.Path(l1TrackJetTree)