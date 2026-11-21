/// <file>BehaviorTree.h</file>
/// <summary>
/// BTに必要なクラス、構造体、列挙子等の宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>


#pragma once

namespace yamaneko_engine::ai {
	/// <summary>
/// BTのノードの状態を表す列挙子。
/// </summary>
	enum class BTNodeState {
		Success, ///< 成功
		Failure, ///< 失敗
		Running  ///< 実行中
	};

	struct BTContext{
		void* agent; ///< エージェントのポインタ
		float delta_time; ///< 前回のフレームからの経過時間
	};

	class BTNode {
	public:
		virtual ~BTNode() = default;
		virtual BTNodeState Tick(BTContext& context) = 0;
	};

	class BTConditionNode : public BTNode {
	protected:
		BTNodeState Tick(BTContext& context) override {
			// 条件ノードの処理を実装
			return BTNodeState::Success; // 仮の実装
		}
	};
}
