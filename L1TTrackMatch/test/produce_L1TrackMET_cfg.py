############################################################
# define basic process
############################################################

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
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



options = VarParsing.VarParsing ('Analysis')

options.parseArguments()


#--- input and output
inputFiles = []
#inputFiles = ['/store/relval/CMSSW_11_1_0/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU25ns_110X_mcRun4_realistic_v3_2026D49PU200-v1/10000/55A5DB80-84E7-2746-819E-2ECAFB126BD2.root']    
for filePath in options.inputFiles:
    if filePath.endswith(".root"):
        inputFiles.append(filePath)
    else:
        inputFiles += FileUtils.loadListFromFile(filePath)

############################################################
# input and output
############################################################


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )


process.source = cms.Source ("PoolSource",
                            fileNames = cms.untracked.vstring(inputFiles),
                            secondaryFileNames = cms.untracked.vstring(),
                            # skipEvents = cms.untracked.uint32(500)
                            )


process.Timing = cms.Service("Timing", summaryOnly = cms.untracked.bool(True))


process.load("L1Trigger.TrackFindingTracklet.L1TrackletEmulationTracksWithQuality_cff")
process.TTTracksEmulation = cms.Path(process.L1HybridTracksWithQuality)
process.TTTracksEmulationWithTruth = cms.Path(process.L1HybridTracksWithAssociatorsWithQuality)


NHELIXPAR = 4
L1TRK_NAME  = "TTTracksFromTrackletEmulationWithQuality"
L1TRK_LABEL = "Level1TTTracks"
L1TRUTH_NAME = "TTTrackAssociatorFromPixelDigisWithQuality"


process.load("L1Trigger.VertexFinder.VertexProducer_cff")
process.VertexProducer.l1TracksInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.VertexProducer.mcTruthTrackInputTag = cms.InputTag(L1TRUTH_NAME, L1TRK_LABEL)
process.pTkPrimaryVertex = cms.Path( process.VertexProducer )


process.load("L1Trigger.L1TTrackMatch.L1TrackerEtMissProducer_cfi")
process.L1TrackerEtMiss.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerEtMiss.L1METTag = cms.string("TrackMET")
process.pL1TrackerEtMiss=cms.Path(process.L1TrackerEtMiss)

process.L1TrackerEtMissPurityCut.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerEtMissPurityCut.L1METTag = cms.string("TrackMETPurityCut")
process.pL1TrackerEtMissPurityCut =cms.Path(process.L1TrackerEtMissPurityCut)

process.L1TrackerEtMissMVACut.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerEtMissMVACut.L1METTag = cms.string("TrackMETMVACut")
process.L1TrackerEtMissMVACut.MVAThreshold = 0.3
process.pL1TrackerEtMissMVACut =cms.Path(process.L1TrackerEtMissMVACut)

process.pL1TrackerGenEt =cms.Path(process.L1TrackerGenEtMiss)

process.out = cms.OutputModule( "PoolOutputModule",
                                fastCloning = cms.untracked.bool( False ),
                                fileName = cms.untracked.string(options.outputFile),
                                outputCommands = cms.untracked.vstring(
#                            	"keep *",
     	                        "keep *_L1TrackerEtMiss*_*_*",
                                "keep *_L1TrackerGenEtMiss*"
     	                        "keep *_VertexProducer_*_*"
     	                        )
		               )
process.FEVToutput_step = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.TTTracksEmulation,
                                process.TTTracksEmulationWithTruth,
                                process.pTkPrimaryVertex,
                                process.pL1TrackerEtMiss,
                                process.pL1TrackerEtMissPurityCut,
                                process.pL1TrackerEtMissMVACut,
                                process.pL1TrackerGenEt,
                                process.FEVToutput_step)
