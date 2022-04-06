#include "scene-manager.h"
#include "quattro-check-scene-manager.h"
#include "sweep-and-prune-scene-manager.h"

#include <memory>

namespace Balls {

SceneManager::~SceneManager() {}

std::unique_ptr<SceneManager> make_scene_manager(const ManagerType mt) {
    if (mt == ManagerType::QWATTRO_PAIRS_CHECK) {
        return std::make_unique<QuattroCheckSceneManager>();
    } else if (mt == ManagerType::SWEEP_AND_PRUNE) {
        return std::make_unique<SweepAndPruneSceneManager>();
    }
    return nullptr;
}

void SceneManager::set_size(const uint16_t w, const uint16_t h) {
    width = w;
    height = h;
}


}
