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
process.L1TrackerEtMiss.L1TrkMETTag = cms.string("TrkMet")
process.pL1TrackerEtMiss=cms.Path(process.L1TrackerEtMiss)

process.L1TrackerEtMissPurityCut.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerEtMissPurityCut.L1TrkMETTag = cms.string("CutTrkMet")
process.pL1TrackerEtMissPurityCut =cms.Path(process.L1TrackerEtMissPurityCut)

process.L1TrackerEtMissMVACut.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerEtMissMVACut.L1TrkMETTag = cms.string("MVATrkMet")
process.L1TrackerEtMissMVACut.MVAThreshold = 0.3
process.pL1TrackerEtMissMVACut =cms.Path(process.L1TrackerEtMissMVACut)

process.pL1TrackerGenEt =cms.Path(process.L1TrackerGenEtMiss)


process.load("L1Trigger.L1TTrackMatch.L1TrackerJetProducer_cfi")
process.L1TrackerJets.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerJets.L1TrkJetTag = cms.string("L1TrackerJets")
process.pL1TrackerJets=cms.Path(process.L1TrackerJets)

process.L1TrackerJetsPurityCut.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerJetsPurityCut.L1TrkJetTag = cms.string("CutL1TrackerJets")
process.pL1TrackerJetsPurityCut =cms.Path(process.L1TrackerJetsPurityCut)

process.L1TrackerJetsMVACut.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerJetsMVACut.L1TrkJetTag = cms.string("MVAL1TrackerJets")
process.L1TrackerJetsMVACut.MVAThreshold = 0.3
process.pL1TrackerJetsMVACut =cms.Path(process.L1TrackerJetsMVACut)



#process.pL1TrackerGenJets =cms.Path(process.L1TrackerGenJets)




'''
process.load('L1Trigger.L1CaloTrigger.Phase1L1TJets_cff')

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')


process.load("L1Trigger.Phase2L1ParticleFlow.l1ParticleFlow_cff")
process.pfTracksFromL1Tracks.L1TrackTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.pfTracksFromL1TracksHGCal.L1TrackTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 

process.pPF = cms.Path(process.l1ParticleFlow)

process.load("L1Trigger.Phase2L1ParticleFlow.l1pfJetMet_cff")
process.pPFMET = cms.Path(process.l1PFMets)


'''

process.out = cms.OutputModule( "PoolOutputModule",
                                fastCloning = cms.untracked.bool( False ),
                                fileName = cms.untracked.string(options.outputFile),
                                dataset = cms.untracked.PSet(
                                filterName = cms.untracked.string(''),
                                dataTier = cms.untracked.string('GEN-SIM')
                                ),
                                outputCommands = cms.untracked.vstring(
                            	"keep *_L1TrackerJets*_*_*",
     	                        "keep *_L1TrackerEtMiss*_*_*",
                                "keep *_L1TrackerGenEtMiss*_*_*",
     	                        "keep *_VertexProducer_*_*",
                                "keep  *TTTrack*_*_*_*",
                                "keep  *TTStub*_*_*_*"
     	                        )
		               )
process.FEVToutput_step = cms.EndPath(process.out)
process.schedule = cms.Schedule(
                                process.TTTracksEmulationWithTruth,
                                process.pTkPrimaryVertex,
                                process.pL1TrackerEtMiss,
                                process.pL1TrackerEtMissPurityCut,
                                process.pL1TrackerEtMissMVACut,
                                process.pL1TrackerGenEt,

                                process.pL1TrackerJets,
                                process.pL1TrackerJetsPurityCut,
                                process.pL1TrackerJetsMVACut,
                                process.pL1TrackerGenJets,
                                #process.pPF,
                                #process.pPFMET,
                                process.FEVToutput_step)
