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
#include <vector>

using namespace std;

namespace yamaneko_engine::ai {
	/// <summary>
/// BTのノードの状態を表す列挙子。
/// </summary>
	enum class BTStatus {
		kInvalid,	// 無効
		kSuccess,	// 成功
		kFailure,	// 失敗
		kRunning,	// 実行中
		kAborted	// 中断
	};

	struct BTContext{
		void* agent; // エージェントのポインタ
		float delta_time; // 前回のフレームからの経過時間
	};

	class BTBehavior {
	protected:
		/// <summary>
		/// 最初の呼び出しの直前に一度だけ呼び出される初期化処理です。
		/// </summary>
		virtual void OnInitialize(){}

		/// <summary>
		/// 動作ツリーが実行されるたびに呼び出される更新処理です。実行ステータスによって終了されたことが通知するまで続きます。
		/// </summary>
		virtual BTStatus Update() = 0;

		/// <summary>
		/// 前回の更新処理が終了したことが通知されると一度だけ呼び出される終了処理です。
		/// </summary>
		virtual void OnTerminate(BTStatus status) {}
	private:
		BTStatus status_;
	public:
		BTBehavior() : status_(BTStatus::kRunning) {}
		virtual ~BTBehavior() = default;
		
		BTStatus Tick() {
			// 初期化処理
			if (status_ != BTStatus::kRunning) { 
				OnInitialize(); 
			}

			// 更新処理
			status_ = Update();

			// 終了処理
			if (status_ != BTStatus::kRunning) { 
				OnTerminate(status_);
			};

			return status_;
		}

		void Abort() {
			if (status_ == BTStatus::kRunning) {
				OnTerminate(BTStatus::kAborted);
				status_ = BTStatus::kAborted;
			}
		}

		bool IsTerminated() const { return status_ == BTStatus::kFailure || 
											status_ == BTStatus::kSuccess || 
											status_ == BTStatus::kAborted; }
		bool IsRunning() const { return status_ == BTStatus::kRunning; }
		BTStatus GetStatus() const { return status_; }
	};

	class BTDecorator : public BTBehavior {
	protected:
		BTBehavior* child_;
	public:
		BTDecorator(BTBehavior* child) : child_(child){}
	};

	class BTComposite : public BTBehavior {
	public: 		
		/// <summary>
		/// 子ビヘイビアを内部の子リストに追加します。
		/// </summary>
		/// <param name="child">追加する子ビヘイビアへのポインタです。</param>
		void AddChild(BTBehavior* child) {
			children_.push_back(child);
		}

		/// <summary>
		/// 子ノードのリストから、指定した子ノードを見つけて削除します。
		/// </summary>
		/// <param name="child">削除対象の子ノードへのポインタです。</param>
		void RemoveChild(BTBehavior* child) {
			for(auto it = children_.begin(); it != children_.end(); ++it) {
				if(*it == child) {
					children_.erase(it);
					break;
				}
			}
		}

		/// <summary>
		/// 内部の子ノードリストをすべてクリアします。
		/// </summary>
		void ClearChildren() {
			children_.clear();
		}
	protected:
		typedef vector<BTBehavior*> BTBehaviors;
		BTBehaviors children_;
	};

	class BTSequence : public BTComposite { 
	protected:
		BTBehaviors::iterator current_child_;
		virtual void OnInitialize() override {
			current_child_ = children_.begin();
		}

		virtual BTStatus Update() override {
			// 子ノードが実行中であると示すまで、処理し続けます
			while (true) {
				BTStatus status = (*current_child_)->Tick(BTContext{});
					
				// 子プロセスが実行中、失敗のどちらでも同じ処理を行います
				if (status != BTStatus::kSuccess) return status;

				// 末尾に到達した場合は、成功として終了します
				++current_child_;
				if(current_child_ == children_.end()) {
					return BTStatus::kSuccess;
				}
			}
			return BTStatus::kInvalid;	// ここには到達しないはずです
		}
	};

	class BTFilter : public BTSequence {
	public:
		void AddCondition(BTBehavior* condition) {
			children_.front(condition);
		}

		void AddAction(BTBehavior* action) {
			children_.push_back(action);
		}
	};

	class BTSelector : public BTComposite {
		virtual BTStatus Update() override {
			while (true) {
				BTStatus status = (*current_child_)->Tick(BTContext{});
				if (status != BTStatus::kFailure) return status;

				++current_child_;
				if (current_child_ == children_.end()) {
					return BTStatus::kFailure;
				}
			}

			return BTStatus::kInvalid;	// ここには到達しないはずです
		}
	};

	class BTParallel : public BTComposite {
	public:
		enum Policy {
			RequireOne, // 1つの子ノードが成功すれば成功
			RequireAll  // 全ての子ノードが成功すれば成功
		};
		BTParallel(Policy success_policy, Policy failure_policy)
			: success_policy_(success_policy), failure_policy_(failure_policy) {
		}

	private:
		Policy success_policy_;
		Policy failure_policy_;

		virtual BTStatus Update() override {
			size_t i_success_count = 0, i_failure_count = 0;
			for(auto it: children_) {
				BTBehavior& behavior = **it;
				if (!behavior.IsTerminated()) behavior.Tick();

				if (behavior.GetStatus() == BTStatus::kSuccess) {
					i_success_count++;
					if (success_policy_ == RequireOne) 
						return BTStatus::kSuccess;
				} 
				if (behavior.GetStatus() == BTStatus::kFailure) {
					i_failure_count++;
					if (failure_policy_ == RequireOne) 
						return BTStatus::kFailure;
				}
			}

			if(failure_policy_ == RequireAll && i_failure_count == children_.size()) 
				return BTStatus::kFailure;
			
			if(success_policy_ == RequireAll && i_success_count == children_.size()) 
				return BTStatus::kSuccess;

			return BTStatus::kRunning;
		}

		virtual void OnInitialize() override {
			for(auto it: children_) {
				BTBehavior& behavior = **it;
				if(behavior.IsRunning) behavior.Abort();
			}
		}
	};

	struct BTMonitor : public BTParallel {
	public:
		void AddCondition(BTBehavior* condition) {
			children_.front(condition);
		}

		void AddAction(BTBehavior* action) {
			children_.push_back(action);
		}
	}

	class BTActiveSelector : public BTSelector {
		virtual BTStatus Update() override {
			BTBehaviors::iterator prev = current_child_;
			BTSelector::OnInitialize();
			BTStatus result = BTSelector::Update();
			if(prev != current_child_.end() && prev != current_child_) {
				(*prev)->Abort();
			}
			return result;
		}
	};
}
