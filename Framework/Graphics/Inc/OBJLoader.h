#pragma once

#include"Mesh.h"

namespace SB::Graphics
{
	class OBJLoader
	{
	public:
		static void Load(const std::filesystem::path& filePath,float scale, Mesh& mesh);
	};
}