/** \class reco::modules::TrackingParticleSelector
 *
 *  Filter to select TrackingParticles according to pt, rapidity, tip, lip, number of hits, pdgId
 *
 *  \author Giuseppe Cerati, INFN
 *
 *  $Date: 2013/04/30 14:45:13 $
 *  $Revision: 1.1.2.1 $
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/RecoAlgos/interface/TrackingParticleSelector.h"

namespace reco {
  namespace modules {
    typedef SingleObjectSelector<TrackingParticleCollection,::TrackingParticleSelector> 
    TrackingParticleSelector ;

    DEFINE_FWK_MODULE( TrackingParticleSelector );
  }
}
