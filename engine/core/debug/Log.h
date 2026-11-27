/// <file>Log.h</file>
/// <summary>
/// ログに関する列挙子と構造体、関数の宣言と定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
/// 
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <cstddef>
#include <deque>
#include <format>
#include <iostream>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ログ出力を簡易的に行えるマクロの定義
#define LOG_TRACE(category, message) yamaneko_engine::Log::Write( \
			yamaneko_engine::Log::LogLevel::Trace, \
			category, \
			message + std::string(" [") + __FILE__ + ":" + std::to_string(__LINE__) + "]")

#define LOG_DEBUG(category, message) yamaneko_engine::Log::Write( \
			yamaneko_engine::Log::LogLevel::Debug, \
			category, \
			message + std::string(" [") + __FILE__ + ":" + std::to_string(__LINE__) + "]")

#define LOG_INFO(category, message) yamaneko_engine::Log::Write( \
			yamaneko_engine::Log::LogLevel::Info, \
			category, \
			message + std::string(" [") + __FILE__ + ":" + std::to_string(__LINE__) + "]")

#define LOG_WARNING(category, message) yamaneko_engine::Log::Write( \
			yamaneko_engine::Log::LogLevel::Warning, \
			category, \
			message + std::string(" [") + __FILE__ + ":" + std::to_string(__LINE__) + "]")

#define LOG_ERROR(category, message) yamaneko_engine::Log::Write( \
			yamaneko_engine::Log::LogLevel::Error, \
			category, \
			message + std::string(" [") + __FILE__ + ":" + std::to_string(__LINE__) + "]")

namespace yamaneko_engine::Log {
	/// <summary>
	/// ログのレベル
	/// </summary>
	enum class LogLevel {
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Critical
	};

	/// <summary>
	/// ログのレベル名を取得します
	/// </summary>
	/// <param name="level">ログのレベル</param>
	/// <returns>文字列</returns>
	inline const char* GetLogLevelName(LogLevel level) noexcept {
		switch (level) {
		case LogLevel::Trace:    return "TRACE";
		case LogLevel::Debug:    return "DEBUG";
		case LogLevel::Info:     return "INFO";
		case LogLevel::Warning:  return "WARN";
		case LogLevel::Error:    return "ERROR";
		case LogLevel::Critical: return "CRITICAL";
		}
		return "UNKNOWN";
	}

	/// <summary>
	/// ログの出力に必要なデータの構造体
	/// </summary>
	struct LogEntry {
		LogLevel level;
		std::string category;
		std::string message;
	};

	namespace{
		[[nondiscard]]
		bool ShouldLog(LogLevel level);

		[[nondiscard]]
		bool IsHistoryEnabled();

		void OutputLog(std::string_view log_level_name, std::string_view category, std::string_view message);

		void AppendHistoryUnlocked(LogEntry&& entry);
	}
	
	void SetMinimumLevel(LogLevel level);	
	void Initialize(std::size_t capacity = 2000);
	void Write(LogLevel level, std::string_view category, std::string_view message);
	template<class... Args>
	void Writef(LogLevel level, std::string_view category, std::format_string<Args...> format, Args&&... args);
	[[nodiscard]]
	std::vector<LogEntry> Snapshot();
	void Clear();
}
