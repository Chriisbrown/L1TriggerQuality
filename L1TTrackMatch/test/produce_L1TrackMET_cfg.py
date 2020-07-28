############################################################
# define basic process
############################################################

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import os
process = cms.Process("L1TrackMET")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')

process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D49_cff')    ## this needs to match the geometry you are running on

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')



from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

############################################################
# input and output
############################################################


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
inputMC = ['/store/relval/CMSSW_11_1_0/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_110X_mcRun4_realistic_v3_2026D49PU200-v1/10000/55A5DB80-84E7-2746-819E-2ECAFB126BD2.root']    

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(*inputMC))

process.TFileService = cms.Service("TFileService", fileName = cms.string('TRACKMET.root'), closeFileFast = cms.untracked.bool(True))
process.Timing = cms.Service("Timing", summaryOnly = cms.untracked.bool(True))

process.load("L1Trigger.TrackFindingTracklet.L1HybridEmulationTracks_cff")
process.TTTracksEmulation = cms.Path(process.L1HybridTracks)
process.TTTracksEmulationWithTruth = cms.Path(process.L1HybridTracksWithAssociators)
NHELIXPAR = 4
L1TRK_NAME  = "TTTracksFromTrackletEmulation"
L1TRK_LABEL = "Level1TTTracks"
L1TRK_CLASSLABEL ="Level1ClassTTTracks"
L1TRUTH_NAME = "TTTrackAssociatorFromPixelDigis"

process.load("L1Trigger.TrackQuality.Classifier_cff")
process.TrackClassifier.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.TrackClassifier.Algorithm = cms.string("NN")
process.pTrackClass = cms.Path( process.TrackClassifier)

process.load("L1Trigger.VertexFinder.VertexProducer_cff")
process.VertexProducer.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_CLASSLABEL) 
process.pTkPrimaryVertex = cms.Path( process.VertexProducer )


process.load("L1Trigger.L1TTrackMatch.L1TrackerEtMissProducer_cfi")
process.L1TrackerEtMiss.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_CLASSLABEL) 
process.pL1TrackerEtMiss=cms.Path(process.L1TrackerEtMiss)

process.out = cms.OutputModule( "PoolOutputModule",
                                fastCloning = cms.untracked.bool( False ),
                                fileName = cms.untracked.string("test.root" )
		               )
process.FEVToutput_step = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.TTTracksEmulation,
                                process.TTTracksEmulationWithTruth,
                                process.pTrackClass,
                                process.pTkPrimaryVertex,
                                process.pL1TrackerEtMiss,
                                process.FEVToutput_step)
