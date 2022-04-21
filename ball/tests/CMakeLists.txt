add_executable(ball-test
  ball-test.cpp)

target_link_libraries(ball-test
 PRIVATE
  GTest::GTest
  ball)

include(GoogleTest)
gtest_discover_tests(ball-test PROPERTIES TIMEOUT 10)

add_custom_target(run-ball-test ALL
                  COMMENT "Run ball tests"
                  COMMAND ctest
                  DEPENDS ball ball-test)
