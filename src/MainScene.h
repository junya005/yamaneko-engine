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

using namespace yamaneko_engine;

class MainScene : public Scene {
public:
	MainScene() {
		RHIDevice* device = RHI::GetDevice();
		shader_ = device->CreateShader(
			"assets/shader/simple_vertex_shader.glsl", 
			"assets/shader/simple_fragment_shader.glsl"
		);

		auto camera = std::make_unique<GameObject>("Camera", "", shader_);
		camera->AddModule<Camera>();
		camera->transform.position = Vector3(0.0f, 0.0f, 10.0f);
		AddGameObject(std::move(camera));
	}
	
	void OnEnter() override {
	
	}

	void OnExit() override {
	
	}

	void OnUpdate(float deltaTime) override {

	}

	void OnRender() override {
		RHIDevice* device = RHI::GetDevice();

		Transform cube_transform;
		cube_transform.position = Vector3(0.0f, 0.0f, 0.0f);
		cube_transform.rotation = Vector3(0.0f, Time::GetTotalTime() * 0.5f, 0.0f);

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		PrimitiveMesh::GenerateCube(vertices, indices);

		auto command_list = device->GetCommandList();
		command_list->SetVertexBuffer(device->CreateVertexBuffer(vertices.size() * sizeof(Vertex), vertices.data()));
		command_list->SetIndexBuffer(device->CreateIndexBuffer(indices.size() * sizeof(unsigned int), indices.data()));

		shader_->SetMat4("model", cube_transform.GetModelMatrix());
		shader_->SetMat4("view", ViewInfo::current->GetViewMatrix());
		shader_->SetMat4("projection", ViewInfo::current->GetProjectionMatrix());

		static RHITexture* white_texture = nullptr;
		if (white_texture == nullptr) {
			static const std::array<unsigned char, 4> white_pixel = { 255, 255, 255, 255 };
			white_texture = device->CreateTexture2D(1, 1, TextureType::Diffuse, white_pixel.data());
		}
		shader_->SetSampler2D("texture_diffuse1", 0);
		device->GetCommandList()->SetTexture(0, white_texture);

		shader_->SetVec4("color", 0.0f, 1.0f, 0.0f, 1.0f);

		command_list->SetShader(shader_);
		command_list->DrawIndexed(indices.size());
	}

	void OnUIRender() override {

	}

private:
	RHIShader* shader_;

};
