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
#ifndef EVAL_RESULTS_H
#define EVAL_RESULTS_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <memory>

#include "utils.h"
#include "space.h"
#include "object.h"
#include "index.h"
#include "knnqueue.h"

namespace similarity {

template <class dist_t>
class GoldStandard {
public:
  GoldStandard(const typename similarity::Space<dist_t>* space,
              const ObjectVector& datapoints, 
              const typename similarity::KNNQuery<dist_t>* query 
              ) {
    DoSeqSearch(space, datapoints, query->QueryObject());
  }
  GoldStandard(const typename similarity::Space<dist_t>* space,
              const ObjectVector& datapoints, 
              const typename similarity::RangeQuery<dist_t>* query 
              ) {
    DoSeqSearch(space, datapoints, query->QueryObject());
  }
  uint64_t GetSeqSearchTime()     const { return SeqSearchTime_; }
  uint64_t GetSeqSearchCPUTime()  const { return SeqSearchCPUTime_; }

  const DistObjectPairVector<dist_t>&   GetExactDists() const { return  ExactDists_;}
private:
  void DoSeqSearch(const similarity::Space<dist_t>* space,
                   const ObjectVector&              datapoints,
                   const Object*                    query) {
    WallClockTimer  wtm;
    CPUTimer        ctm;

    wtm.reset();
    ctm.reset();

    ExactDists_.resize(datapoints.size());

    for (size_t i = 0; i < datapoints.size(); ++i) {
      // Distance can be asymmetric, but the query is always on the right side
      ExactDists_[i] = std::make_pair(space->IndexTimeDistance(datapoints[i], query), datapoints[i]);
    }

    wtm.split();
    ctm.split();

    SeqSearchTime_ = wtm.elapsed();
    SeqSearchCPUTime_ = ctm.elapsed();

    std::sort(ExactDists_.begin(), ExactDists_.end());
  }

  uint64_t                            SeqSearchTime_;
  uint64_t                            SeqSearchCPUTime_;

  DistObjectPairVector<dist_t>        ExactDists_;
};

template <class dist_t>
class EvalResults {
public:
  EvalResults(const typename similarity::Space<dist_t>* space,
                   const typename similarity::KNNQuery<dist_t>* query,
                   const GoldStandard<dist_t>& gs) : K_(0), ExactDists_(gs.GetExactDists()) {
    GetKNNData(query);
    ComputeMetrics();
  }

  EvalResults(const typename similarity::Space<dist_t>* space,
                   const typename similarity::RangeQuery<dist_t>* query,
                   const GoldStandard<dist_t>& gs) : K_(0), ExactDists_(gs.GetExactDists()) {
    GetRangeData(query);
    ComputeMetrics();
  }

  /* 
   * For all metrics below, pos(i) denotes a position of the 
   * i-th approximate result in the exact list of neighbors.
   * That is, a k=pos(i) means that the i-th element returned
   * by a search method is, in truth, a k-th nearest neighbor.
   */

  /* 
   * GetNumCloser() makes most sense only for 1-NN search,
   * because it computes the number of points closer than
   * the nearest point found by a method.
   * 
   * Formally is it equal to pos(0) - 1.
   *
   * This metric was proposed in:
   * L. Cayton. Fast nearest neighbor retrieval for bregman divergences. 
   * Twenty-Fifth International Conference on Machine Learning (ICML), 2008.
   *
   */
  double GetNumCloser()       const { return NumberCloser_;}
   /*
    * An average logarithm of a relative position error.
    * Just exponentiate to get a geometric mean of relative position errors.
    *
    * Formally, it is equal to:
    * 1/K sum_{i=1}^K   log(pos(i))
    */
  double GetLogRelPos()       const { return LogRelPosError_;}
    /*
     * Just the classic recall value
     */
  double GetRecall()          const { return Recall_; }
   /*
    * Proposed in:
    * Zezula, P., Savino, P., Amato, G., Rabitti, F., 
    * Approximate similarity retrieval with m-trees. 
    * The VLDB Journal 7(4) (December 1998) 275-293
    *
    * Formally, the precision of approximation is equal to:
    * 1/K sum_{i=1}^K   i/pos(i)
    */
  double GetPrecisionOfApprox() const { return PrecisionOfApprox_; }
private:
  size_t K_;

