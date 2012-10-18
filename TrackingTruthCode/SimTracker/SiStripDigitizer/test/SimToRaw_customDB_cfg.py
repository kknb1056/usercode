import FWCore.ParameterSet.Config as cms

process = cms.Process("MYRECO")

process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.L1Emulator_cff")
process.load("Configuration.StandardSequences.DigiToRaw_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_37Y_V0::All'


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
   moduleSeeds = cms.PSet(
        simMuonRPCDigis = cms.untracked.uint32(6),
        simEcalUnsuppressedDigis = cms.untracked.uint32(8),
        simSiStripDigis = cms.untracked.uint32(7),
        simSiStripDigisOrig = cms.untracked.uint32(7),
        mix = cms.untracked.uint32(4),
        simHcalUnsuppressedDigis = cms.untracked.uint32(9),
        simMuonCSCDigis = cms.untracked.uint32(6),
        VtxSmeared = cms.untracked.uint32(2),
        g4SimHits = cms.untracked.uint32(3),
        simMuonDTDigis = cms.untracked.uint32(6),
        simSiPixelDigis = cms.untracked.uint32(7)
    ),
    sourceSeed = cms.untracked.uint32(1) 
)


process.load("FWCore.MessageLogger.MessageLogger_cfi") 


process.source = cms.Source("PoolSource",

   fileNames = cms.untracked.vstring('/store/relval/CMSSW_3_7_0_pre1/RelValSingleMuPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_37Y_V0-v1/0015/D4146A66-0E4D-DF11-8E10-002618943984.root')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

### new payloads:
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.SiStripNoises = cms.ESSource("PoolDBESSource",process.CondDBSetup,
                             connect = cms.string("sqlite_file:dbfile_noise.db"),
                             toGet = cms.VPSet(cms.PSet(record = cms.string("SiStripNoisesRcd"),
                                                        tag = cms.string("SiStripNoiseNormalizedWithGain"))))
process.es_prefer_SiStripNoises = cms.ESPrefer("PoolDBESSource","SiStripNoises")

process.SiStripGain = cms.ESSource("PoolDBESSource",process.CondDBSetup,
                           connect = cms.string('sqlite_file:dbfile_gainFromData.db'),
                           toGet = cms.VPSet(cms.PSet( record = cms.string('SiStripApvGainRcd'),
                                                       tag = cms.string('SiStripApvGain_default'))))
process.es_prefer_SiStripGain = cms.ESPrefer("PoolDBESSource", "SiStripGain")

###

process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
    #'keep *_*_*_RECO'
    'keep *_rawDataCollector_*_*',
    'keep SiStripDigi*_*_*_*'
    ),
    fileName = cms.untracked.string('SimRawDigi_RelValMuPt10_OnlyTRK_customDB.root')
)


#Parameters for Simulation Digitizer
process.simSiStripDigis.ZeroSuppression = cms.bool(True)
process.simSiStripDigis.Noise = cms.bool(True)
process.simSiStripDigis.TrackerConfigurationFromDB = cms.bool(False)


process.SiStripDigiToRaw.InputDigiLabel = cms.string('ZeroSuppressed')
process.SiStripDigiToRaw.FedReadoutMode = cms.string('ZERO_SUPPRESSED')


process.p = cms.Path(process.mix*process.simSiStripDigis)

process.outpath = cms.EndPath(process.output)





