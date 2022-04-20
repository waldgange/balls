#ifndef NARROWPHASESEQUENTPROCESSOR_H
#define NARROWPHASESEQUENTPROCESSOR_H

#include "scene-manager.h"

namespace Balls {

class NarrowPhaseSequentProcessor : public NarrowPhaseManager
{
public:
    void process_potential_collisions(UniqueBallPairs pairs) override;

protected:
    UniqueBallPairs colliding_pairs;

    void filter_potential_collisions(UniqueBallPairs& ubp) const;
    void process_collisions(UniqueBallPairs& ubp);
};

}

#endif // NARROWPHASESEQUENTPROCESSOR_H
