#ifndef WAV_HPP
#define WAV_HPP

#include "AEEngine.h"
#include <utility>

constexpr int MAZE_HEIGHT{ 10 };
constexpr int MAZE_WIDTH{ 10 };

//==================================================================================================================
// CONST EXPRESSIONS;
//==================================================================================================================
constexpr f32 WAVES_SIZE{ 150.f };
constexpr f32 REFLECTIONS_MAX_SIZE{ 80.f };
constexpr f32 GROWTH_R{ 50.f };

constexpr size_t MAX_NUM_WAVES{ 10 };
constexpr size_t MAX_NUM_REFLECTIONS{ 10 };
constexpr size_t MAX_NUM_OUTLINES{ 20 };

constexpr size_t NUM_WAVE_MESHES{ 3 };

//==================================================================================================================
// ENUMS;
//==================================================================================================================
enum TYPE {

	TYPE_WALL = 66,
	TYPE_WAVE,
	TYPE_REFLECTION,
	TYPE_OUTLINE,

	TYPE_NULL

};

enum DIRECTION{

	DIR_LEFT = 77,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,

	DIR_NULL

};

// prefix overload
DIRECTION& operator++(DIRECTION& dir);
DIRECTION operator-(DIRECTION& dir);
//static std::ostream& operator<<(std::ostream& os, DIRECTION& dir);

//==================================================================================================================
// CLASS DECLARATIONS;
//==================================================================================================================
struct GameObj {

	AEGfxVertexList**	mesh;
	unsigned long		type;
	bool				is_active;
	
	AEVec2				pos;
	AEVec2				scale;
	f32					rot;

	GameObj();
};

struct Outline : public GameObj {
	f32 max_len;
	f32 growth_rate;
	f32 lifespan;

	Outline();

	void spawn(AEVec2 pos_in, f32 max_in, f32 grow_r, AEGfxVertexList** mesh_in, DIRECTION dir_in);
	void update();
	void draw();

};

class Wave : public GameObj {
protected:
	f32	max_size;

public:
	f32	growth_rate;
	short r_collision[8]{};	// left, right, up, down;

	Wave();

	void spawn(AEVec2 pos_in, AEGfxVertexList** mesh_in, f32 max_in, f32 grow_r = GROWTH_R, f32 size_in = 0.f, DIRECTION = DIR_NULL);
	void update();
	void draw();

	void echolocation(int const(&maze)[MAZE_HEIGHT][MAZE_WIDTH], f32 tilesize, Wave(&reflections)[MAX_NUM_REFLECTIONS], Outline(&outlines)[MAX_NUM_OUTLINES]);
	
};

//==================================================================================================================
// FUNCTION DECLARATIONS;
//==================================================================================================================
// find location of reflection waves;
AEVec2 locate_reflection(AEVec2 wave_pos, AEVec2 wall_pos, f32 wall_size);
// finds x and y index of wave with respect to maze and size of each tile;
std::pair<int, int> find_wave_index(int const(&maze)[MAZE_HEIGHT][MAZE_WIDTH], AEVec2 wpos, f32 tilesize);
// interface for spawning wave;
void spawn_wave(Wave(&arr)[MAX_NUM_WAVES], f32 pos_x, f32 pos_y, AEGfxVertexList** mesh_in, f32 max_size = WAVES_SIZE);

void create_two_outlines(Outline(&outlines)[MAX_NUM_OUTLINES], AEVec2 pos_in, AEVec2 maxlen_in,
	std::pair<DIRECTION, DIRECTION> dir_pair, AEGfxVertexList** mesh_in, f32 g_rate);

//==================================================================================================================
// TEMPLATES
//==================================================================================================================
template <typename T>
T clamp(T value, T min, T max) {
	if (value < min) return min;    // If value is less than min, return min
	if (value > max) return max;    // If value is greater than max, return max
	return value;                   // Otherwise, return the value itself
}
template <typename T>
T abs(T num) {
	return (num < 0) ? -num : num;
}


//==================================================================================================================
// create meshes for waves and outlines
//==================================================================================================================
AEGfxVertexList* Create_SemiCircle_Mesh(f32 outerRadius, f32 innerRadius, int numSegments, int divisor = 3);
AEGfxVertexList* CreateWaveMesh(f32 outerRadius, f32 innerRadius, int numSegments);
AEGfxVertexList* Create_LineMesh();

#endif