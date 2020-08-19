import FWCore.ParameterSet.Config as cms

l1TrackMETTree = cms.EDAnalyzer("L1TrackMETTreeProducer",

   genMETToken = cms.InputTag("genMetTrue"),
   TkMETToken  = cms.InputTag("L1TrackerEtMiss","TrkMet"),
   CutTkMETToken = cms.InputTag("L1TrackerEtMissPurityCut","CutTrkMet"),
   MVATkMETToken = cms.InputTag("L1TrackerEtMissMVACut","MVATrkMet"),
   SimTkMETToken = cms.InputTag("L1TrackerGenEtMiss","SimMET"),

)

runmenutree=cms.Path(l1TrackMETTree)

