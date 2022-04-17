#ifndef BROADPHASESORTANDSWEEPDETECTOR_H
#define BROADPHASESORTANDSWEEPDETECTOR_H

#include "scene-manager.h"

namespace Balls {


class SortAndSweepDetector : public BroadPhaseManager
{
protected:
    UniqueBallPtrPairs get_potential_collisions(const Frame& bf) const override;
};


}

#endif // BROADPHASESORTANDSWEEPDETECTOR_H
