import FWCore.ParameterSet.Config as cms
from math import pi
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing


process = cms.Process("TEST")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

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

process.pL1TrackerGenEt =cms.Path(process.L1TrackerGenEtMiss)


process.load("L1Trigger.L1TNtuples.l1TrackMETTreeProducer_cfi")


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('L1Ntuple.root')
)

process.p = cms.Path(process.TTTracksEmulation+
                                process.TTTracksEmulationWithTruth+
                                process.pTkPrimaryVertex+
                                process.pL1TrackerGenEt+
                                process.pL1TrackerEtMiss)