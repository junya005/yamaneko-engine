/// <file>Mesh.cpp</file>
/// <summary>
/// メッシュに関するクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "Mesh.h"
#include "../../core/debug/Log.h"
#include "../../rhi/RHI.h"
#include <array>

namespace yamaneko_engine {
	Mesh::Mesh(
		std::vector<Vertex> vertices, 
		std::vector<unsigned int> indices, 
		std::vector<RHITexture*> textures
	) :
		vertices_(std::move(vertices)),
		indices_(std::move(indices)),
		textures_(std::move(textures))
	{
		SetUpMesh();
	}

	void Mesh::Draw(RHIShader& shader) {
		RHIDevice* device = RHI::GetDevice();
		if (device == nullptr) {
			Log::Write(Log::LogLevel::Error, "Mesh", "RHIDevice is not initialized.");
			return;
		}

		device->GetCommandList()->SetShader(&shader);

		unsigned int diffuse_nr = 1;
		unsigned int specular_nr = 1;
		bool diffuse_bound = false;

		for (unsigned int i = 0; i < textures_.size(); i++) {
			RHITexture* texture = textures_[i];
			if (!texture) continue;

			TextureType type = texture->GetType();
			std::string number = (type == TextureType::Diffuse) ? std::to_string(diffuse_nr++) : std::to_string(specular_nr++);

			if (type == TextureType::Diffuse && number == "1") {
				diffuse_bound = true;
			}

			shader.SetSampler2D((TextureTypeToString(type) + number).c_str(), i);
			device->GetCommandList()->SetTexture(i, texture);
		}

		// diffuse未設定時は白テクスチャを使う
		if (!diffuse_bound) {
			static RHITexture* white_texture = nullptr;
			if (white_texture == nullptr) {
				static const std::array<unsigned char, 4> white_pixel = { 255, 255, 255, 255 };
				white_texture = device->CreateTexture2D(1, 1, TextureType::Diffuse, white_pixel.data());
			}
			shader.SetSampler2D("texture_diffuse1", 0);
			device->GetCommandList()->SetTexture(0, white_texture);
		}

		device->GetCommandList()->SetVertexBuffer(vertex_buffer_.get());
		device->GetCommandList()->SetIndexBuffer(elements_buffer_.get());
		device->GetCommandList()->DrawIndexed(indices_.size());
	}

	void Mesh::SetUpMesh() {
		RHIDevice* device = RHI::GetDevice();
		if (device == nullptr) {
			Log::Write(Log::LogLevel::Error, "Mesh", "RHIDevice is not initialized.");
			return;
		}

		vertex_buffer_.reset(device->CreateVertexBuffer(vertices_.size() * sizeof(Vertex), vertices_.data()));
		elements_buffer_.reset(device->CreateIndexBuffer(indices_.size() * sizeof(unsigned int), indices_.data()));
	}
}
