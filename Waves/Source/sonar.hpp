#pragma once

#include "AEEngine.h"

enum SonarDirection { UP, DOWN, LEFT, RIGHT };

struct wall_s {
	f32 x, y;
	f32 width;
	f32 height;

	AEMtx33 transform;
};

struct wave_s {
	s32 x, y;
	f32 current_size;	// radius
	f32 max_size;		// max radius

	bool alive;
	// int hp; (3counts)

	f32 growth_rate;
	AEGfxVertexList* mesh;
	AEMtx33 transform;
};

void update_wave_size(struct wave_s& wave);
void draw_wave(struct wave_s& wave);

AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments);
// semicircle faces upwards;
AEGfxVertexList* Create_SemiCircle_Mesh(f32 outerRadius, f32 innerRadius, int numSegments);

bool wave_collision(struct wave_s wave, struct wall_s wall);
void bounce_wave(struct wave_s& wave, struct wall_s const& wall, AEGfxVertexList* mesh);
void draw_bounce_wave(struct wave_s& wave, struct wall_s wall);

void draw_wall(struct wall_s wall, AEGfxVertexList* mesh);