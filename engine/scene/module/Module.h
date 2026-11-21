/// <file>Module.h</file>
/// <summary>
/// モジュールクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>


#pragma once

#include <vector>
#include <memory>
#include "sub/SubModule.h"

namespace yamaneko_engine {
	class GameObject;
    struct Transform;
    class RHIShader;

	class Module {
	public:
		virtual ~Module() = default;
		void Initialize();
		void Update(float delta_time);
		void Render();

		void SetOwner(GameObject* owner) { owner_ = owner; }

		template<typename T, typename... Args>
		T* AddSubModule(Args&&... args) {
			auto sub_module = std::make_unique<T>(std::forward<Args>(args)...);
			T* sub_module_ptr = sub_module.get();
			sub_module_ptr->SetParentModule(this);
			sub_modules_.push_back(std::move(sub_module));
			sub_module_ptr->Initialize();
			return sub_module_ptr;
		}

		/// <summary>
		/// サブモジュール一覧から、指定した型のサブモジュールを取得します。
		/// </summary>
		/// <typeparam name="T">取得したいサブモジュールの型です。</typeparam>
		/// <returns>指定した型に一致する最初のサブモジュールへのポインタを返します。見つからない場合は nullptr を返します。</returns>
		template<typename T>
		T* GetSubModule() {
			for (auto& sub_module : sub_modules_) {
				if (T* casted_sub_module = dynamic_cast<T*>(sub_module.get())) {
					return casted_sub_module;
				}
			}
			return nullptr;
		}

		GameObject* GetGameObject() const;
		Transform& GetTransform() const;
		RHIShader* GetShader() const;

	protected:
		virtual void OnInitialize() {};
		virtual void OnUpdate(float delta_time) {};
		virtual void OnRender() {};

	private:
		bool is_active_ = true;
		GameObject* owner_ = nullptr;
		std::vector<std::unique_ptr<SubModule>> sub_modules_;
	};
}