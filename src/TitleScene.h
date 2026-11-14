#pragma once

#include <vector>
#include <memory>
#include <imgui.h>
#include <SDL2/SDL.h>
#include <array>
#include "../engine/scene/Scene.h"
#include "../engine/scene/Collision.h"
#include "../engine/platform/sdl/Window.h"
#include "../engine/platform/sdl/Input.h"
#include "../engine/core/math/Random.h"
#include "../engine/platform/sdl/Time.h"
#include "../engine/rhi/RHI.h"
#include "../engine/scene/module/Camera.h"
#include "../engine/renderer/3d/PrimitiveMesh.h"
#include "../engine/scene/SceneManager.h"
#include "MainScene.h"
#include "../engine/core/debug/Log.h"

using namespace yamaneko_engine;

class TitleScene : public Scene {
public:
	TitleScene() : Scene("TitleScene") {
		
	}
	
	void OnEnter() override {

	}

	void OnExit() override {

	}

	void OnUpdate(float deltaTime) override {
		if(Input::GetKeyDown(SDLK_SPACE)) {
			SceneManager::ChangeScene("MainScene");
		}
	}

	void OnRender() override {

	}

	void OnUIRender() override {
		ImGuiWindowFlags canvas_flags_ = ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_NoInputs;

		if(ImGui::Begin("Title Scene", nullptr, canvas_flags_)) {
			ImGui::Text("Welcome to the Title Scene!\nPress space key to continue...");
		}
		ImGui::End();
	}
};
