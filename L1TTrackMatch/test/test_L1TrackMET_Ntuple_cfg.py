import FWCore.ParameterSet.Config as cms
from math import pi
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing


process = cms.Process("TEST")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')


process.load('Configuration.Geometry.GeometryExtended2026D35Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D35_cff')

options = VarParsing.VarParsing ('analysis')
# get and parse the command line arguments

options.register('skipEvents',
                 0,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")
options.register('outFile',
                 'L1Ntuple.root',
                  VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 'Output file')

options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

fileList = FileUtils.loadListFromFile('snu.list')
readFiles = cms.untracked.vstring(*fileList)

process.source = process.source = cms.Source("PoolSource",
  fileNames = readFiles,
)

process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

'''
process.load("L1Trigger.TrackFindingTracklet.L1TrackletEmulationTracksWithQuality_cff")
#process.TTTracksEmulation = cms.Path(process.L1HybridTracksWithQuality)
#process.TTTracksEmulationWithTruth = cms.Path(process.L1HybridTracksWithAssociatorsWithQuality)


NHELIXPAR = 4
L1TRK_NAME  = "TTTracksFromTrackletEmulationWithQuality"
L1TRK_LABEL = "Level1TTTracks"
L1TRUTH_NAME = "TTTrackAssociatorFromPixelDigisWithQuality"


process.load("L1Trigger.VertexFinder.VertexProducer_cff")
process.VertexProducer.l1TracksInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.VertexProducer.mcTruthTrackInputTag = cms.InputTag(L1TRUTH_NAME, L1TRK_LABEL)
#process.pTkPrimaryVertex = cms.Path( process.VertexProducer )


process.load("L1Trigger.L1TTrackMatch.L1TrackerEtMissProducer_cfi")
process.L1TrackerEtMiss.L1TrackInputTag = cms.InputTag(L1TRK_NAME, L1TRK_LABEL) 
process.L1TrackerEtMiss.L1METTag = cms.string("TrackMET")
process.L1TrackerEtMiss.L1METTag = cms.string("GenMET")
#process.pL1TrackerEtMiss=cms.Path(process.L1TrackerEtMiss)

#process.pL1TrackerGenEt =cms.Path(process.L1TrackerGenEtMiss)

'''
process.load("L1Trigger.L1TNtuples.l1TrackMETTreeProducer_cfi")


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('L1Ntuple.root')
)

process.p = cms.Path(
                     process.L1HybridTracksWithAssociatorsWithQuality+
                     process.VertexProducer+
                     process.L1TrackerGenEtMiss+
                     process.L1TrackerEtMiss)