import FWCore.ParameterSet.Config as cms


#RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#        mix = cms.PSet(initialSeed = cms.untracked.uint32(12345),
#                       engineName = cms.untracked.string('HepJamesRandom')
#        ),
#        restoreStateLabel = cms.untracked.string("randomEngineStateProducer"),
#)

from Validation.GlobalDigis.globaldigis_analyze_cfi import *
from Validation.GlobalRecHits.globalrechits_analyze_cfi import *
from Validation.GlobalHits.globalhits_analyze_cfi import *
from Validation.Configuration.globalValidation_cff import *

from HLTriggerOffline.Common.HLTValidation_cff import *


from Validation.RecoMET.METRelValForDQM_cff import *
from Validation.TrackingMCTruth.trackingTruthValidation_cfi import *
from Validation.RecoTrack.TrackValidation_cff import *
from Validation.RecoMuon.muonValidation_cff import *
from Validation.MuonIsolation.MuIsoVal_cff import *
from Validation.MuonIdentification.muonIdVal_cff import *
from Validation.RecoMuon.muonValidationHLT_cff import *
from Validation.EventGenerator.BasicGenValidation_cff import *

prevalidation = cms.Sequence( globalPrevalidation * hltassociation )

validation = cms.Sequence(cms.SequencePlaceholder("mix")
                         +genvalid_all
                         *globaldigisanalyze
                         *globalhitsanalyze
#                         *globalrechitsanalyze # N.B. this has been taken out because rec hits are no longer put in crossing frames
                         *globalValidation
                         *hltvalidation)

prevalidation_preprod = cms.Sequence( preprodPrevalidation )

validation_preprod = cms.Sequence(
                          genvalid_all
                          +trackingTruthValid
                          +tracksValidation
                          +METRelValSequence
                          +recoMuonValidation
                          +muIsoVal_seq
                          +muonIdValDQMSeq
                          +hltvalidation_preprod
                          )

validation.remove(condDataValidation)
validation_prod = cms.Sequence(
             genvalid_all
            +hltvalidation_prod
            )

