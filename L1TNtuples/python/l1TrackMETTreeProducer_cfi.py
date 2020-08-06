import FWCore.ParameterSet.Config as cms

l1TrackMETTree = cms.EDAnalyzer("L1TrackMETTreeProducer",

   genMETToken = cms.InputTag("L1TrackerGenEtMiss","GenMet"),
   TkMETToken  = cms.InputTag("L1TrackerEtMiss","TrkMet"),
   CutTkMETToken = cms.InputTag("L1TrackerEtMissPurityCut","CutTrkMet"),
   MVATkMETToken = cms.InputTag("L1TrackerEtMissMVACut","MVATrkMet"),

)

runmenutree=cms.Path(l1TrackMETTree)

