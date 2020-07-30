import FWCore.ParameterSet.Config as cms

l1TrackMETTree = cms.EDAnalyzer("L1TrackMETTreeProducer",

   genMETToken     = cms.untracked.InputTag("L1TrackerGenEtMiss","GenMET"),
   TkMETToken = cms.InputTag("L1TrackerEtMiss", "TrackMET"),

   maxL1Extra = cms.uint32(20)
)

runmenutree=cms.Path(l1TrackMETTree)