#include "broad-phase-quattro-detector.h"

namespace Balls {


UniqueBallPairs QuattroDetector::get_potential_collisions(const Frame &bf) const {
    std::lock_guard<std::mutex> guard(prev_result.m);
    if (bf.size() != prev_result.frame_size) {
        prev_result.frame_size = bf.size();
        UniqueBallPairs new_ball_pairs;
        for (auto it1 = bf.begin(); it1 != bf.end(); ++it1) {
            for (auto it2 = it1 + 1; it2 != bf.end(); ++it2) {
                new_ball_pairs.emplace(*it1, *it2);
            }
        }
        std::swap(prev_result.pairs, new_ball_pairs);
    }
    return prev_result.pairs;
}


}
