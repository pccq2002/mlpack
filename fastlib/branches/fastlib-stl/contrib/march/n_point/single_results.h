/*
 *  single_results.h
 *  
 *
 *  Created by William March on 9/7/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SINGLE_RESULTS_H
#define SINGLE_RESULTS_H

#include "single_matcher.h"
#include "boost/multi_array.h"

/*
 *  Knows the structure of the results and processes the intermediate 
 *  results from the matcher
 *  This is where a result from n tree nodes gets processed into the 
 *  correct jackknife results
 * 
 *  The generic resampling class handles giving the right stuff to the matcher
 *  and running the algorithm (through the generic algorithm).
 *  This takes the matcher and info on which computation(s) were run and 
 *  puts the results in the right place.
 *
 *  IMPORTANT: for now, only writing this for the angle matchers
 *  In order to do general n, I'll need to figure out how to do runtime
 *  sized multi arrays
 *  
 */


namespace npt {
  
  class SingleResults {

  private:
    
    // indexed by [resampling_region][num_random][r1][theta]
    boost::multi_array<int, 4> results_;
    boost::multi_array<double, 4> weighted_results_;
    
    // indexed by [r1][theta]
    boost::multi_array<int, 2> RRR_result_;
    boost::multi_array<double, 2> RRR_weighted_result_;
    
    
    int num_regions_;
    int num_r1_;
    int num_theta_;
    
    std::vector<double> r1_vec_;
    std::vector<double> theta_vec_;
    
    static int tuple_size_ = 3;
    
    void AddResult_(int region_ind, int num_random, 
                    int r1_ind, int theta_ind, int result);
    
    void AddRandomResult_(boost::multi_array<int, 2>& partial_result);

    
    
  public:
    
    SingleResults(int num_regions, 
                  std::vector<double>& r1_vec,
                  std::vector<double>& theta_vec) :
    num_regions_(num_regions), num_r1_(r1_vec.size()), 
    num_theta_(theta_vec.size()),
    results_(boost::extents[num_regions][tuple_size_]
             [r1_vec.size()][theta_vec.size()]),
    weighted_results_(boost::extents[num_regions][tuple_size_]
                      [r1_vec.size()][theta_vec.size()]),
    RRR_result_(boost::extents[r1_vec.size()][theta_vec.size()]),
    RRR_weighted_result_(boost::extents[r1_vec.size()][theta_vec.size()]),
    r1_vec_(r1_vec), theta_vec_(theta_vec)
    {
    
      std::fill(results_.origin(), results_.origin() + results_.size(), 0);
      std::fill(weighted_results_.origin(), 
                weighted_results_.origin() + weighted_results_.size(), 0.0);
      
      std::fill(RRR_result_.origin(), RRR_result_.origin() + RRR_result_.size(), 0);
      std::fill(RRR_weighted_result_.origin(), 
                RRR_weighted_result_.origin() + RRR_weighted_result_.size(), 0.0);
      
      
    } // constructor
    
    
    void ProcessResults(std::vector<int> region_ids, int num_random,
                        SingleMatcher& matcher,
                        int r1_ind, int theta_ind);
    
    
    void PrintResults();
    
    boost::multi_array<int, 4>& results() {
      return results_;
    }
    
    boost::multi_array<2, int>& RRR_result() {
      return RRR_result_;
    }
    
    
  }; // class
  
} // namespace


#endif