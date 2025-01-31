#pragma once

#include "AEEngine.h"

//=============================================================================
// const expressions for functions;
//=============================================================================
constexpr size_t MAX_NUM_REFLECTIONS{ 6 };
constexpr size_t MAX_NUM_OUTLINES{ 10 };
constexpr size_t MAX_LINE_LENGTH{ 250 };

constexpr size_t NUM_WALLS{ 5 };

constexpr f32 REFLECTIONS_MAX_SIZE{ 200.f };

enum LineType { Left, Right, Top, Bottom };

//=============================================================================
// temporary testing struct for wall
//=============================================================================
struct line_s {
	f32 x, y;
	f32 width, height;
	f32 growth_rate;

	//f32 maxwidth, maxheight;

	s16 type;
	bool alive;

	//f32 rotation;
};

struct wall_s {
	f32 x, y;
	f32 width;
	f32 height;

	bool reflected;

	AEMtx33 transform;
};
//=============================================================================
// wave struct; used for both main and reflection waves;
//=============================================================================
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
//=============================================================================
// general wave functions
//=============================================================================
// general update and draw; can be used for both main waves and reflections;
void update_waves(struct wave_s& wave);
void draw_wave(struct wave_s const& wave, AEGfxVertexList const* mesh);

//=============================================================================
// reflection waves
//=============================================================================
// check collision with wall; circle rect collision;
bool wave_collision(struct wave_s wave, struct wall_s const wall);
// make a reflection wave come alive;
void init_reflection_waves(wave_s(&reflection_waves)[MAX_NUM_REFLECTIONS], AEGfxVertexList* mesh);
// init array of reflection waves; use before game loop;
void spawn_reflection_wave(struct wave_s &wave, struct wall_s const(&wall), struct wave_s(&reflections)[MAX_NUM_REFLECTIONS]);

//=============================================================================
// outlines
//=============================================================================
void update_outlines(struct line_s& line);
void draw_outlines(struct line_s& line, AEGfxVertexList* mesh);
//void init_outlines(struct line_s(&line)[MAX_NUM_OUTLINES]);
void spawn_outlines(struct wave_s const& wave, struct wall_s const(&wall), struct line_s (&line)[MAX_NUM_OUTLINES]);

//=============================================================================
// temporary testing functions
//=============================================================================
void draw_wall(struct wall_s wall, AEGfxVertexList* mesh);

//=============================================================================
// temp meshes for testing
//=============================================================================
AEGfxVertexList* Create_SemiCircle_Mesh(f32 outerRadius, f32 innerRadius, int numSegments);
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments);

