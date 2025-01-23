#pragma once

enum SonarDirection { UP, DOWN, LEFT, RIGHT };

typedef struct CircleSonar_c {
	f32 posx, posy;

	f32 speed = 50.0f;
	f32 maxSize = 150.f;
	f32 currentSize = 0.f;

	AEMtx33 transform;
};
namespace sonar {

	AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments);

	void CircleSonar(struct CircleSonar_c& Wave, AEGfxVertexList* mesh, int& active);

}
