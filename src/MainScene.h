#pragma once

#include <vector>
#include <memory>
#include <imgui.h>
#include <SDL2/SDL.h>
#include "../engine/scene/Scene.h"
#include "../engine/scene/Collision.h"
#include "../engine/platform/sdl/Window.h"
#include "../engine/platform/sdl/Input.h"
#include "../engine/core/math/Random.h"
#include "../engine/platform/sdl/Time.h"
#include "../engine/rhi/RHI.h"
#include "../engine/scene/module/Camera.h"
#include "Player.h"
#include "Debri.h"

using namespace yamaneko_engine;

class MainScene : public Scene {
public:
	MainScene() {
		RHIDevice* device = RHI::GetDevice();
		shader_ = device->CreateShader(
			"assets/shader/simple_vertex_shader.glsl", 
			"assets/shader/simple_fragment_shader.glsl"
		);

	}
	
	void OnEnter() override {
	
	}

	void OnExit() override {
	
	}

	void OnUpdate(float deltaTime) override {

	}

	void OnRender() override {
		
	}

	void OnUIRender() override {

	}

private:

};