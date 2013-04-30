/** \class PFClusterRefCandidateProducer
 *
 * Framework module that produces a collection
 * of candidates with a Track compoment
 *
 * \author Steven Lowette
 *
 * $Id: PFClusterCandidateProducer.cc,v 1.1.2.1 2013/04/30 14:44:53 grimes Exp $
 *
 */

#include "CommonTools/RecoAlgos/src/PFClusterToRefCandidate.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef CandidateProducer<
          edm::View<reco::PFCluster>,
          reco::RecoPFClusterRefCandidateCollection,
          AnySelector,
          converter::helper::CandConverter<reco::PFCluster>::type
        > PFClusterRefCandidateProducer;

DEFINE_FWK_MODULE(PFClusterRefCandidateProducer);
