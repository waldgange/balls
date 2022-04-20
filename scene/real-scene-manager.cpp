#include "real-scene-manager.h"

namespace Balls {


RealSceneManager::RealSceneManager(const std::shared_ptr<PrePhaseManager>& _ppm,
                                   const std::shared_ptr<BroadPhaseManager>& _bpm,
                                   const std::shared_ptr<NarrowPhaseManager>& _npm)
    : SceneManager(_ppm, _bpm, _npm) {}

void RealSceneManager::start() {
    ppm->start();
    npm->start();
}

void RealSceneManager::set_size(const uint16_t w, const uint16_t h) {
    std::lock_guard<std::mutex> guard(scene_mutex);
    width = w;
    height = h;
}

void RealSceneManager::add_ball(std::shared_ptr<Ball> b) {
    std::lock_guard<std::mutex> guard(scene_mutex);
    frame.emplace_back(b);
}

void RealSceneManager::remove_balls() {
    std::lock_guard<std::mutex> guard(scene_mutex);
    frame.clear();
}

Frame RealSceneManager::get_next_frame() {
    std::lock_guard<std::mutex> guard(scene_mutex);
    for (int i = 0; i < MAX_BALL_SPEED; ++i) {
        process_scene(1.0f / MAX_BALL_SPEED);
    }
    return frame;
}

void RealSceneManager::process_scene(const float dt) {
    ppm->process_balls(frame, dt, width, height);
    UniqueBallPairs ball_pairs = bpm->get_potential_collisions(frame);
    npm->process_potential_collisions(std::move(ball_pairs));
}

}
