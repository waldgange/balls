#include <ball.h>
#include <gtest/gtest.h>

const float FP_COMPARE_DELTA = 0.0001;

TEST(BallTest, IdTest)
{
    auto b1 = Balls::make_ball(Balls::BallType::REAL_BALL, 0, 0);
    auto b2 = Balls::make_ball(Balls::BallType::REAL_BALL, 0, 0);
    EXPECT_NE(b1->id(), b2->id());
}

TEST(BallTest, ProcessTest)
{
    float x = 100;
    float y = 100;
    float dx = 12;
    float dy = 21;
    auto b = Balls::make_ball(Balls::BallType::REAL_BALL, x, y, 10, dx, dy);
    b->process(1.0f, 600, 600);
    EXPECT_TRUE(abs(x + dx - b->x()) < FP_COMPARE_DELTA);
    EXPECT_TRUE(abs(y + dy - b->y()) < FP_COMPARE_DELTA);
}

TEST(BallTest, CollidingTest)
{
    auto b1 = Balls::make_ball(Balls::BallType::REAL_BALL, 100, 100, 10, 0, 0);
    auto b2 = Balls::make_ball(Balls::BallType::REAL_BALL, 115, 100, 10, 0, 0);
    auto b3 = Balls::make_ball(Balls::BallType::REAL_BALL, 130, 100, 10, 0, 0);
    EXPECT_TRUE(b1->collides(b2));
    EXPECT_FALSE(b1->collides(b3));
    EXPECT_TRUE(b2->collides(b1));
    EXPECT_TRUE(b2->collides(b3));
    EXPECT_TRUE(b3->collides(b2));
    EXPECT_FALSE(b3->collides(b1));
    bool self_collision_allowed = false;
    try {
        b1->collides(b1);
        self_collision_allowed = true;
    } catch (const std::runtime_error& e) {
        std::cout << e.what();
    }
    try {
        b1->process_collision(b1);
        self_collision_allowed = true;
    } catch (const std::runtime_error& e) {
        std::cout << e.what();
    }
    EXPECT_FALSE(self_collision_allowed);
}

TEST(BallTest, ProcessCollisionTest)
{
    auto b1 = Balls::make_ball(Balls::BallType::REAL_BALL, 100, 100, 10, 20, 0);
    auto b2 = Balls::make_ball(Balls::BallType::REAL_BALL, 130, 100, 10, 0, 0);

    EXPECT_FALSE(b1->collides(b2));
    b1->process(1.0f, 600, 600);
    b2->process(1.0f, 600, 600);
    EXPECT_TRUE(b1->collides(b2));
    b1->process_collision(b2);
    b1->process(1.0f, 600, 600);
    b2->process(1.0f, 600, 600);
    EXPECT_FALSE(b1->collides(b2));
}
