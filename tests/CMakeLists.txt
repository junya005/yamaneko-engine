find_package(GTest CONFIG REQUIRED)

# テスト対象のファイルをここに列挙してください
add_executable(yamaneko_tests
    ai/BehaviorTreeTest.cpp
)

target_link_libraries(yamaneko_tests PRIVATE
    yamaneko_engine
    GTest::gtest
    GTest::gtest_main
)

# CTest / Visual Studio テストエクスプローラーで各テストケースを自動検出する
include(GoogleTest)
gtest_discover_tests(yamaneko_tests)
