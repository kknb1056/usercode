import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Validation_cff import *
from Validation.RecoHI.hiBasicGenTest_cfi import *
from Validation.RecoHI.globalValidationHeavyIons_cff import *
from Validation.RecoHI.HLTValidationHeavyIons_cff import *

validationHI = cms.Sequence(hiBasicGenTest
                            *globaldigisanalyze
                            *globalhitsanalyze
#                            *globalrechitsanalyze # N.B. this has been taken out because rec hits are no longer put in crossing frames
                            *globalValidationHI
                            *hltValidationHI
                            )

# temporary removal
# due to massive redundant output
validationHI.remove(condDataValidation)
