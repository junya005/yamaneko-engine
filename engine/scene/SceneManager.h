/// <file>SceneManager.h</file>
/// <summary>
/// シーンマネージャークラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <memory>
#include "Scene.h"

namespace yamaneko_engine::SceneManager {
	/// <summary>
	/// シーンマネージャーを初期化します。
	/// </summary>
	/// <param name="initial_scene">初期シーン</param>
	void Initialize(std::unique_ptr<Scene> initial_scene);

	/// <summary>
	/// シーンマネージャーのシャットダウン処理を行います。
	/// </summary>
	void Shutdown();

	/// <summary>
	/// 現在シーンの更新処理を行います。
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	void Update(float deltaTime);

	/// <summary>
	/// 現在シーンの描画処理を行います。
	/// </summary>
	void Render();

	/// <summary>
	/// 現在シーンのUI描画処理を行います。
	/// </summary>
	void OnUIRender();

	/// <summary>
	/// クラス名からシーン変更を要求します。
	/// </summary>
	/// <typeparam name="T">遷移先クラスの型</typeparam>
	template<typename T, typename... Args>
	void ChangeScene(Args&&... args);

	/// <summary>
	/// 文字列で指定されたシーン名に基づいてシーン変更を要求します。
	/// </summary>
	/// <param name="scene_name">変更先のシーン名です。</param>
	void ChangeScene(std::string scene_name);

	/// <summary>
	/// 新しいシーンへの切り替えを要求します。
	/// </summary>
	/// <param name="new_scene">切り替え先のシーンを所有権付きで保持する `std::unique_ptr<Scene>` です。</param>
	void RequestSceneChange(std::unique_ptr<Scene> new_scene);

	/// <summary>
	/// 文字列で指定されたシーン名に基づいて新しいシーンへの切り替えを要求します。
	/// </summary>
	/// <param name="scene_name">変更先のシーン名です。</param>
	void RequestSceneChange(std::string scene_name);

	/// <summary>
	/// 現在のシーンを取得します。
	/// </summary>
	/// <returns>現在のシーンへのポインタです。</returns>
	Scene* GetCurrentScene();

	/// <summary>
	/// シーンをシーン管理に追加します。
	/// </summary>
	/// <param name="scene">追加する Scene オブジェクトの一意所有ポインタです。関数呼び出し後、所有権は受け取り側に移動します。</param>
	void AddScene(std::unique_ptr<Scene> scene);
}
