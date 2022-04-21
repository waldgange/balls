#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "ball.h"
#include <cassert>
#include <unordered_set>
#include <vector>


namespace Balls {

using Frame = std::vector<std::shared_ptr<Ball>>;

using BallPair = std::pair<std::shared_ptr<Ball>, std::shared_ptr<Ball>>;
struct BallPairHash {
    std::size_t operator() (BallPair p) const noexcept {
        static_assert(sizeof(std::size_t) >= 2 * sizeof(uint32_t),
                      "BallPairHash : can not hash ball pair");
        return std::size_t(p.second->id()) << 32 | p.first->id();
    }
};
using UniqueBallPairs = std::unordered_set<BallPair, BallPairHash>;

class PrePhaseManager {
public:
    virtual ~PrePhaseManager() {};

    virtual void start() {};
    virtual void stop() {};
    virtual void process_balls(Frame& bf,
                               const float dt,
                               const uint16_t width,
                               const uint16_t height) = 0;
};

class BroadPhaseManager {
public:
    virtual ~BroadPhaseManager() {};

    virtual UniqueBallPairs get_potential_collisions(const Frame& bf) const = 0;
};

class NarrowPhaseManager {
public:
    virtual ~NarrowPhaseManager() {};

    virtual void start() {};
    virtual void stop() {};
    virtual void process_potential_collisions(UniqueBallPairs potential_collisions) = 0;
};


class SceneManager {
public:
    SceneManager(const std::shared_ptr<PrePhaseManager>&    _ppm,
                 const std::shared_ptr<BroadPhaseManager>&  _bpm,
                 const std::shared_ptr<NarrowPhaseManager>& _npm);
    virtual ~SceneManager() {};

    virtual void start() = 0;
    virtual void set_size(const uint16_t w, const uint16_t h) = 0;
    virtual void add_ball(std::shared_ptr<Ball> b) = 0;
    virtual void remove_balls() = 0;
    virtual Frame get_next_frame() = 0;

protected:
    std::shared_ptr<PrePhaseManager>    ppm;
    std::shared_ptr<BroadPhaseManager>  bpm;
    std::shared_ptr<NarrowPhaseManager> npm;
};

enum class PrePhaseType {
    SEQUENT,
    PARALLEL
};

enum class BroadPhaseType {
    QWATTRO,
    SORT_AND_SWEEP
};

enum class NarrowPhaseType {
    SEQUENT,
    PARALLEL
};

std::shared_ptr<SceneManager> make_scene_manager(PrePhaseType pt,
                                                 BroadPhaseType bt,
                                                 NarrowPhaseType nt);

std::shared_ptr<PrePhaseManager> make_pre_manager(PrePhaseType pt);

std::shared_ptr<BroadPhaseManager> make_broad_manager(BroadPhaseType bt);

std::shared_ptr<NarrowPhaseManager> make_narrow_manager(NarrowPhaseType nt);


}

#endif // SCENEMANAGER_H
