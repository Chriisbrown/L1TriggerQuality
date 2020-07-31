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

process.load("L1Trigger.L1TNtuples.l1TrackMETTreeProducer_cfi")
process.l1TrackMETTree.TkMETToken = cms.string("TrackMET")
process.pl1TrackMETTree=cms.Path(process.l1TrackMETTree)

process.l1TrackMETTree.TkMETToken = cms.string("TrackMETPurityCut")
process.pl1TrackMETTreecut=cms.Path(process.l1TrackMETTree)

process.l1TrackMETTree.TkMETToken = cms.string("TrackMETMVACut")
process.pl1TrackMETTreeMVA=cms.Path(process.l1TrackMETTree)


process.out = cms.OutputModule( "PoolOutputModule",
                                fastCloning = cms.untracked.bool( False ),
                                fileName = cms.untracked.string('L1Ntuple.root')
                              )
process.schedule = cms.Schedule(process.pl1TrackMETTree,
                                process.pl1TrackMETTreecut,
                                process.pl1TrackMETTreeMVA)
  