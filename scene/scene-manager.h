#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../ball/ball.h"
#include <unordered_set>
#include <vector>

namespace Balls {

using Frame = std::vector<std::shared_ptr<Ball>>;

using BallPair = std::pair<uint16_t, uint16_t>;
struct BallsPairHash {
    size_t operator()(BallPair p) const noexcept {
        return size_t(p.first) << 16 | p.second;
    }
};
using UniqueBallPairs = std::unordered_set<BallPair, BallsPairHash>;

class SceneManager {
public:
    virtual ~SceneManager() = 0;

    virtual void add_ball(std::shared_ptr<Ball> b) = 0;
    virtual void remove_balls() = 0;
    virtual Frame get_next_frame() = 0;

    void set_size(const uint16_t w, const uint16_t h);

protected:
    uint16_t width;
    uint16_t height;
};

enum class ManagerType {
    QWATTRO_PAIRS_CHECK,
    SWEEP_AND_PRUNE,
    SWEEP_AND_PRUNE_MULTITHREAD
};

std::unique_ptr<SceneManager> make_scene_manager(ManagerType mt);

}

#endif // SCENEMANAGER_H
