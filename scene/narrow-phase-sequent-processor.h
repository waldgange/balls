#ifndef NARROWPHASESEQUENTPROCESSOR_H
#define NARROWPHASESEQUENTPROCESSOR_H

#include "scene-manager.h"

namespace Balls {

class NarrowPhaseSequentProcessor : public NarrowPhaseManager
{
public:
    void process_potential_collisions(UniqueBallPtrPairs pairs) override;

protected:
    UniqueBallPtrPairs colliding_pairs;

    void filter_potential_collisions(UniqueBallPtrPairs& ubp) const;
    void process_collisions(UniqueBallPtrPairs& ubp);
};

}

#endif // NARROWPHASESEQUENTPROCESSOR_H
