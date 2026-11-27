/// <file>BTBlackBoard.h</file>
/// <summary>
/// BTBlackBoardの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once
#include <any>
#include <unordered_map>
#include <string>
#include "core/debug/Log.h"

namespace yamaneko_engine::ai {
	struct BTBlackBoardKey {
	public:
		std::string key;
		std::any value;

		BTBlackBoardKey() = default;
		BTBlackBoardKey(const std::string& k, const std::any& v) : key(k), value(v) {}
	};

	class BTBlackBoard {
	public:
		BTBlackBoard() = default;
		~BTBlackBoard() = default;

		template<typename T>
		void SetValue(const std::string& key, const T& value) {
			data_[key] = BTBlackBoardKey(key, value);
		}

		template<typename T>
		[[nodiscard]]T GetValue(const std::string& key) const {
			auto it = data_.find(key);
			if (it != data_.end()) {
				return std::any_cast<T>(it->second.value);
			}
			LOG_ERROR("BehaviorTree", "Key not found in blackboard: " + key);
			return T{};
		}

		template<typename T>
		[[nodiscard]]bool ConstainsValue(const std::string& key) const {
			auto it = data_.find(key);
			if (it != data_.end()) {
				return std::any_cast<T>(&it->second.value) != nullptr;
			}
			return false;
		}

		void EraseValue(const std::string& key) {
			data_.erase(key);
		}
	private:
		std::unordered_map<std::string, BTBlackBoardKey> data_;
	};
}