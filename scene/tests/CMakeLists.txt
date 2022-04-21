add_executable(scene-test
  scene-test.cpp)

target_link_libraries(scene-test
 PRIVATE
  GTest::GTest
  scene)

include(GoogleTest)
gtest_discover_tests(scene-test PROPERTIES TIMEOUT 10)

add_custom_target(run-scene-test ALL
                  COMMENT "Run scene tests"
                  COMMAND ctest
                  DEPENDS scene scene-test)
