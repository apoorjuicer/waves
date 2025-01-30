#pragma once

#include "AEEngine.h"

enum SonarDirection { UP, DOWN, LEFT, RIGHT };
constexpr size_t MAX_NUM_REFLECTIONS{ 6 };
constexpr size_t NUM_WALLS{ 5 };
constexpr f32 REFLECTIONS_MAX_SIZE{ 200.f };

struct wall_s {
	f32 x, y;
	f32 width;
	f32 height;

	bool reflected;

	AEMtx33 transform;
};

struct wave_s {
	f32 x, y;
	f32 current_size;	// radius
	f32 max_size;		// max radius

	bool alive;
	bool reflected;
	// int hp; (3counts)

	f32 growth_rate;
	AEGfxVertexList* mesh;

	f32 rotation;
	
};
void init_reflection_waves(wave_s(&reflection_waves)[MAX_NUM_REFLECTIONS], AEGfxVertexList* mesh);
void update_waves(struct wave_s& wave);
void draw_wave(struct wave_s const& wave, AEGfxVertexList const* mesh);

AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments);
// semicircle faces upwards;
AEGfxVertexList* Create_SemiCircle_Mesh(f32 outerRadius, f32 innerRadius, int numSegments);

bool wave_collision(struct wave_s wave, struct wall_s const wall);
void bounce_wave(struct wave_s& wave, struct wall_s const& wall, AEGfxVertexList* mesh);
void draw_bounce_wave(struct wave_s& wave, struct wall_s wall);

void draw_wall(struct wall_s wall, AEGfxVertexList* mesh);

void spawn_reflection_wave(struct wave_s &wave, struct wall_s const(&wall), struct wave_s(&reflections)[MAX_NUM_REFLECTIONS]);