#ifndef BROADPHASEQUATTRODETECTOR_H
#define BROADPHASEQUATTRODETECTOR_H

#include "scene-manager.h"

#include <mutex>

namespace Balls {

class QuattroDetector : public BroadPhaseManager
{
protected:
    UniqueBallPtrPairs get_potential_collisions(const Frame& bf) const override;
private:
    mutable struct CachePairs {
        std::mutex m;
        uint16_t frame_size;
        UniqueBallPtrPairs pairs;
    } prev_result;
};


}

#endif // BROADPHASEQUATTRODETECTOR_H
