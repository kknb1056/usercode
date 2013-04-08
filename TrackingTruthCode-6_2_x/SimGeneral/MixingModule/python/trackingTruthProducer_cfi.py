import FWCore.ParameterSet.Config as cms

mergedtruth = cms.PSet(
	accumulatorType = cms.string('TrackingTruthAccumulator'),
	createUnmergedCollection = cms.bool(True),
	createMergedBremsstrahlung = cms.bool(True),
	alwaysAddAncestors = cms.bool(True),
	maximumPreviousBunchCrossing = cms.uint32(9999),
	maximumSubsequentBunchCrossing = cms.uint32(9999),
	vertexDistanceCut = cms.double(0.003),
	simHitCollections = cms.PSet(
		muon = cms.vstring('MuonDTHits',
			'MuonCSCHits',
			'MuonRPCHits'),
		tracker = cms.vstring('TrackerHitsTIBLowTof',
			'TrackerHitsTIBHighTof',
			'TrackerHitsTIDLowTof',
			'TrackerHitsTIDHighTof',
			'TrackerHitsTOBLowTof',
			'TrackerHitsTOBHighTof',
			'TrackerHitsTECLowTof',
			'TrackerHitsTECHighTof'),
		pixel = cms.vstring('TrackerHitsPixelBarrelLowTof',
        	'TrackerHitsPixelBarrelHighTof',
        	'TrackerHitsPixelEndcapLowTof',
        	'TrackerHitsPixelEndcapHighTof')
	),
	simHitLabel = cms.string('g4SimHits'),
	genParticleCollection = cms.InputTag('genParticles'),
	copySimHits = cms.bool(True),
	removeDeadModules = cms.bool(False), # currently not implemented
	useMultipleHepMCLabels = cms.bool(False),
	volumeRadius = cms.double(1200.0),
	volumeZ = cms.double(3000.0),
	ignoreTracksOutsideVolume = cms.bool(False)
)
