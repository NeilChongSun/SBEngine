#pragma once
#include "Material.h"

namespace SB::Graphics::MaterialIO
{
	void Write(FILE* file, const Material& material);
	void Read(FILE* file, Material& material);
}