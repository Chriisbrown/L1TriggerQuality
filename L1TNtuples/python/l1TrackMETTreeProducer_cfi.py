import FWCore.ParameterSet.Config as cms

l1TrackMETTree = cms.EDAnalyzer("L1TrackMETTreeProducer",

   genMETToken = cms.InputTag("L1TrackerGenEtMiss","GenMET"),
   TkMETToken  = cms.InputTag("L1TrackerEtMiss","TrackMET"),
   CutTkMETToken = cms.InputTag("L1TrackerEtMissPurityCut","TrackMETPurityCut"),
   MVATkMETToken = cms.InputTag("L1TrackerEtMissMVACut","TrackMETMVACut"),

)

runmenutree=cms.Path(l1TrackMETTree)

