/** \class reco::RecoTrackSelector
 *
 * Filter to select tracks according to pt, rapidity, tip, lip, number of hits, chi2
 *
 * \author Giuseppe Cerati, INFN
 *
 *  $Date: 2013/04/30 14:45:03 $
 *  $Revision: 1.1.2.1 $
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/RecoAlgos/interface/TrackSelector.h"
#include "CommonTools/RecoAlgos/interface/RecoTrackSelector.h"

namespace reco {
  typedef ObjectSelector<RecoTrackSelector> RecoTrackSelector;
  DEFINE_FWK_MODULE(RecoTrackSelector);
}
