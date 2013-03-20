#ifndef SimDataFormats_TrackingAnalysis_TrackingVertexContainer_h
#define SimDataFormats_TrackingAnalysis_TrackingVertexContainer_h

#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/RefProd.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"

typedef std::vector<TrackingVertex>                TrackingVertexCollection;
typedef edm::Ref<TrackingVertexCollection>         TrackingVertexRef;
typedef edm::RefVector<TrackingVertexCollection>   TrackingVertexContainer;
typedef edm::RefVector<TrackingVertexCollection>   TrackingVertexRefVector;
typedef edm::RefProd<TrackingVertexCollection>     TrackingVertexRefProd;
typedef TrackingVertexRefVector::iterator   tv_iterator;


#endif
