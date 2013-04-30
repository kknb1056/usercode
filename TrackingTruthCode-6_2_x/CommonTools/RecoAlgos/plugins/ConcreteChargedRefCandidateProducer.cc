/** \class ConcreteChargedCandidateProducer
 *
 * Framework module that produces a collection
 * of candidates with a Track compoment
 *
 * \author Luca Lista, INFN
 *
 * \version $Revision: 1.1.2.1 $
 *
 * $Id: ConcreteChargedRefCandidateProducer.cc,v 1.1.2.1 2013/04/30 14:44:22 grimes Exp $
 *
 */

#include "CommonTools/RecoAlgos/src/TrackToRefCandidate.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef CandidateProducer<
          reco::TrackCollection, 
          reco::RecoChargedRefCandidateCollection
        > ConcreteChargedRefCandidateProducer;

DEFINE_FWK_MODULE(ConcreteChargedRefCandidateProducer);
