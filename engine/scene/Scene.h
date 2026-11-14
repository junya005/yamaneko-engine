/// <file>Scene.h</file>
/// <summary>
/// シーンクラスの宣言。
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
#include "GameObject.h"

namespace yamaneko_engine {
	class Scene {
	public:
		Scene(std::string name) : name_(name) {};

		~Scene() {
			OnDestory();
		};
	
		/// <summary>
		/// ゲームオブジェクトをシーンに追加します。
		/// </summary>
		/// <param name="game_object">追加する GameObject オブジェクトの一意所有ポインタです。関数呼び出し後、所有権は受け取り側に移動します。</param>
		void AddGameObject(std::unique_ptr<GameObject> game_object) {
			game_objects_.push_back(std::move(game_object));
		}

		/// <summary>
		/// ゲームオブジェクトをシーンから削除します。
		/// </summary>
		/// <param name="game_object">削除する GameObject オブジェクトへのポインタです。</param>
		void RemoveGameObject(GameObject* game_object) {
			game_objects_.erase(
				std::remove_if(
					game_objects_.begin(),
					game_objects_.end(),
					[game_object](const std::unique_ptr<GameObject>& obj) {
						return obj.get() == game_object;
					}
				),
				game_objects_.end()
			);
		}

		/// <summary>
		/// 条件に一致するゲームオブジェクトをシーンから削除します。
		/// </summary>
		/// <param name="predicate">削除するゲームオブジェクトを判定する述語関数です。</param>
		void RemoveGameObjects(std::function<bool(const std::unique_ptr<GameObject>&)> predicate) {
			game_objects_.erase(
				std::remove_if(
					game_objects_.begin(),
					game_objects_.end(),
					predicate
				),
				game_objects_.end()
			);
		}

		/// <summary>
		/// 指定されたタグを持つゲームオブジェクトをシーンから削除します。
		/// </summary>
		/// <param name="tag">削除するゲームオブジェクトのタグです。</param>
		void RemoveGameObjectByTag(const std::string& tag) {
			game_objects_.erase(
				std::remove_if(
					game_objects_.begin(),
					game_objects_.end(),
					[&tag](const std::unique_ptr<GameObject>& obj) {
						return obj->tag == tag;
					}
				),
				game_objects_.end()
			);
		}

		/// <summary>
		/// 指定されたタグを持つすべてのゲームオブジェクトを取得します。
		/// </summary>
		/// <param name="tag">取得するゲームオブジェクトのタグです。</param>
		/// <returns>指定されたタグを持つゲームオブジェクトのポインタのベクターです。</returns>
		std::vector<GameObject*> GetAllGameObjectsByTag(const std::string& tag) {
			std::vector<GameObject*> result;
			for (auto& game_object : game_objects_) {
				if (game_object && game_object->tag == tag) {
					result.push_back(game_object.get());
				}
			}
			return result;
		}

		/// <summary>
		/// シーンに入るときに呼び出されます。
		/// このメソッドは自動的に呼び出されるため、ユーザーが直接呼び出す必要はありません。
		/// </summary>
		void Enter() {
			OnEnter();
		}

		/// <summary>
		/// 終了処理を実行します。
		/// このメソッドは自動的に呼び出されるため、ユーザーが直接呼び出す必要はありません。
		/// </summary>
		void Exit() {
			OnExit();
		}

		/// <summary>
		/// すべてのゲームオブジェクトを更新し、その後に追加の更新処理を実行します。
		/// このメソッドは自動的に呼び出されるため、ユーザーが直接呼び出す必要はありません。
		/// </summary>
		/// <param name="deltaTime">前回の更新からの経過時間（通常は秒単位）です。各オブジェクトの更新処理に渡されます。</param>
		void Update(float deltaTime) {
			for (auto& game_object : game_objects_) {
				if (game_object) {
					game_object->Update(deltaTime);
				}
			}

			OnUpdate(deltaTime);
		}

		/// <summary>
		/// 保持しているすべてのゲームオブジェクトを描画し、その後に追加のレンダリング処理を実行します。
		/// </summary>
		void Render() {
			for (auto& game_object : game_objects_) {
				if (game_object) {
					game_object->Draw();
				}
			}
			OnRender();
		}

		/// <summary>
		/// UIの描画処理を実行します。
		/// </summary>
		void UIRender() {
			OnUIRender();
		}

		/// <summary>
		/// シーンの名前を取得します。
		/// </summary>
		/// <returns>シーンの名前。</returns>
		std::string GetName() const { return name_; }

	protected:
		virtual void OnEnter() {}
		virtual void OnExit() {}
		virtual void OnUpdate(float deltaTime) { (void)deltaTime; }
		virtual void OnRender() {}
		virtual void OnUIRender() {}
		virtual void OnDestory() {}

	private:
		std::string name_;
		std::vector<std::unique_ptr<GameObject>> game_objects_;
	};
}
