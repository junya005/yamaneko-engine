/// <file>PrimitiveMesh.h</file>
/// <summary>
/// エンジンのビルトインメッシュを生成する関数の宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <vector>
#include "renderer/3d/Vertex.h"

/// <summary>
/// 基本的な図形のメッシュデータを生成する関数群を提供する名前空間です。
/// </summary>
namespace yamaneko_engine::PrimitiveMesh {
	/// <summary>
	/// キューブのメッシュデータを生成し、頂点配列とインデックス配列に書き込みます。
	/// </summary>
	/// <param name="vertices">生成されたキューブの頂点データを格納する配列への参照です。</param>
	/// <param name="indices">生成されたキューブのインデックスデータを格納する配列への参照です。</param>
	void GenerateCube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	/// <summary>
	/// UV球の頂点データとインデックスデータを生成します。
	/// </summary>
	/// <param name="vertices">生成した頂点を格納する配列への参照です。</param>
	/// <param name="indices">生成したインデックスを格納する配列への参照です。</param>
	/// <param name="radius">球の半径です。既定値は 0.5f です。</param>
	/// <param name="sectorCount">経度方向の分割数です。値を大きくすると横方向の滑らかさが増します。既定値は 36 です。</param>
	/// <param name="stackCount">緯度方向の分割数です。値を大きくすると縦方向の滑らかさが増します。既定値は 18 です。</param>
	void GenerateUVSphere(
		std::vector<Vertex>& vertices, 
		std::vector<unsigned int>& indices, 
		float radius = 0.5f, unsigned int 
		sectorCount = 36, 
		unsigned int stackCount = 18
	);

	/// <summary>
	/// 平面メッシュの頂点データとインデックスデータを生成します。
	/// </summary>
	/// <param name="vertices">生成した頂点を格納する `Vertex` 配列への参照です。</param>
	/// <param name="indices">生成したインデックスを格納する符号なし整数配列への参照です。</param>
	void GeneratePlane(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
}
