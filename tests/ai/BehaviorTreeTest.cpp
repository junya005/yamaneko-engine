#include <gtest/gtest.h>
#include "ai/bt/BehaviorTree.h"

using namespace yamaneko_engine::ai;

// テスト用のモックノード（指定したステータスを返す・呼び出し回数を記録する）
class MockBehavior : public BTBehavior {
public:
    BTStatus return_status = BTStatus::kSuccess;
    int init_count = 0;
    int update_count = 0;
    int terminate_count = 0;
    BTStatus last_terminated_status = BTStatus::kInvalid;

protected:
    void OnInitialize() override {
        ++init_count;
    }

    BTStatus Update() override {
        ++update_count;
        return return_status;
    }

    void OnTerminate(BTStatus status) override {
        ++terminate_count;
        last_terminated_status = status;
    }
};

// 1. 単体ノードの基本動作テスト
TEST(BehaviorTreeTest, MockNodeReturnsConfiguredStatus) {
    MockBehavior node;
    node.return_status = BTStatus::kSuccess;

    EXPECT_EQ(node.Tick(), BTStatus::kSuccess);
    EXPECT_EQ(node.update_count, 1);
    EXPECT_EQ(node.terminate_count, 1);
    EXPECT_TRUE(node.IsTerminated());
}

// 2. BTSequence の動作テスト（全子ノードが成功すると成功）
TEST(BehaviorTreeTest, SequenceSucceedsWhenAllChildrenSucceed) {
    BTSequence seq;
    MockBehavior child1;
    MockBehavior child2;
    child1.return_status = BTStatus::kSuccess;
    child2.return_status = BTStatus::kSuccess;

    seq.AddChild(&child1);
    seq.AddChild(&child2);

    EXPECT_EQ(seq.Tick(), BTStatus::kSuccess);
    EXPECT_EQ(child1.update_count, 1);
    EXPECT_EQ(child2.update_count, 1);
}

// 3. BTSequence の中断テスト（途中で失敗したら後続は実行されない）
TEST(BehaviorTreeTest, SequenceStopsOnFailure) {
    BTSequence seq;
    MockBehavior child1;
    MockBehavior child2;
    child1.return_status = BTStatus::kFailure;
    child2.return_status = BTStatus::kSuccess;

    seq.AddChild(&child1);
    seq.AddChild(&child2);

    EXPECT_EQ(seq.Tick(), BTStatus::kFailure);
    EXPECT_EQ(child1.update_count, 1);
    EXPECT_EQ(child2.update_count, 0); // child2 は呼ばれない
}

// 4. 複数フレームにまたがる kRunning のテスト
TEST(BehaviorTreeTest, SequenceResumesFromRunningChild) {
    BTSequence seq;
    MockBehavior child1;
    MockBehavior child2;
    child1.return_status = BTStatus::kSuccess;
    child2.return_status = BTStatus::kRunning;

    seq.AddChild(&child1);
    seq.AddChild(&child2);

    // 1フレーム目: child1 成功 -> child2 が Running
    EXPECT_EQ(seq.Tick(), BTStatus::kRunning);
    EXPECT_EQ(child1.update_count, 1);
    EXPECT_EQ(child2.update_count, 1);

    // 2フレーム目: child2 が成功に変わると Sequence 全体が成功
    child2.return_status = BTStatus::kSuccess;
    EXPECT_EQ(seq.Tick(), BTStatus::kSuccess);
    EXPECT_EQ(child1.update_count, 1); // child1 は再実行されない
    EXPECT_EQ(child2.update_count, 2);
}