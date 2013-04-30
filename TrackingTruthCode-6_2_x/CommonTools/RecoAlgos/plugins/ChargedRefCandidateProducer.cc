/** \class ChargedRefCandidateProducer
 *
 * Framework module that produces a collection
 * of candidates with a Track compoment
 *
 * \author Steven Lowette
 *
 * $Id: ChargedRefCandidateProducer.cc,v 1.1.2.1 2013/04/30 14:44:19 grimes Exp $
 *
 */

#include "CommonTools/RecoAlgos/src/TrackToRefCandidate.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef CandidateProducer<
          edm::View<reco::Track>,
          reco::RecoChargedRefCandidateCollection,
          AnySelector,
          converter::helper::CandConverter<reco::Track>::type
        > ChargedRefCandidateProducer;

DEFINE_FWK_MODULE(ChargedRefCandidateProducer);
