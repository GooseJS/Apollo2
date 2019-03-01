#pragma once

#include <Apollo/GL.h>

namespace Apollo
{
	struct ChunkMesh // TODO: This class is very subject to change. I don't like just having this as a storage / data class. I'm probably gonna move ChunkMeshGeneration into this
	{
		bool init = false;

		int numVertices = 0;
		GLuint vaoID = 0;
		
		bool needsUpdate = true;
	};
}