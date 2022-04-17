#include "scene-manager.h"
#include "real-scene-manager.h"
#include "broad-phase-quattro-detector.h"
#include "broad-phase-sort-and-sweep-detector.h"
#include "narrow-phase-sequent-processor.h"
#include "narrow-phase-parallel-processor.h"

#include <memory>

namespace Balls {


std::shared_ptr<SceneManager> make_scene_manager(BroadPhaseType bt, NarrowPhaseType nt) {
    std::shared_ptr<BroadPhaseManager> bpm;
    std::shared_ptr<NarrowPhaseManager> npm;
    if (bt == BroadPhaseType::QWATTRO) {
        bpm = std::make_shared<QuattroDetector>();
    } else if (bt == BroadPhaseType::SORT_AND_SWEEP) {
        bpm = std::make_shared<SortAndSweepDetector>();
    }
    if (nt == NarrowPhaseType::SEQUENT) {
        npm = std::make_shared<NarrowPhaseSequentProcessor>();
    } else if (nt == NarrowPhaseType::PARALLEL) {
        npm = std::make_shared<NarrowPhaseParallelProcessor>();
    }
    if (!bpm || !npm) {
        return nullptr;
    }
    return std::make_shared<RealSceneManager>(std::move(bpm), std::move(npm));
}

SceneManager::SceneManager(const std::shared_ptr<BroadPhaseManager>&  _bpm,
                           const std::shared_ptr<NarrowPhaseManager>& _npm)
    : bpm(_bpm)
    , npm(_npm) { }


}
