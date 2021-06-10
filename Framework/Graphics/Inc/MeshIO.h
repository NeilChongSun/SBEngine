#pragma once
#include "Mesh.h"

namespace SB::Graphics::MeshIO
{
	void Write(FILE* file, const SkinneMesh& mesh);
	void Read(FILE* file, SkinneMesh& mesh);
}

//For homework: Add MeshIO.cpp implement Write/Read