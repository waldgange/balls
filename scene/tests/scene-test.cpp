#include <scene-manager.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Balls {

class MockBall : public Ball {
public:
    MOCK_METHOD(void, process, (const float dt, const uint16_t width, const uint16_t height), (override));
    MOCK_METHOD(void, process_collision, (const std::shared_ptr<Ball>& other), (override));
    MOCK_METHOD(bool, collides, (const std::shared_ptr<Ball>& other), (const, override));
    MOCK_METHOD(uint32_t, id, (), (const, override));
    MOCK_METHOD(float, x, (), (const, override));
    MOCK_METHOD(float, y, (), (const, override));
    MOCK_METHOD(float, r, (), (const, override));
};

static
Frame generate_mock_balls_for_pre_phase() {
    Frame f;
    for (int i = 0; i < 10; ++i) {
        auto b = std::make_shared<MockBall>();
        EXPECT_CALL(*b, process)
                .Times(1);
        f.emplace_back(std::move(b));
    }
    return f;
}

static
UniqueBallPairs generate_mock_ball_pairs_for_narrow_phase() {
    UniqueBallPairs bp;
    for (int i = 0; i < 10; ++i) {
        bool coll = i < 5;
        auto b1 = std::make_shared<MockBall>();
        auto b2 = std::make_shared<MockBall>();
        EXPECT_CALL(*b1, collides)
                .WillOnce(testing::Return(coll));
        EXPECT_CALL(*b1, process_collision)
                .Times(coll);
        bp.emplace(std::move(b1), std::move(b2));
    }
    return bp;
}

TEST(SceneTest, SequentPrePhaseTest)
{
    Frame f = generate_mock_balls_for_pre_phase();
    auto ppm = make_pre_manager(PrePhaseType::SEQUENT);
    ppm->process_balls(f, 1.0f, 600, 600);
}

TEST(SceneTest, ParallelPrePhaseTest)
{
    Frame f = generate_mock_balls_for_pre_phase();
    auto ppm = make_pre_manager(PrePhaseType::PARALLEL);
    ppm->start();
    ppm->process_balls(f, 1.0f, 600, 600);
    ppm->stop();
}

TEST(SceneTest, QwattroBroadPhaseTest)
{
    Frame f;
    for (int i = 0; i < 4; ++i) {
        auto b = std::make_shared<MockBall>();
        f.emplace_back(std::move(b));
    }
    auto bpm = make_broad_manager(BroadPhaseType::QWATTRO);
    UniqueBallPairs bp = bpm->get_potential_collisions(f);
    EXPECT_EQ(bp.size(), 6);
}

TEST(SceneTest, SortAndSweepBroadPhaseTest)
{
    auto b1 = std::make_shared<MockBall>();
    EXPECT_CALL(*b1, id)
            .WillRepeatedly(testing::Return(1));
    EXPECT_CALL(*b1, r)
            .WillRepeatedly(testing::Return(10));
    EXPECT_CALL(*b1, x)
            .WillRepeatedly(testing::Return(100));
    EXPECT_CALL(*b1, y)
            .WillRepeatedly(testing::Return(100));
    auto b2 = std::make_shared<MockBall>();
    EXPECT_CALL(*b2, id)
            .WillRepeatedly(testing::Return(2));
    EXPECT_CALL(*b2, r)
            .WillRepeatedly(testing::Return(10));
    EXPECT_CALL(*b2, x)
            .WillRepeatedly(testing::Return(119));
    EXPECT_CALL(*b2, y)
            .WillRepeatedly(testing::Return(119));
    auto b3 = std::make_shared<MockBall>();
    EXPECT_CALL(*b3, id)
            .WillRepeatedly(testing::Return(3));
    EXPECT_CALL(*b3, r)
            .WillRepeatedly(testing::Return(10));
    EXPECT_CALL(*b3, x)
            .WillRepeatedly(testing::Return(130));
    EXPECT_CALL(*b3, y)
            .WillRepeatedly(testing::Return(130));
    Frame f = {b1, b2, b3};

    auto bpm = make_broad_manager(BroadPhaseType::SORT_AND_SWEEP);
    UniqueBallPairs bp = bpm->get_potential_collisions(f);
    EXPECT_EQ(bp.size(), 2);
    for (const auto& b : bp) {
        std::cout << b.first->id() << " " << b.second->id() << std::endl;
    }
    EXPECT_NE(bp.find(BallPair(b1, b2)), bp.end());
    EXPECT_NE(bp.find(BallPair(b2, b3)), bp.end());
}

TEST(SceneTest, SequentNarrowPhaseTest)
{
    UniqueBallPairs bp = generate_mock_ball_pairs_for_narrow_phase();
    auto npm = make_narrow_manager(NarrowPhaseType::SEQUENT);
    npm->process_potential_collisions(bp);
}

TEST(SceneTest, ParallelNarrowPhaseTest)
{
    UniqueBallPairs bp = generate_mock_ball_pairs_for_narrow_phase();
    auto npm = make_narrow_manager(NarrowPhaseType::PARALLEL);
    npm->start();
    npm->process_potential_collisions(bp);
    npm->stop();
}

}
