#include "scene-manager.h"
#include "real-scene-manager.h"
#include "pre-phase-sequent-processor.h"
#include "pre-phase-parallel-processor.h"
#include "broad-phase-quattro-detector.h"
#include "broad-phase-sort-and-sweep-detector.h"
#include "narrow-phase-sequent-processor.h"
#include "narrow-phase-parallel-processor.h"

#include <memory>

namespace Balls {


std::shared_ptr<SceneManager> make_scene_manager(PrePhaseType pt,
                                                 BroadPhaseType bt,
                                                 NarrowPhaseType nt) {
    auto ppm = make_pre_manager(pt);
    auto bpm = make_broad_manager(bt);
    auto npm = make_narrow_manager(nt);
    if (!ppm || !bpm || !npm) {
        return nullptr;
    }
    return std::make_shared<RealSceneManager>(std::move(ppm), std::move(bpm), std::move(npm));
}

std::shared_ptr<PrePhaseManager> make_pre_manager(PrePhaseType pt) {
    std::shared_ptr<PrePhaseManager> ppm;
    if (pt == PrePhaseType::SEQUENT) {
        ppm = std::make_shared<PrePhaseSequentProcessor>();
    } else if (pt == PrePhaseType::PARALLEL) {
        ppm = std::make_shared<PrePhaseParallelProcessor>();
    }
    return ppm;
}

std::shared_ptr<BroadPhaseManager> make_broad_manager(BroadPhaseType bt) {
    std::shared_ptr<BroadPhaseManager> bpm;
    if (bt == BroadPhaseType::QWATTRO) {
        bpm = std::make_shared<QuattroDetector>();
    } else if (bt == BroadPhaseType::SORT_AND_SWEEP) {
        bpm = std::make_shared<SortAndSweepDetector>();
    }
    return bpm;
}

std::shared_ptr<NarrowPhaseManager> make_narrow_manager(NarrowPhaseType nt) {
    std::shared_ptr<NarrowPhaseManager> npm;
    if (nt == NarrowPhaseType::SEQUENT) {
        npm = std::make_shared<NarrowPhaseSequentProcessor>();
    } else if (nt == NarrowPhaseType::PARALLEL) {
        npm = std::make_shared<NarrowPhaseParallelProcessor>();
    }
    return npm;
}

SceneManager::SceneManager(const std::shared_ptr<PrePhaseManager>&    _ppm,
                           const std::shared_ptr<BroadPhaseManager>&  _bpm,
                           const std::shared_ptr<NarrowPhaseManager>& _npm)
    : ppm(_ppm)
    , bpm(_bpm)
    , npm(_npm) { }

SceneManager::~SceneManager() {
    ppm->stop();
    bpm->stop();
    npm->stop();
}

void SceneManager::start() {
    ppm->start();
    bpm->start();
    npm->start();
}

void SceneManager::set_pre_phase_manager(std::shared_ptr<PrePhaseManager> _ppm) {
    ppm->stop();
    std::swap(ppm, _ppm);
    ppm->start();
}

void SceneManager::set_broad_phase_manager(std::shared_ptr<BroadPhaseManager> _bpm) {
    bpm->stop();
    std::swap(bpm, _bpm);
    bpm->start();
}

void SceneManager::set_narrow_phase_manager(std::shared_ptr<NarrowPhaseManager> _npm) {
    npm->stop();
    std::swap(npm, _npm);
    npm->start();
}


}
