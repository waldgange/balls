#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "ball.h"
#include <cassert>
#include <unordered_set>
#include <vector>


namespace Balls {

using Frame = std::vector<std::shared_ptr<Ball>>;

using BallPtrPair = std::pair<std::shared_ptr<Ball>, std::shared_ptr<Ball>>;
struct BallsPtrPairHash {
    std::size_t operator() (BallPtrPair p) const noexcept {
        assert(sizeof(std::size_t) >= 2 * sizeof(uint32_t));
        return std::size_t(p.first->id()) << 32 | p.second->id();
    }
};
using UniqueBallPtrPairs = std::unordered_set<BallPtrPair, BallsPtrPairHash>;

class BroadPhaseManager {
public:
    virtual ~BroadPhaseManager() {};

    virtual UniqueBallPtrPairs get_potential_collisions(const Frame& bf) const = 0;
};

class NarrowPhaseManager {
public:
    virtual ~NarrowPhaseManager() {};

    virtual void start() {};
    virtual void process_potential_collisions(UniqueBallPtrPairs potential_collisions) = 0;
};


class SceneManager {
public:
    SceneManager(const std::shared_ptr<BroadPhaseManager>&  _bpm,
                 const std::shared_ptr<NarrowPhaseManager>& _npm);
    virtual ~SceneManager() {};

    virtual void start() = 0;
    virtual void set_size(const uint16_t w, const uint16_t h) = 0;
    virtual void add_ball(std::shared_ptr<Ball> b) = 0;
    virtual void remove_balls() = 0;
    virtual Frame get_next_frame() = 0;

protected:
    std::shared_ptr<BroadPhaseManager>  bpm;
    std::shared_ptr<NarrowPhaseManager> npm;
};


enum class BroadPhaseType {
    QWATTRO,
    SORT_AND_SWEEP
};

enum class NarrowPhaseType {
    SEQUENT,
    PARALLEL
};

std::shared_ptr<SceneManager> make_scene_manager(BroadPhaseType bt, NarrowPhaseType nt);

}

#endif // SCENEMANAGER_H
