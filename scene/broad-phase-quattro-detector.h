#ifndef BROADPHASEQUATTRODETECTOR_H
#define BROADPHASEQUATTRODETECTOR_H

#include "scene-manager.h"

#include <mutex>

namespace Balls {

class QuattroDetector : public BroadPhaseManager
{
protected:
    UniqueBallPairs get_potential_collisions(const Frame& bf) const override;
private:
    mutable struct CachePairs {
        std::mutex m;
        uint16_t frame_size;
        UniqueBallPairs pairs;
    } prev_result;
};


}

#endif // BROADPHASEQUATTRODETECTOR_H
