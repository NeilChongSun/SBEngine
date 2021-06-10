#include"Precompiled.h"
#include"MaterialIO.h"

using namespace SB::Graphics;

void MaterialIO::Write(FILE* file, const Material& material)
{
	fprintf_s(file, "Color\n");
	fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n",
		material.ambient.r, material.ambient.g, material.ambient.b, material.ambient.w,
		material.diffuse.r, material.diffuse.g, material.diffuse.b, material.diffuse.w,
		material.specular.r, material.specular.g, material.specular.b, material.specular.w,
		material.power,
		material.padding[0], material.padding[1], material.padding[2]);
}

void MaterialIO::Read(FILE* file, Material& material)
{
	fscanf_s(file, "Color\n");
	fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n",
		&material.ambient.r, &material.ambient.g, &material.ambient.b, &material.ambient.w,
		&material.diffuse.r, &material.diffuse.g, &material.diffuse.b, &material.diffuse.w,
		&material.specular.r, &material.specular.g, &material.specular.b, &material.specular.w,
		&material.power,
		&material.padding[0], &material.padding[1], &material.padding[2]);
}

