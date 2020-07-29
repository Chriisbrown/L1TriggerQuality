import FWCore.ParameterSet.Config as cms

L1TrackerEtMiss = cms.EDProducer('L1TrackerEtMissProducer',
     L1TrackInputTag = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),
     L1VertexInputTag = cms.InputTag("VertexProducer", "l1vertextdr"),
     L1METTag = cms.string("TrackMET"),

     maxZ0 = cms.double ( 15. ) ,    # in cm
     maxEta = cms.double ( 2.4 ) ,
     chi2dofMax = cms.double( 40. ),
     bendchi2Max = cms.double( 2.4 ),
     minPt = cms.double( 2. ),       # in GeV
     DeltaZ = cms.double( 3. ),      # in cm
     nStubsmin = cms.int32( 4 ),     # min number of stubs for the tracks to enter in TrkMET calculation
     nStubsPSmin = cms.int32( -1 ),   # min number of stubs in the PS Modules
     maxPt = cms.double( 200. ),	 # in GeV. When maxPt > 0, tracks with PT above maxPt are considered as
                                     # mismeasured and are treated according to HighPtTracks below.
                                     # When maxPt < 0, no special treatment is done for high PT tracks.
     HighPtTracks = cms.int32( 1 ),  # when = 0 : truncation. Tracks with PT above maxPt are ignored
                                     # when = 1 : saturation. Tracks with PT above maxPt are set to PT=maxPt.
                                     # When maxPt < 0, no special treatment is done for high PT tracks.
     Cut = cms.bool(False),
     MVACut = cms.bool(False),
     MVAThreshold = cms.double(0.5),
)


L1TrackerEtMissPurityCut = L1TrackerEtMiss.clone(
     
     Cut = cms.bool(True)
)

L1TrackerEtMissMVACut = L1TrackerEtMiss.clone(
     
     MVACut = cms.bool(True),
     MVAThreshold = cms.double(0.5),
)


L1TrackerGenEtMiss = cms.EDProducer('L1TrackerEtGenProducer',
     L1TrackingTrackInputTag = cms.InputTag("mix", "MergedTrackTruth"),
     L1TrackingVertexInputTag = cms.InputTag("mix", "MergedTrackTruth"),
     L1METTag = cms.string("GenMET"),
)