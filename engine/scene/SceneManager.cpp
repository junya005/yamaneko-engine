/// <file>SceneManager.cpp</file>
/// <summary>
/// シーンマネージャークラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "SceneManager.h"
#include "Scene.h"
#include "core/debug/Log.h"

namespace yamaneko_engine::SceneManager {
	namespace {
		std::unordered_map<std::string, std::unique_ptr<Scene>> scene_list;
		std::unique_ptr<Scene> current_scene = nullptr;
		std::unique_ptr<Scene> next_scene = nullptr;
		bool has_pending_transition = false;
	}

	void Initialize(std::unique_ptr<Scene> initial_scene) {
		current_scene = std::move(initial_scene);
		next_scene.reset();
		has_pending_transition = false;
	}

	void Shutdown() {
		if (current_scene) {
			current_scene->Exit();
			current_scene.reset();
		}
		next_scene.reset();
		has_pending_transition = false;
	}

	template<typename T, typename... Args>
	void ChangeScene(Args&&... args) {
		RequestSceneChange(std::make_unique<T>(std::forward<Args>(args)...));
	}

	void ChangeScene(std::string scene_name) {
		RequestSceneChange(scene_name);
	}

	void RequestSceneChange(std::unique_ptr<Scene> new_scene) {
		next_scene = std::move(new_scene);
		has_pending_transition = true;
	}

	void RequestSceneChange(std::string scene_name) {
		auto it = std::find_if(scene_list.begin(), scene_list.end(), [&scene_name](const auto& pair) {
			return pair.second->GetName() == scene_name;
		});
		if (it != scene_list.end()) {
			next_scene = std::move(it->second);
			has_pending_transition = true;
		}
		else {
			Log::Write(Log::LogLevel::Error, "SceneManager", "Cancelled scene change. Scene not found: " + scene_name);
		}
	}

	void ProcessTransition() {
		if (!has_pending_transition) {
			return;
		}

		if (current_scene) {
			current_scene->Exit();
		}

		current_scene = std::move(next_scene);
		has_pending_transition = false;
		if (current_scene) {
			current_scene->Enter();
		}
	}

	void Update(float deltaTime) {
		// 遅延シーン遷移を適用
		ProcessTransition();

		if (current_scene) {
			current_scene->Update(deltaTime);
		}
	}

	void Render() {
		if (current_scene) {
			current_scene->Render();
		}
	}

	void OnUIRender() {
		if (current_scene) {
			current_scene->UIRender();
		}
	}

	Scene* GetCurrentScene() { 
		return current_scene.get(); 
	}

	void AddScene(std::unique_ptr<Scene> scene) {
		scene_list[scene->GetName()] = std::move(scene);
	}
}