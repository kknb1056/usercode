#ifndef Tracker_SiInduceChargeOnStrips_H
#define Tracker_SiInduceChargeOnStrips_H

#include "SimTracker/SiStripDigitizer/interface/SiChargeCollectionDrifter.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "SimTracker/SiStripDigitizer/interface/SiPileUpSignals.h"

#include<map>

class StripDet;
/**
 * Base class for the induction of signal on strips.
 * Given a SignalPoint, computes the charge on each strip.
 */

class SiInduceChargeOnStrips{
public:
  
  virtual ~SiInduceChargeOnStrips() { }
  virtual void induce(const SiChargeCollectionDrifter::collection_type&, const StripGeomDetUnit&, 
		      std::vector<double>&, size_t&, size_t&) const = 0 ;
};
#endif
