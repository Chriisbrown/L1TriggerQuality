import FWCore.ParameterSet.Config as cms

from RecoVertex.BeamSpotProducer.BeamSpot_cfi import *

from L1Trigger.TrackFindingTracklet.Tracklet_cfi import *

from SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff import *
TTTrackAssociatorFromPixelDigis.TTTracks = cms.VInputTag(cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks") )

# quality emulation
TTTrackAssociatorFromPixelDigisWithQuality = TTTrackAssociatorFromPixelDigis.clone(
    TTTracks = cms.VInputTag(cms.InputTag("TTTracksFromTrackletEmulationWithQuality", "Level1TTTracks") )
)

L1HybridTracksWithQuality = cms.Sequence(offlineBeamSpot*TTTracksFromTrackletEmulationWithQuality)
L1HybridTracksWithAssociatorsWithQuality = cms.Sequence(offlineBeamSpot*TTTracksFromTrackletEmulationWithQuality*TTTrackAssociatorFromPixelDigisWithQuality)


