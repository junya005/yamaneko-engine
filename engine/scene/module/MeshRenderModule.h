/// <file>MeshRenderModule.h</file>
/// <summary>
/// メッシュレンダリングモジュールクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "scene/module/Module.h"

namespace yamaneko_engine {
	class MeshRenderModule : public Module {
	public:
		MeshRenderModule() = default;
		
	protected:
		void OnInitialize() override;
		void OnUpdate(float delta_time) override;
		void OnRender() override;

	private:

	};
}
