import FWCore.ParameterSet.Config as cms

TTTracksFromTracklet = cms.EDProducer("L1TrackProducer",
                                      SimTrackSource = cms.InputTag("g4SimHits"),
                                      SimVertexSource = cms.InputTag("g4SimHits"),
                                      TTStubSource = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"),
                                      readMoreMcTruth = cms.bool(True),
                                      MCTruthClusterInputTag = cms.InputTag("TTClusterAssociatorFromPixelDigis", "ClusterAccepted"),
                                      MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
                                      TrackingParticleInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                      TrackingVertexInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                      BeamSpotSource = cms.InputTag("offlineBeamSpot"),
                                      asciiFileName = cms.untracked.string(""),
                                      failscenario = cms.untracked.int32(0),
                                      trackerGeometryType  = cms.untracked.string("")
                                        #tilted barrel is assumed, use "flat" if running on flat
    )

TTTracksFromTrackletEmulation = cms.EDProducer("L1FPGATrackProducer",
                                               # general L1 tracking inputs
                                               SimTrackSource = cms.InputTag("g4SimHits"),
                                               SimVertexSource = cms.InputTag("g4SimHits"),
                                               TTStubSource = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"),
                                               readMoreMcTruth = cms.bool(True),
                                               MCTruthClusterInputTag = cms.InputTag("TTClusterAssociatorFromPixelDigis", "ClusterAccepted"),
                                               MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
                                               TrackingParticleInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                               TrackingVertexInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                               BeamSpotSource = cms.InputTag("offlineBeamSpot"),
                                               asciiFileName = cms.untracked.string(""),
                                               failscenario = cms.untracked.int32(0),
                                               trackerGeometryType  = cms.untracked.string(""),  #tilted barrel is assumed, use "flat" if running on flat
                                               # specific emulation inputs 
                                               # (if running on CRAB use "../../fitpattern.txt" etc instead)
                                               Quality =cms.bool(False),
                                               Extended=cms.untracked.bool(False),
                                               Hnpar=cms.untracked.uint32(4),
                                               fitPatternFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/fitpattern.txt'),
                                               memoryModulesFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/memorymodules_hourglass.dat'),
                                               processingModulesFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/processingmodules_hourglass.dat'),
                                               wiresFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/wires_hourglass.dat'),
                                               DTCLinkFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/calcNumDTCLinks.txt'),
                                               DTCLinkLayerDiskFile = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/dtclinklayerdisk.dat'),
                                               moduleCablingFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/modules_T5v3_27SP_nonant_tracklet.dat')
    )

TTTracksFromExtendedTrackletEmulation = cms.EDProducer("L1FPGATrackProducer",
                                               # general L1 tracking inputs
                                               SimTrackSource = cms.InputTag("g4SimHits"),
                                               SimVertexSource = cms.InputTag("g4SimHits"),
                                               TTStubSource = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"),
                                               readMoreMcTruth = cms.bool(True),
                                               MCTruthClusterInputTag = cms.InputTag("TTClusterAssociatorFromPixelDigis", "ClusterAccepted"),
                                               MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
                                               TrackingParticleInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                               TrackingVertexInputTag = cms.InputTag("mix", "MergedTrackTruth"),
                                               BeamSpotSource = cms.InputTag("offlineBeamSpot"),
                                               asciiFileName = cms.untracked.string(""),
                                               failscenario = cms.untracked.int32(0),
                                               trackerGeometryType  = cms.untracked.string(""),  #tilted barrel is assumed, use "flat" if running on flat
                                               # specific emulation inputs 
                                               # (if running on CRAB use "../../fitpattern.txt" etc instead)
                                               Quality =cms.bool(False),
                                               Extended=cms.untracked.bool(True),
                                               Hnpar=cms.untracked.uint32(5),
                                               fitPatternFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/fitpattern.txt'),
                                               memoryModulesFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/memorymodules_hourglassExtended.dat'),
                                               processingModulesFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/processingmodules_hourglassExtended.dat'),
                                               wiresFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/wires_hourglassExtended.dat'),
                                               DTCLinkFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/calcNumDTCLinks.txt'),
                                               DTCLinkLayerDiskFile = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/dtclinklayerdisk.dat'),
                                               moduleCablingFile  = cms.FileInPath('L1Trigger/TrackFindingTracklet/data/modules_T5v3_27SP_nonant_tracklet.dat')
    )

TTTracksFromTrackletEmulationWithQuality = TTTracksFromTrackletEmulation.clone(
                                                Quality =cms.bool(True),
                                                Quality_Algorithm = cms.string("NN"), #None, Cut, NN, GBDT

                                                ONNXmodel = cms.string("L1Trigger/TrackFindingTracklet/ML_data/FakeIDNN/NN_model.onnx"),
                                                ONNXInputName = cms.string("input_1"),
                                                ONNXOutputName = cms.string("Sigmoid_Output_Layer"),

                                                in_features = cms.vstring(["log_chi2","log_bendchi2","log_chi2rphi","log_chi2rz",
                                                                            "nstubs","lay1_hits","lay2_hits","lay3_hits","lay4_hits",
                                                                            "lay5_hits","lay6_hits","disk1_hits","disk2_hits",
                                                                            "disk3_hits","disk4_hits","disk5_hits","rinv","tanl",
                                                                            "z0","dtot","ltot"]),

                                                maxZ0 = cms.double ( 15. ) ,    # in cm
                                                maxEta = cms.double ( 2.4 ) ,
                                                chi2dofMax = cms.double( 40. ),
                                                bendchi2Max = cms.double( 2.4 ),
                                                minPt = cms.double( 2. ),       # in GeV
                                                nStubsmin = cms.int32( 4 )
    )

