/// <file>Time.h</file>
/// <summary>
/// 時間に関する機能を持ったクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <cstdint>

namespace yamaneko_engine::Time {
	void Initialize();
	void Update();
	float GetDeltaTime();
	float GetTotalTime();
	uint64_t GetFrameCount();
	void SetMaxDeltaTime(float maxDeltaTime);
	float GetMaxDeltaTime();
}
