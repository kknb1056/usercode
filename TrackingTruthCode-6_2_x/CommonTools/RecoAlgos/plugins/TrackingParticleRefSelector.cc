/** \class reco::modules::TrackingParticleSelector
 *
 *  Filter to select TrackingParticles according to pt, rapidity, tip, lip, number of hits, pdgId
 *
 *  \author Ian Tomalin, RAL
 *
 *  $Date: 2013/04/30 14:45:13 $
 *  $Revision: 1.1.2.1 $
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/RecoAlgos/interface/TrackingParticleSelector.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"

namespace reco {
  namespace modules {
    typedef SingleObjectSelector<TrackingParticleCollection,::TrackingParticleSelector,TrackingParticleRefVector> 
    TrackingParticleRefSelector ;

    DEFINE_FWK_MODULE( TrackingParticleRefSelector );
  }
}
