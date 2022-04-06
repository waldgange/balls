#include "sweep-and-prune-scene-manager.h"

#include <map>

namespace Balls {

UniqueBallPairs SweepAndPruneSceneManager::get_potential_collisions(const Frame& bf) const {
    // "Sweep and Prune" algorythm
    // Breaking O(n^2) complexity of checking all the pairs
    std::multimap<float, int> balls_x, balls_y;
    for(int i = 0; i < bf.size(); ++i) {
        balls_x.emplace(bf[i]->x() - bf[i]->r(), i);
        balls_y.emplace(bf[i]->y() - bf[i]->r(), i);
    }

    auto filter_pairs = [&] (std::multimap<float, int>& balls_on_axle) -> UniqueBallPairs {
        UniqueBallPairs r;

        auto l_it = balls_on_axle.begin();
        auto r_it = l_it;
        while (l_it != balls_on_axle.end()) {
            if (l_it == r_it) {
                ++r_it;
                continue;
            }
            if (r_it == balls_on_axle.end() || l_it->first + bf[l_it->second]->r() * 2 < r_it->first) {
                ++l_it;
                r_it = l_it;
                continue;
            }
            if (l_it->second < r_it->second) {
                r.emplace(l_it->second, r_it->second);
            } else {
                r.emplace(r_it->second, l_it->second);
            }
            ++r_it;
        }

        return r;
    };

    UniqueBallPairs result;
    const UniqueBallPairs bx = filter_pairs(balls_x);
    const UniqueBallPairs by = filter_pairs(balls_y);
    for (const auto& b : bx) {
        if (by.find(b) != by.end()) {
            result.emplace(b);
        }
    }

    return result;
}


}
