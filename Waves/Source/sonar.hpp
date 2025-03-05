#pragma once

#include "AEEngine.h"

//=============================================================================
// const expressions for functions;
//=============================================================================
constexpr size_t MAX_NUM_WAVES{ 6 };
constexpr size_t MAX_NUM_OUTLINES{ 100 };
constexpr size_t MAX_LINE_LENGTH{ 100 };
constexpr size_t NUM_WAVE_MESHES{ 6 };

constexpr f32 WAVES_SIZE{ 100.f };
constexpr f32 REFLECTIONS_MAX_SIZE{ 120.f };

enum LineType { Horizontal, Vertical, Corner };
enum LineOrigin { BottomLeft, TopLeft, BottomRight, TopRight, OriginH, OriginV};

//=============================================================================
// line struct;
//=============================================================================
struct line_s {
	f32 x, y;
	f32 width, height;
	f32 growth_rate;

	s16 type;
	f32 rotation;
	bool alive;

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
// draw wave
void draw_wave(struct wave_s(&wave)[MAX_NUM_WAVES]);
// spawn main wave at rock location and initialize;
void spawn_wave(struct wave_s(&MainWaves)[MAX_NUM_WAVES], f32 x, f32 y, AEGfxVertexList* mesh);

//=============================================================================
// reflection waves
//=============================================================================
void reflections_spawner(wave_s wave, int const (&
	
	
	)[10][10], f32 tilesize, struct wave_s(&reflections)[MAX_NUM_WAVES], struct line_s(&line)[MAX_NUM_OUTLINES], AEGfxVertexList* (&mesh)[NUM_WAVE_MESHES]);
// check collision with wall; circle rect collision;
bool wave_collision(struct wave_s wave, int index_x, int index_y, float tileSize);
// make a reflection wave come alive;
void init_reflection_waves(wave_s(&reflection_waves)[MAX_NUM_WAVES], AEGfxVertexList* mesh);
// init array of reflection waves; use before game loop;
void spawn_reflection_wave(struct wave_s& wave, int index_x, int index_y, float tileSize, struct wave_s(&reflections)[MAX_NUM_WAVES], AEGfxVertexList* mesh);

//=============================================================================
// outlines
//=============================================================================
void update_outlines(struct line_s& line);
void draw_outlines(struct line_s& line, AEGfxVertexList* (&mesh)[NUM_WAVE_MESHES]);
void spawn_outlines(struct wave_s const& wave, f32 posX, f32 posY, f32 tilesize, struct line_s (&line)[MAX_NUM_OUTLINES], int origin);

//=============================================================================
// create meshes for waves and outlines
//=============================================================================
AEGfxVertexList* Create_SemiCircle_Mesh(f32 outerRadius, f32 innerRadius, int numSegments, int divisor = 3);
AEGfxVertexList* CreateWaveMesh(f32 outerRadius, f32 innerRadius, int numSegments);
AEGfxVertexList* Create_LineMesh(bool skew);

