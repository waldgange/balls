#ifndef SWEEPANDPRUNESCENEMANAGER_H
#define SWEEPANDPRUNESCENEMANAGER_H

#include "common-scene-manager.h"

namespace Balls {


class SweepAndPruneSceneManager : public CommonSceneManager
{
protected:
    UniqueBallPairs get_potential_collisions(const Frame& bf) const override;
};


}

#endif // SWEEPANDPRUNESCENEMANAGER_H
