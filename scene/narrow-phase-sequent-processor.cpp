#include "narrow-phase-sequent-processor.h"

namespace Balls {

void NarrowPhaseSequentProcessor::process_potential_collisions(UniqueBallPtrPairs pairs) {
    filter_potential_collisions(pairs);
    UniqueBallPtrPairs new_colliding_pairs = pairs;
    for (const auto& cp : colliding_pairs) {
        pairs.erase(cp);
    }
    std::swap(colliding_pairs, new_colliding_pairs);
    process_collisions(pairs);
}

void NarrowPhaseSequentProcessor::process_collisions(UniqueBallPtrPairs &ball_pairs) {
    for (auto& b : ball_pairs) {
        b.first->process_collision(b.second);
    }
}

void NarrowPhaseSequentProcessor::filter_potential_collisions(UniqueBallPtrPairs &ubp) const {
    for (auto it = ubp.begin(); it != ubp.end();) {
        const std::shared_ptr<Ball>& b1 = it->first;
        const std::shared_ptr<Ball>& b2 = it->second;
        bool colliding = b1->collides(b2);
        if (!colliding) {
            it = ubp.erase(it);
        } else {
            ++it;
        }
    }
}

}
