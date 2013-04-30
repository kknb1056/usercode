/** \class reco::RecoTrackRefSelector
 *
 * Filter to select tracks according to pt, rapidity, tip, lip, number of hits, chi2
 *
 * \author Giuseppe Cerati, INFN
 * \author Ian Tomalin, RAL
 *
 *  $Date: 2013/04/30 14:45:02 $
 *  $Revision: 1.1.2.1 $
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
//#include "CommonTools/RecoAlgos/interface/TrackSelector.h"
#include "CommonTools/RecoAlgos/interface/RecoTrackRefSelector.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

namespace reco {
  typedef ObjectSelector<RecoTrackRefSelector,reco::TrackRefVector> RecoTrackRefSelector;
  DEFINE_FWK_MODULE(RecoTrackRefSelector);
}
