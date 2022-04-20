#include "pre-phase-sequent-processor.h"

namespace Balls {


void PrePhaseSequentProcessor::process_balls(Frame& balls,
                                             const float dt,
                                             const uint16_t width,
                                             const uint16_t height) {
    for (auto& b : balls) {
        b->process(dt, width, height);
    }
}


}
