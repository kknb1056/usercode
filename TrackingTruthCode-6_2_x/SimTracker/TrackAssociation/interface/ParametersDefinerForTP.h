#ifndef TrackAssociation_ParametersDefinerForTP_h
#define TrackAssociation_ParametersDefinerForTP_h

/**
 *
 *
 * \author Boris Mangano (UCSD)  5/7/2009
 */

#include <SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"      
#include "FWCore/Framework/interface/EventSetup.h"

class ParametersDefinerForTP {

 public:
  ParametersDefinerForTP(){};
  virtual ~ParametersDefinerForTP() {};

  virtual TrackingParticle::Vector momentum(const edm::Event& iEvent, const edm::EventSetup& iSetup, const TrackingParticle& tp) const;
  virtual TrackingParticle::Point vertex(const edm::Event& iEvent, const edm::EventSetup& iSetup, const TrackingParticle& tp) const;

  virtual TrackingParticle::Vector momentum(const edm::Event& iEvent, const edm::EventSetup& iSetup, const reco::Candidate& tp) const {
#warning "This file has been modified just to get it to compile without any regard as to whether it still functions as intended"
#ifdef REMOVED_JUST_TO_GET_IT_TO_COMPILE__THIS_CODE_NEEDS_TO_BE_CHECKED
    return momentum(iEvent, iSetup, ParticleBase(tp.charge(),tp.p4(),tp.vertex()));
#else
    return TrackingParticle::Vector();
#endif
  }
  virtual TrackingParticle::Point vertex(const edm::Event& iEvent, const edm::EventSetup& iSetup, const reco::Candidate& tp) const {
#warning "This file has been modified just to get it to compile without any regard as to whether it still functions as intended"
#ifdef REMOVED_JUST_TO_GET_IT_TO_COMPILE__THIS_CODE_NEEDS_TO_BE_CHECKED
    return vertex(iEvent, iSetup, ParticleBase(tp.charge(),tp.p4(),tp.vertex()));
#else
    return TrackingParticle::Point();
#endif
  }

};


#endif
