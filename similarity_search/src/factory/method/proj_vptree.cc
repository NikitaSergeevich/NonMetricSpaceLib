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
#include "proj_vptree.h"
#include "methodfactory.h"

#include "space_sparse_scalar.h"

namespace similarity {

/*
 * Creating functions.
 */

template <typename dist_t>
Index<dist_t>* CreateProjVPTree(bool PrintProgress,
                           const string& SpaceType,
                           const Space<dist_t>* space,
                           const ObjectVector& DataObjects,
                           const AnyParams& AllParams) {

    if (SpaceType != SPACE_SPARSE_ANGULAR_DISTANCE &&
        SpaceType != SPACE_SPARSE_COSINE_SIMILARITY) LOG(FATAL) << METH_PROJ_VPTREE << 
        " works only with " << SPACE_SPARSE_ANGULAR_DISTANCE << 
        " or with " << SPACE_SPARSE_COSINE_SIMILARITY;
      ;

    return new ProjectionVPTree<dist_t>(space, DataObjects, AllParams);
}

/*
 * End of creating functions.
 */

/*
 * Let's register creating functions in a method factory.
 *
 * IMPORTANT NOTE: don't include this source-file into a library.
 * Sometimes C++ carries out a lazy initialization of global objects
 * that are stored in a library. Then, the registration code doesn't work.
 */

REGISTER_METHOD_CREATOR(float,  METH_PROJ_VPTREE, CreateProjVPTree)
REGISTER_METHOD_CREATOR(double, METH_PROJ_VPTREE, CreateProjVPTree)


}

