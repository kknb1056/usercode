/** \class ChargedCandidateProducer
 *
 * Framework module that produces a collection
 * of candidates with a Track Component
 *
 * \author Steven Lowette
 *
 * $Id: ChargedCandidateProducer.cc,v 1.1.2.1 2013/04/30 14:44:19 grimes Exp $
 *
 */

#include "CommonTools/RecoAlgos/src/TrackToCandidate.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef CandidateProducer<
          edm::View<reco::Track>,
          reco::RecoChargedCandidateCollection,
          AnySelector,
          converter::helper::CandConverter<reco::Track>::type
        > ChargedCandidateProducer;

DEFINE_FWK_MODULE(ChargedCandidateProducer);
