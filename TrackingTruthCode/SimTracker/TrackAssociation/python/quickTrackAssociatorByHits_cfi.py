import FWCore.ParameterSet.Config as cms

quickTrackAssociatorByHits = cms.ESProducer("QuickTrackAssociatorByHitsESProducer",
	AbsoluteNumberOfHits = cms.bool(False),
	Cut_RecoToSim = cms.double(0.75),
	SimToRecoDenominator = cms.string('sim'), # either "sim" or "reco"
	Quality_SimToReco = cms.double(0.5),
	Purity_SimToReco = cms.double(0.75),
	ThreeHitTracksAreSpecial = cms.bool(True),
	associatePixel = cms.bool(True),
	associateStrip = cms.bool(True),
	requireStoredHits = cms.bool(True), # True retains the old behaviour, False allows TrackingParticles without stored PSimHits
    ComponentName = cms.string('quickTrackAssociatorByHits')
)
