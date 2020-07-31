import FWCore.ParameterSet.Config as cms

l1TrackMETTree = cms.EDAnalyzer("L1TrackMETTreeProducer",

   genMETToken = cms.InputTag("L1TrackerGenEtMiss","GenMET"),
   TkMETToken  = cms.InputTag("L1TrackerEtMiss","TrackMET"),
   CutTkMETToken = cms.InputTag("L1TrackerEtMiss","TrackMETPurityCut"),
   MVATkMETToken = cms.InputTag("L1TrackerEtMiss","TrackMETMVACut"),

)

runmenutree=cms.Path(l1TrackMETTree)

