/**
 * Non-metric Space Library
 *
 * Authors: Bilegsaikhan Naidan (https://github.com/bileg), Leonid Boytsov (http://boytsov.info).
 * With contributions from Lawrence Cayton (http://lcayton.com/).
 *
 * For the complete list of contributors and further details see:
 * https://github.com/searchivarius/NonMetricSpaceLib 
 * 
 * Copyright (c) 2014
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */

#include "searchoracle.h"
#include "space_sparse_scalar.h"
#include "spacefactory.h"

namespace similarity {

/*
 * Creating functions.
 */

template <typename dist_t>
Space<dist_t>* CreateSparseCosineSimilarity(const AnyParams& /* ignoring params */) {
  // Cosine Similarity
  return new SpaceSparseCosineSimilarity<dist_t>();
}


template <typename dist_t>
Space<dist_t>* CreateSparseAngularDistance(const AnyParams& /* ignoring params */) {
  // Cosine Similarity
  return new SpaceSparseAngularDistance<dist_t>();
}

/*
 * End of creating functions.
 */

/*
 * Let's register creating functions in a space factory.
 *
 * IMPORTANT NOTE: don't include this source-file into a library.
 * Sometimes C++ carries out a lazy initialization of global objects
 * that are stored in a library. Then, the registration code doesn't work.
 */

REGISTER_SPACE_CREATOR(float,  SPACE_SPARSE_COSINE_SIMILARITY, CreateSparseCosineSimilarity)
REGISTER_SPACE_CREATOR(double, SPACE_SPARSE_COSINE_SIMILARITY, CreateSparseCosineSimilarity)
REGISTER_SPACE_CREATOR(float,  SPACE_SPARSE_ANGULAR_DISTANCE, CreateSparseAngularDistance)
REGISTER_SPACE_CREATOR(double, SPACE_SPARSE_ANGULAR_DISTANCE, CreateSparseAngularDistance)

}

