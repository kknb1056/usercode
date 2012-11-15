# Auto generated configuration file
# using: 
# Revision: 1.392 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: valTest --datatier GEN-SIM-RAW --conditions auto:startup -s DIGI,L1,DIGI2RAW,HLT,RAW2DIGI,RECO,VALIDATION --eventcontent FEVTDEBUG -n -1 --filein file:step1-ZEE.root --fileout file:ZEE_slimValidation.root --pileup LowLumiPileUp --pileup_input file:MinBias_60X.root --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixLowLumPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:step1-ZEE.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    annotation = cms.untracked.string('valTest nevts:5'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.FEVTDEBUGoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('file:ZEE_slimValidation.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RAW')
    )
)

# Additional output definition

# Other statements
process.mix.input.fileNames = cms.untracked.vstring(['file:MinBias_60X.root'])
process.GlobalTag.globaltag = 'START60_V4::All'

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.prevalidation_step = cms.Path(process.prevalidation)
process.validation_step = cms.EndPath(process.validation)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGoutput_step = cms.EndPath(process.FEVTDEBUGoutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.reconstruction_step,process.prevalidation_step,process.validation_step,process.endjob_step,process.FEVTDEBUGoutput_step])

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions

###################################################################################
##
## Everything above is the output from cmsDriver. All of my changes are below.
##
###################################################################################


#process.mix.mixObjects.mixTracks.makeCrossingFrame = False
#process.mix.mixObjects.mixVertices.makeCrossingFrame = False
#process.mix.mixObjects.mixHepMC.makeCrossingFrame = False
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsPixelBarrelHighTof")
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsPixelBarrelLowTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsPixelEndcapHighTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsPixelEndcapLowTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsTECHighTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsTECLowTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsTIBHighTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsTIBLowTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsTIDHighTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsTIDLowTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsTOBHighTof") 
#process.mix.mixObjects.mixSH.crossingFrames.remove("TrackerHitsTOBLowTof")

#process.mergedtruth.mergedBremsstrahlung = False
# Need to get rid of the old TrackingTruthProducer because the python
# configs haven't been updated yet.
#process.digitisation_step.remove(process.mergedtruth)
#del(process.mergedtruth)


# Also need to get rid of the old StripDigiSimLink producer
process.digitisation_step.remove(process.simSiStripDigiSimLink)
del(process.simSiStripDigiSimLink)

# Also redefine simSiStripDigis so StripDigiSimLinkedmDetSetVector is under "mix"
# and not "simSiStripDigiSimLink".
process.simSiStripDigis = cms.EDAlias(
	mix = cms.VPSet(
		cms.PSet(
			type = cms.string('SiStripDigiedmDetSetVector')
		), 
		cms.PSet(
			type = cms.string('SiStripRawDigiedmDetSetVector')
		),
		cms.PSet(
			type = cms.string('StripDigiSimLinkedmDetSetVector')
		)
	)
)


# Now define it as a component in the mixing module
process.mix.digitizers.mergedtruth = cms.PSet(
	accumulatorType = cms.string('TrackingTruthAccumulator'),
	createUnmergedCollection = cms.bool(True),
	createMergedBremsstrahlung = cms.bool(True),
	alwaysAddAncestors = cms.bool(True),
	maximumBunchCrossing = cms.uint32(9999),
	copySimHits = cms.bool(True),
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
	removeDeadModules = cms.bool(False),
	HepMCDataLabels = cms.vstring('VtxSmeared',
		'generator',
		'PythiaSource',
		'source'),
	useMultipleHepMCLabels = cms.bool(False),
	volumeRadius = cms.double(100.0),
	volumeZ = cms.double(900.0),
	discardOutVolume = cms.bool(True)
)
# Set an alias so that the products have the correct name
process.mergedtruthNew = cms.EDAlias(
	mix = cms.VPSet(
		cms.PSet(type = cms.string('TrackingParticles')),
		cms.PSet(type = cms.string('TrackingVertexs'))
	)
)

process.load("SimTracker.TrackAssociation.quickTrackAssociatorByHits_cfi")
process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')
process.trackValidator.associators = cms.vstring('quickTrackAssociatorByHits')
# Trim off all the other track collections from validation to save time
process.trackValidator.label = cms.VInputTag(cms.InputTag("generalTracks"))

process.trackValidatorUnmerged = process.trackValidator.clone(
	label_tp_effic=cms.InputTag("mergedtruth"),
	label_tp_fake=cms.InputTag("mergedtruth"),
	label_tv = cms.InputTag("mergedtruth"),
	dirName = cms.string('Tracking/TrackUnmerged/')
)

process.trackValidatorNew = process.trackValidator.clone(
	label_tp_effic=cms.InputTag("mergedtruthNew","MergedTrackTruth"),
	label_tp_fake=cms.InputTag("mergedtruthNew","MergedTrackTruth"),
	label_tv = cms.InputTag("mergedtruthNew","MergedTrackTruth"),
	dirName = cms.string('Tracking/TrackNew/')
)

process.trackValidatorNewUnmerged = process.trackValidator.clone(
	label_tp_effic=cms.InputTag("mergedtruthNew"),
	label_tp_fake=cms.InputTag("mergedtruthNew"),
	label_tv = cms.InputTag("mergedtruthNew"),
	dirName = cms.string('Tracking/TrackNewUnmerged/')
)

# Remove these just to make sure I'm doing the same for the old and new collections
process.prevalidation_step.remove( process.tpSelection )
process.prevalidation_step.remove( process.tpSelecForFakeRate )
process.prevalidation_step.remove( process.tpSelecForEfficiency )
process.trackingTruthValid.src = cms.InputTag("mergedtruth","MergedTrackTruth")

process.FEVTDEBUGoutput.outputCommands = cms.untracked.vstring( "drop *", "keep *_MEtoEDMConverter_*_*" )

# Change the validation sequence to only do the tracking validation, and
# cut out all the other stuff I'm not interested in. Also add track validation
# for the other collections.
process.validation = cms.Sequence(
	process.trackingTruthValid
	+process.trackValidator
	+process.trackValidatorUnmerged
	+process.trackValidatorNew
	+process.trackValidatorNewUnmerged
)

## Add a moderate amount of pileup for testing
process.mix.input.nbPileupEvents = cms.PSet( averageNumber = cms.double(20.0) )
## Uncomment these line for simpler pileup for testing
#process.mix.input.type = cms.string('fixed')
#process.mix.maxBunch = cms.int32(0)
#process.mix.minBunch = cms.int32(0)

