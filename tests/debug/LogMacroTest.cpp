#include <gtest/gtest.h>
#include "core/debug/Log.h"

using namespace yamaneko_engine::Log;

TEST(LogMacroTest, LogTraceMacro) {
	LOG_TRACE("TestCategory", "This is a trace log message.");
}

TEST(LogMacroTest, LogDebugMacro) {
	LOG_DEBUG("TestCategory", "This is a debug log message.");
}

TEST(LogMacroTest, LogInfoMacro) {
	LOG_INFO("TestCategory", "This is an info log message.");
}

TEST(LogMacroTest, LogWarningMacro) {
	LOG_WARNING("TestCategory", "This is a warning log message.");
}

TEST(LogMacroTest, LogErrorMacro) {
	LOG_ERROR("TestCategory", "This is an error log message.");
}