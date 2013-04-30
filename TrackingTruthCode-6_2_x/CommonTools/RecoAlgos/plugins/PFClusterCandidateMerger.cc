/** \class PFClusterRefCandidateMerger
 *
 * Framework module that produces a merged collection
 * of candidates with a PFCluster compoment
 *
 * \author Hartmut Stadie
 *
 * $Id: PFClusterCandidateMerger.cc,v 1.1.2.1 2013/04/30 14:44:53 grimes Exp $
 *
 */

#include "CommonTools/RecoAlgos/src/PFClusterToRefCandidate.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/Merger.h"

typedef Merger<reco::RecoPFClusterRefCandidateCollection>
        PFClusterRefCandidateMerger;

DEFINE_FWK_MODULE(PFClusterRefCandidateMerger);
