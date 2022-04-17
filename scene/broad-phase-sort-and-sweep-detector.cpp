#include "broad-phase-sort-and-sweep-detector.h"

#include <map>

namespace Balls {

UniqueBallPtrPairs SortAndSweepDetector::get_potential_collisions(const Frame& bf) const {
    // "Sort and Sweep" algorythm
    // Breaking O(n^2) complexity of checking all the pairs
    std::multimap<float, std::shared_ptr<Ball>> balls_x, balls_y;
    for(const auto& b : bf) {
        balls_x.emplace(b->x() - b->r(), b);
        balls_y.emplace(b->y() - b->r(), b);
    }

    auto filter_pairs = [&] (std::multimap<float, std::shared_ptr<Ball>>& balls_on_axle) -> UniqueBallPtrPairs {
        UniqueBallPtrPairs r;

        auto l_it = balls_on_axle.begin();
        auto r_it = l_it;
        while (l_it != balls_on_axle.end()) {
            if (l_it == r_it) {
                ++r_it;
                continue;
            }
            if (r_it == balls_on_axle.end() || l_it->first + l_it->second->r() * 2 < r_it->first) {
                ++l_it;
                r_it = l_it;
                continue;
            }
            if (l_it->second->id() < r_it->second->id()) {
                r.emplace(l_it->second, r_it->second);
            } else {
                r.emplace(r_it->second, l_it->second);
            }
            ++r_it;
        }

        return r;
    };

    UniqueBallPtrPairs result;
    const UniqueBallPtrPairs bx = filter_pairs(balls_x);
    const UniqueBallPtrPairs by = filter_pairs(balls_y);
    for (const auto& b : bx) {
        if (by.find(b) != by.end()) {
            result.emplace(b);
        }
    }

    return result;
}


}