  void GetKNNData(const KNNQuery<dist_t>* query) {
    K_ = query->GetK();
    for (size_t i = 0; i < ExactDists_.size(); ++i) {
      /* 
       * TODO (@leo) are there situations where we need to compare distances approximately?
       *             Probably not, if computation of distances is deterministic and exact,
       *             i.e., given the query Q and the object X, we always get the same 
       *             distance from Q to X, no matter which method returned X.
       */
#if 0
      if (i < K_ || (i && ApproxEqual(ExactDists_[i].first, ExactDists_[i-1].first))) {
#else
      if (i < K_ || (i && ExactDists_[i].first == ExactDists_[i-1].first)) {
#endif
        ExactResultSet_.insert(ExactDists_[i].second->id());
      }
      else break; // ExactDists are sorted by distance
    }

    unique_ptr<KNNQueue<dist_t>> ResQ(query->Result()->Clone());

    while (!ResQ->Empty()) {
      const Object* ResObject = reinterpret_cast<const Object*>(ResQ->TopObject());
      CHECK(ResObject);
      /*
       * A search method can potentially return duplicate records.
       * We simply ignore duplicates during evaluation.
       */
      if (ApproxResultSet_.find(ResObject->id()) == ApproxResultSet_.end()) {
        ApproxResultSet_.insert(ResObject->id());
        ApproxDists_.insert(ApproxDists_.begin(), ResQ->TopDistance());
      }
      ResQ->Pop();
    }
  }

  void GetRangeData(const RangeQuery<dist_t>* query) {
    for (size_t i = 0; i < ExactDists_.size(); ++i) {
      if (ExactDists_[i].first <= query->Radius()) ExactResultSet_.insert(ExactDists_[i].second->id());
      else break; // ExactDists are sorted by distance
    }

    const ObjectVector&         ResQ = *query->Result();
    const std::vector<dist_t>&  ResQDists = *query->ResultDists();

    CHECK(ResQ.size() == ResQDists.size());

    for (size_t i = 0; i < ResQ.size(); ++i) {
      const Object* ResObject = ResQ[i];
      CHECK(ResObject);
      // We should not have any duplicates!
      if (ApproxResultSet_.find(ResObject->id()) == ApproxResultSet_.end()) {
        ApproxResultSet_.insert(ResObject->id());
        ApproxDists_.insert(ApproxDists_.begin(), ResQDists[i]);
      }
    }

    std::sort(ApproxDists_.begin(), ApproxDists_.end());
  }

  void ComputeMetrics() {
    Recall_            = 0.0;
    NumberCloser_      = 0.0;
    LogRelPosError_    = 0.0;
    PrecisionOfApprox_ = 0.0;

    // 1. First let's do recall
    for (auto it = ApproxResultSet_.begin(); it != ApproxResultSet_.end(); ++it) {
      Recall_ += ExactResultSet_.count(*it);
    }

    size_t ExactResultSize = K_ ? K_:ExactResultSet_.size();

    if (ExactResultSet_.empty()) Recall_ = 1.0;
    /* 
     * In k-NN search the k-neighborhood may be definied ambiguously.
     * It can happen if several points are at the same distance from the query point.
     * All these points are included into the ExactResultSet_, yet, their number can be > K_.
     */
    else  Recall_ /= ExactResultSize;


    if (ApproxDists_.size()) { 
      // 2. Compute the number of points closer to the 1-NN then the first result.
      CHECK(!ApproxDists_.empty());
      for (size_t p = 0; p < ExactDists_.size(); ++p) {
        if (ExactDists_[p].first >= ApproxDists_[0]) break;
        ++NumberCloser_;
      }
      // 3. Compute the relative position error and the precision of approximation
      CHECK(ApproxDists_.size() <= ExactDists_.size());

      for (size_t k = 0, p = 0; k < ApproxDists_.size(); ++k) {
        if (ApproxDists_[k] -  ExactDists_[p].first < 0) {
          double mx = std::abs(std::max(ApproxDists_[k], ExactDists_[p].first));
          double mn = std::abs(std::min(ApproxDists_[k], ExactDists_[p].first));
  
          const double epsRel = 2e-5;
          const double epsAbs = 5e-4;
          /*
           * TODO: @leo These eps are quite adhoc.
           *            There can be a bug here (where approx is better than exact??), 
           *            to reproduce a situation when the below condition is triggered 
           *            for epsRel = 1e-5 use & epsAbs = 1-e5:
           *            release/experiment  --dataFile ~/TextCollect/VectorSpaces/colors112.txt --knn 1 --testSetQty 1 --maxNumQuery 1000  --method vptree:alphaLeft=0.8,alphaRight=0.8  -s cosinesimi 
           *
           */
          if (mx > 0 && (1- mn/mx) > epsRel && (mx - mn) > epsAbs) {
            for (size_t i = 0; i < std::min(ExactDists_.size(), ApproxDists_.size()); ++i ) {
              LOG(INFO) << "Ex: " << ExactDists_[i].first << 
                           " -> Apr: " << ApproxDists_[i] << 
                           " 1 - ratio: " << (1 - mn/mx) << " diff: " << (mx - mn);
            }
            LOG(FATAL) << "bug: the approximate query should not return objects "
                   << "that are closer to the query than object returned by "
                   << "(exact) sequential searching!"
                   << " Approx: " << ApproxDists_[k]
                   << " Exact: "  << ExactDists_[p].first;
          }
        }
        size_t LastEqualP = p;
        if (p < ExactDists_.size() && ApproxEqual(ExactDists_[p].first, ApproxDists_[k])) {
          ++p;
        } else {
          while (p < ExactDists_.size() && ExactDists_[p].first < ApproxDists_[k]) {
            ++p;
            ++LastEqualP;
          }
        }
        if (p < k) {
          for (size_t i = 0; i < std::min(ExactDists_.size(), ApproxDists_.size()); ++i ) {
            LOG(INFO) << "E: " << ExactDists_[i].first << " -> " << ApproxDists_[i];
          }
          LOG(FATAL) << "bug: p = " << p << " k = " << k;
        }
        CHECK(p >= k);
        PrecisionOfApprox_ += static_cast<double>(k + 1) / (LastEqualP + 1);
        LogRelPosError_    += log(static_cast<double>(LastEqualP + 1) / (k + 1));
      }
      PrecisionOfApprox_  /= ApproxDists_.size();
      LogRelPosError_     /= ApproxDists_.size();
    } else {
      /* 
       * Let's assume that an empty result set has a zero degree of approximation.
       * Both the relative position error and the # of points closer than the first
       * approximate neighbor are set to (K_ ? K_:ExactResultSet_.size())
       */
      PrecisionOfApprox_ = 0.0;
      if (!ExactResultSet_.empty()) {
        LogRelPosError_ = log(ExactResultSize);
        NumberCloser_   = ExactResultSize;
      }
    }

  }
  double                              NumberCloser_;
  double                              LogRelPosError_;
  double                              Recall_;
  double                              PrecisionOfApprox_;

  std::vector<dist_t>                 ApproxDists_;
  std::set<IdType>                    ApproxResultSet_;
  std::set<IdType>                    ExactResultSet_;

  const DistObjectPairVector<dist_t>& ExactDists_;
};

}

#endif

