#################################################################################################
# To run execute do
# cmsRun tmtt_tf_analysis_cfg.py Events=50 inputMC=Samples/Muons/PU0.txt histFile=outputHistFile.root
# where the arguments take default values if you don't specify them. You can change defaults below.
#################################################################################################

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("FastL1TVertexFinder")

#process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2023D17_cff')
process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D49_cff')


process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')
process.load("FWCore.MessageLogger.MessageLogger_cfi")


options = VarParsing.VarParsing ('analysis')
options.register('analysis',True,VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool,"Run vertex finding analysis code")
options.register('histFile','Hist.root',VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,"Name of output histogram file")
options.register('l1Tracks','TTTracksFromTrackletEmulation:Level1TTTracks', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, 'L1 track collection to use')

options.parseArguments()


#--- input and output
inputFiles = []
for filePath in options.inputFiles:
    if filePath.endswith(".root"):
        inputFiles.append(filePath)
    else:
        inputFiles += FileUtils.loadListFromFile(filePath)

if options.l1Tracks.count(':') != 1:
    raise RuntimeError("Value for 'l1Tracks' command-line argument (= '{}') should contain one colon".format(options.l1Tracks))

l1TracksTag = cms.InputTag(options.l1Tracks.split(':')[0], options.l1Tracks.split(':')[1])
print "  INPUT TRACK COLLECTION = {0}  {1}".format(*options.l1Tracks.split(':')) 
    
process.load("L1Trigger.TrackFindingTracklet.L1HybridEmulationTracks_cff")


process.TTTracksEmulation = cms.Path(process.L1HybridTracks)
process.TTTracksEmulationWithTruth = cms.Path(process.L1HybridTracksWithAssociators)


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )#,SkipEvent = cms.untracked.vstring('ProductNotFound'))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

if options.analysis:
    process.TFileService = cms.Service("TFileService", fileName = cms.string(options.histFile))

process.source = cms.Source ("PoolSource",
                            fileNames = cms.untracked.vstring(inputFiles),
                            secondaryFileNames = cms.untracked.vstring(),
                            # skipEvents = cms.untracked.uint32(500)
                            )


# process.out = cms.OutputModule("PoolOutputModule",
#     fileName = cms.untracked.string(options.outputFile),
#     outputCommands = cms.untracked.vstring(
#     	"keep *",
#     	"keep *_producer_*_*",
#     	"keep *_VertexProducer_*_*"
#     	)
# )


process.Timing = cms.Service("Timing", summaryOnly = cms.untracked.bool(True))


#--- Load config fragment that configures vertex producer
process.load('L1Trigger.VertexFinder.VertexProducer_cff')
process.VertexProducer.l1TracksInputTag = l1TracksTag

process.load('L1Trigger.L1TTrackMatch.L1TkPrimaryVertexProducer_cfi')
process.L1TkFastVertexProducer.L1TrackInputTag = l1TracksTag

process.load('L1Trigger.L1TTrackMatch.L1TrackdfEtMissProducer_cfi')


process.p = cms.Path(process.VertexProducer + process.L1TkFastVertexProducer)



process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( options.histFile ),
    fastCloning = cms.untracked.bool( False ),
    outputCommands = cms.untracked.vstring( 'drop *')
)
process.Out.outputCommands.append('keep *_L1TkPrimaryVertex_*_*')

process.e = cms.EndPath(process.out)
