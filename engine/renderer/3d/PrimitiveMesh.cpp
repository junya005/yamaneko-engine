/// <file>PrimitiveMesh.cpp</file>
/// <summary>
/// エンジンのビルトインメッシュを生成する関数の定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "PrimitiveMesh.h"
#include "core/math/Math.h"

namespace yamaneko_engine::PrimitiveMesh {
	void GenerateCube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
		for(int i = 0; i < 24; ++i) {
			Vertex vertex;
			switch(i / 4) {
				case 0: // Front face
					vertex.position = Vector3(-0.5f + (i % 2), -0.5f + ((i / 2) % 2), 0.5f);
					vertex.normal = Vector3(0.0f, 0.0f, 1.0f);
					break;
				case 1: // Back face
					vertex.position = Vector3(0.5f - (i % 2), -0.5f + ((i / 2) % 2), -0.5f);
					vertex.normal = Vector3(0.0f, 0.0f, -1.0f);
					break;
				case 2: // Left face
					vertex.position = Vector3(-0.5f, -0.5f + (i % 2), -0.5f + ((i / 2) % 2));
					vertex.normal = Vector3(-1.0f, 0.0f, 0.0f);
					break;
				case 3: // Right face
					vertex.position = Vector3(0.5f, -0.5f + (i % 2), -0.5f + ((i / 2) % 2));
					vertex.normal = Vector3(1.0f, 0.0f, 0.0f);
					break;
				case 4: // Top face
					vertex.position = Vector3(-0.5f + (i % 2), 0.5f, -0.5f + ((i / 2) % 2));
					vertex.normal = Vector3(0.0f, 1.0f, 0.0f);
					break;
				case 5: // Bottom face
					vertex.position = Vector3(-0.5f + (i % 2), -0.5f, -0.5f + ((i / 2) % 2));
					vertex.normal = Vector3(0.0f, -1.0f, 0.0f);
					break;
			}
			vertex.tex_coords = Vector2((float)(i % 2), (float)((i / 2) % 2));
			vertices.push_back(vertex);
		}

		indices = {
			0, 1, 2, 2, 1, 3,       // Front face
			4, 5, 6, 6, 5, 7,       // Back face
			8, 9, 10, 10, 9, 11,    // Left face
			12, 13, 14, 14, 13, 15, // Right face
			16, 17, 18, 18, 17, 19, // Top face
			20, 21, 22, 22, 21, 23  // Bottom face
		};
	}

	void GenerateUVSphere(
		std::vector<Vertex>& vertices, 
		std::vector<unsigned int>& indices, 
		float radius,
		unsigned int sectorCount, 
		unsigned int stackCount
	) {
		for(int i = 0; i <= stackCount; ++i) {
			float stackAngle = Math::PI / 2 - i * Math::PI / stackCount;
			float xy = radius * Math::Cos(stackAngle);
			float z = radius * Math::Sin(stackAngle);
			for(int j = 0; j <= sectorCount; ++j) {
				float sectorAngle = j * 2 * Math::PI / sectorCount;
				Vertex vertex;
				vertex.position.x = xy * Math::Cos(sectorAngle);
				vertex.position.y = xy * Math::Sin(sectorAngle);
				vertex.position.z = z;
				vertex.normal = Math::Normalize(vertex.position);
				vertex.tex_coords.x = (float)j / sectorCount;
				vertex.tex_coords.y = (float)i / stackCount;
				vertices.push_back(vertex);
			}
		}

		for(int i = 0; i < stackCount; ++i) {
			int k1 = i * (sectorCount + 1);
			int k2 = k1 + sectorCount + 1;
			for(int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
				if(i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				if(i != (stackCount - 1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}

	void GeneratePlane(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
		vertices = {
			{{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{ 0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{ 0.5f, 0.0f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.0f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
		};

		indices = {
			0, 1, 2,
			2, 3, 0
		};
	}
}
