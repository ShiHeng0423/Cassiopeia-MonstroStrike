#include "AEEngine.h"
#include "Utils.h"

AEGfxVertexList* GenerateSquareMesh(u32 MeshColor)
{
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, MeshColor, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, MeshColor, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, MeshColor, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, MeshColor, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, MeshColor, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, MeshColor, 0.0f, 0.0f);  // top-left: blue

	return AEGfxMeshEnd();
}

AEGfxVertexList* GenerateLineMesh(u32 MeshColor)
{
	AEGfxMeshStart();

	AEGfxVertexAdd(-0.5f, 0.f, MeshColor, 1.0f, 1.0f);
	AEGfxVertexAdd(0.5f, 0.f, MeshColor, 1.0f, 1.0f);

	return AEGfxMeshEnd();
}