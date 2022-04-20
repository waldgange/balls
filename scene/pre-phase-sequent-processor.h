#ifndef PREPHASESEQUENTPROCESSOR_H
#define PREPHASESEQUENTPROCESSOR_H

#include <scene-manager.h>


namespace Balls {


class PrePhaseSequentProcessor : public PrePhaseManager
{
public:
    void process_balls(Frame& f,
                       const float dt,
                       const uint16_t width,
                       const uint16_t height) override;
};


}

#endif // PREPHASESEQUENTPROCESSOR_H
