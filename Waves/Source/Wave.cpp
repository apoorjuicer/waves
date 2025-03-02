#include "Wave.hpp"
#include <iostream>

namespace {

	f32 set_rotation(DIRECTION dir_in) {
		f32 rot{};

		switch (dir_in) {
			case DIR_LEFT:
				rot = 90;
				break;
			case DIR_RIGHT:
				rot = -90;
				break;
			case DIR_DOWN:
				rot = 180;
				break;
			default:
				rot = 0;
		}

		return rot;
	}

	AEMtx33 set_transform(AEVec2 scale_in, AEVec2 trans_in, f32 rot_in) {
		AEMtx33 scale = { 0 }; AEMtx33 translate = { 0 }; AEMtx33 rotate = { 0 }; AEMtx33 transform = { 0 };

		AEMtx33RotDeg(&rotate, rot_in);
		AEMtx33Scale(&scale, scale_in.x, scale_in.y);
		AEMtx33Trans(&translate, trans_in.x, trans_in.y);

		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		return transform;
	}

	bool point_rect_collision(f32 x, f32 y, f32 rx, f32 ry, f32 width, f32 height) {
		f32 left = rx - width / 2.f;
		f32 right = rx + width / 2.f;
		f32 bot = ry - height / 2.f;
		f32 top = ry + height / 2.f;

		return ((x >= left && x <= right) && (y <= top && y >= bot)) ? true : false;
	}

	bool circle_square_collision(AEVec2 cpos, f32 rad, AEVec2 rpos, f32 rscale) {
		// nearest point in rectangle to circle;
		f32 near_x = clamp(cpos.x, rpos.x - rscale / 2.f, rpos.x + rscale / 2.f);
		f32 near_y = clamp(cpos.y, rpos.y - rscale / 2.f, rpos.y + rscale / 2.f);

		// Calculate the distance from the circle center to this closest point;
		f32 dx = cpos.x - near_x;
		f32 dy = cpos.y - near_y;

		// If the distance is less than or equal to the circle's radius, they collide;
		f32 distanceSquared = (dx * dx) + (dy * dy);
		f32 radiusSquared = (rad) * (rad);

		// return result;
		return (distanceSquared <= radiusSquared) ? true : false;
	}
}

DIRECTION& operator++(DIRECTION& dir) {

	switch (dir) {
	case (DIR_LEFT):
		dir = DIR_RIGHT;
		break;
	case (DIR_RIGHT):
		dir = DIR_UP;
		break;
	case (DIR_UP):
		dir = DIR_DOWN;
		break;
	case (DIR_DOWN):
		dir = DIR_LEFT;
		break;
	default:
		dir = DIR_NULL;
	}

	return dir;

}
DIRECTION operator-(DIRECTION& dir) {

	switch (dir) {
	case (DIR_LEFT):
		dir = DIR_RIGHT;
		break;
	case (DIR_RIGHT):
		dir = DIR_LEFT;
		break;
	case (DIR_UP):
		dir = DIR_DOWN;
		break;
	case (DIR_DOWN):
		dir = DIR_UP;
		break;
	default:
		dir = DIR_NULL;
	}

	return dir;
}
std::ostream& operator<<(std::ostream& os, DIRECTION& dir) {

	switch (dir) {
	case (DIR_LEFT):
		os << "Direction Left";
		break;
	case (DIR_RIGHT):
		os << "Direction Right";
		break;
	case (DIR_UP):
		os << "Direction Up";
		break;
	case (DIR_DOWN):
		os << "Direction Down";
		break;
	default:
		os << "Direction Null";
	}

	return os;

}

//==================================================================================================================
// CLASS CONSTRUCTORS;
//==================================================================================================================
GameObj::GameObj() : mesh{ nullptr }, type{ TYPE_NULL }, is_active{ false }, 
					pos{0.f,0.f}, scale{0.f, 0.f}, rot{0.f} { /*empty by design*/ }

Wave::Wave() : max_size{ WAVES_SIZE }, growth_rate{ GROWTH_R } { /*empty by design*/ }

//==================================================================================================================
// WAVE MEMBER FUNCTIONS;
//==================================================================================================================
void Wave::spawn(AEVec2 pos_in, AEGfxVertexList** mesh_in, f32 max_in, f32 grow_r, f32 size_in, DIRECTION dir_in) {

	// GameObj variables;
	mesh = mesh_in;
	type = TYPE_WAVE;
	is_active = true;

	pos = pos_in;
	rot = set_rotation(dir_in);

	scale.x = scale.y = size_in;

	// Wave variables;
	max_size = max_in;
	growth_rate = grow_r;

	return;

}

void Wave::update() {

	// quick exit for non-active wave;
	if (!is_active) {
		return;
	}

	// check if wave has grown to max_size;
	if (scale.x >= max_size || scale.y >= max_size) {
		is_active = false;

		scale.x = 0.f;
		growth_rate = GROWTH_R;
		
		for (short& i : r_collision) {
			i = 0;
		}

		return;
	}

	// update wave size;
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	scale.x += growth_rate * dt;
	scale.y += growth_rate * dt;
	growth_rate *= 1.05f;

	return;

}

void Wave::draw() {

	// quick exit for non-active wave;
	if (!is_active) {
		return;
	}
	// set transforms;
	AEMtx33 transform = set_transform(scale, pos, rot);
	AEGfxSetTransform(transform.m);
	// draw wave;
	AEGfxMeshDraw(*mesh, AE_GFX_MDM_TRIANGLES);

	return;

}

// update collision check array in Wave object; spawns reflections;
void Wave::echolocation(int const(&maze)[MAZE_HEIGHT][MAZE_WIDTH], f32 tilesize, Wave(&reflections)[MAX_NUM_REFLECTIONS]) {

	for (int y{}; y < MAZE_HEIGHT; ++y) {
		for (int x{}; x < MAZE_WIDTH; ++x) {

			// location of current tile;
			AEVec2 wpos{ (x * tilesize), tilesize - ((y + 1) * tilesize) };
			// if wall is active && wave is colliding with current wall;
			if (maze[y][x] && circle_square_collision(pos, scale.x, wpos, tilesize)) {

				// boundary of wall;
				f32 wall_left = wpos.x - (tilesize / 2.f);
				f32 wall_right = wpos.x + (tilesize / 2.f);
				f32 wall_top = wpos.y + (tilesize / 2.f);
				f32 wall_bot = wpos.y - (tilesize / 2.f);

				// distance of wave position from wall position;
				f32 dist_x{ abs(pos.x - wpos.x) }, dist_y{ abs(pos.y - wpos.y) };
				
				// variables for spawning reflections;
				AEGfxVertexList** r_mesh{ nullptr };
				bool r_change{ false }; bool r_corner{ false };
				f32 r_max{}, r_scale{};
				AEVec2 rpos{};

				// direct reflections -----------------------------------------------------------------------------
				if ((pos.x >= wall_left && pos.x <= wall_right)
					|| (pos.y >= wall_bot && pos.y <= wall_top)) {
					
					rpos = locate_reflection(pos, wpos, tilesize);
					r_mesh = mesh + 1;
					r_max = REFLECTIONS_MAX_SIZE; r_scale = scale.x;

					if (dist_x > dist_y) {
						// wave left of wall;
						if (pos.x < wall_left && !maze[y][clamp(x - 1, 0, MAZE_WIDTH)] && r_collision[0] < 2) {
							r_collision[0] += 1;
							r_change = true;
						}
						// wave right of wall;
						else if (pos.x > wall_right && !maze[y][clamp(x + 1, 0, MAZE_WIDTH)] && r_collision[1] < 2) {
							r_collision[1] += 1;
							r_change = true;
						}
					}
					else {
						// wave above wall;
						if (pos.y > wall_top && !maze[clamp(y - 1, 0, MAZE_HEIGHT)][x] && r_collision[2] < 2) {
							r_collision[2] += 1;
							r_change = true;
						}
						// wave below wall;
						else if (pos.y < wall_bot && !maze[clamp(y + 1, 0, MAZE_HEIGHT)][x] && r_collision[3] < 2) {
							r_collision[3] += 1;
							r_change = true;
						}
					}

				}
				// corner reflections ----------------------------------------------------------------------------
				else {

					r_mesh = mesh;
					r_max = REFLECTIONS_MAX_SIZE / 3.f; r_scale = scale.x / 5.f;

					if (pos.y > wall_top && !maze[clamp(y - 1, 0, MAZE_HEIGHT)][x] && r_collision[2] < 2) {
						// top left corner;
						if (pos.x < wall_left && !maze[y][clamp(x - 1, 0, MAZE_WIDTH)] && r_collision[0] < 2) {
							r_collision[0] += 1; r_collision[2] += 1;
							r_change = true;
							rpos = { wall_left, wall_top };
						}
						// top right corner;
						else if (pos.x > wall_right && !maze[y][clamp(x + 1, 0, MAZE_WIDTH)] && r_collision[1] < 2) {
							r_collision[1] += 1; r_collision[2] += 1;
							r_change = true;
							rpos = { wall_right, wall_top };
						}
					}
					else if (pos.y < wall_bot && !maze[clamp(y + 1, 0, MAZE_HEIGHT)][x] && r_collision[3] < 2) {
						// bot left corner;
						if (pos.x < wall_left && !maze[y][clamp(x - 1, 0, MAZE_WIDTH)] && r_collision[0] < 2) {
							r_collision[0] += 1; r_collision[3] += 1;
							r_change = true;
							rpos = { wall_left, wall_bot };
						}
						// bot right corner;
						else if (pos.x > wall_right && !maze[y][clamp(x + 1, 0, MAZE_WIDTH)] && r_collision[1] < 2) {
							r_collision[1] += 1; r_collision[3] += 1;
							r_change = true;
							rpos = { wall_right, wall_bot };
						}
					}
				}

				// spawn reflections ------------------------------------------------------------------------------
				for (Wave& r : reflections) {
					if (!r.is_active) {

						DIRECTION dir{ DIR_LEFT };
						// iterate through directions
						for (int i{}; i < 4; ++i, ++dir) {
							// only spawn 1 time and if r_collision_check array has been updated;
							if (1 == r_collision[i] && r_change) {
								r.spawn(rpos, r_mesh, r_max, growth_rate, r_scale, dir);
								std::cout << "Spawning reflection : " << dir << " wall[" << y << "][" << x << "]\n";
								r_collision[i] += 1;
								break;
							}
						}
					}
				}
			}
		}
	}
}


//==================================================================================================================
// OUTLINE MEMBER FUNCTIONS;
//==================================================================================================================
void Outline::spawn(AEVec2 pos_in, f32 max_in, f32 grow_r, AEGfxVertexList* mesh_in, DIRECTION dir_in) {

	// GameObj variables;
	*mesh = mesh_in;
	type = TYPE_WAVE;
	is_active = true;

	pos = pos_in;
	rot = set_rotation(dir_in);

	scale = { 2.f, 2.f };
	
	// Outline variables;
	max_len = max_in;
	growth_rate = grow_r;

	return;

}

void Outline::update() {

	// quick exit for non-active line;
	if (!is_active) {
		return;
	}

	if (scale.y > max_len) {
		is_active = false;

		// reset variables;
		scale.x = scale.y = 2.f;
		growth_rate = GROWTH_R;

		return;
	}

	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
	scale.y += growth_rate * dt;
	growth_rate *= 1.05f;

	return;

}

void Outline::draw() {

	// quick exit for non-active wave;
	if (!is_active) {
		return;
	}
	// set transforms;
	AEMtx33 transform = set_transform(scale, pos, rot);
	// draw wave;
	AEGfxMeshDraw(*mesh, AE_GFX_MDM_TRIANGLES);

	return;
}


//==================================================================================================================
// GENERAL WAVE FUNCTIONS;
//==================================================================================================================
// finds spawn position {x, y} of reflection wave;
AEVec2 locate_reflection(AEVec2 wave_pos, AEVec2 wall_pos, f32 wall_size) {
	
	// set boundaries of wall;
	f32 wall_left	= wall_pos.x - (wall_size / 2.f);
	f32 wall_right	= wall_pos.x + (wall_size / 2.f);
	f32 wall_top	= wall_pos.y + (wall_size / 2.f);
	f32 wall_bot	= wall_pos.y - (wall_size / 2.f);

	// initialize variables for reflection wave;
	f32 pos_x{}, pos_y{};

	// find x position of reflection wave------------------------------
	f32 dist_x{}, dist_y{};

	if (wave_pos.x < wall_left) {			// if wave to left of wall;
		dist_x = wall_left - wave_pos.x;
		pos_x = wall_left + dist_x;
	}
	else if (wave_pos.x > wall_right) {		// else if right of wall;
		dist_x = wave_pos.x - wall_right;
		pos_x = wall_right - dist_x;
	}
	else {
		pos_x = wave_pos.x;
	}

	// find y position of reflection wave------------------------------
	if (wave_pos.y < wall_bot) {			// if below wall;
		dist_y = wall_bot - wave_pos.y;
		pos_y = wall_bot + dist_y;
	}
	else if (wave_pos.y > wall_top) {
		dist_y = wave_pos.y - wall_top;		// else if above wall;``
		pos_y = wall_top - dist_y;
	}
	else {
		pos_y = wave_pos.y;
	}

	return AEVec2{ pos_x, pos_y };
}

#if 0

// direct reflections;
if ((wave.pos.x >= wpos.x - tilesize / 2.f && wave.pos.x <= wpos.x + tilesize / 2.f)
	|| (wave.pos.y >= wpos.y - tilesize / 2.f && wave.pos.y <= wpos.y + tilesize / 2.f)) {
}

for (Wave& i : wavearray) {
	if (i.is_active) {
		i.update();
		
		for (int y{}; y < MAZE_HEIGHT; ++y) {
			for (int x{}; x < MAZE_WIDTH; ++x) {

				// position of current tile;
				AEVec2 wpos{ (x * tilesize), tilesize - ((y + 1) * tilesize) };

				if (maze[y][x] && circle_square_collision(wave.pos, wave.scale.x, wpos, tilesize)) {
					WM_collision_check(i, maze, tilesize);
					AEVec2 r_pos = locate_reflection(i.pos);
				}
			}
		}
	}
}


#endif

void spawn_wave(Wave(&wave)[MAX_NUM_WAVES], f32 pos_x, f32 pos_y, AEGfxVertexList** mesh_in) {
	for (Wave& i : wave) {
		if (!i.is_active) {
			i.spawn(AEVec2{ pos_x, pos_y }, mesh_in, WAVES_SIZE);
			return;
		}
	}
}

//==================================================================================================================
// create meshes for waves and outlines
//==================================================================================================================
AEGfxVertexList* Create_SemiCircle_Mesh(f32 outerRadius, f32 innerRadius, int numSegments, int divisor) {
	// Start creating the mesh
	AEGfxMeshStart();

	// Loop through the segments to define vertices
	for (int i = 0; i < (numSegments / divisor); ++i)
	{
		// Calculate angles for the current and next segments

		f32 theta1 = (2.0f * PI * i) / numSegments;
		f32 theta2 = (2.0f * PI * (i + 1)) / numSegments;

		// Outer ring vertices
		f32 x1_outer = outerRadius * cosf(theta1);
		f32 y1_outer = outerRadius * sinf(theta1);
		f32 x2_outer = outerRadius * cosf(theta2);
		f32 y2_outer = outerRadius * sinf(theta2);

		// Inner ring vertices
		f32 x1_inner = innerRadius * cosf(theta1);
		f32 y1_inner = innerRadius * sinf(theta1);
		f32 x2_inner = innerRadius * cosf(theta2);
		f32 y2_inner = innerRadius * sinf(theta2);

		// Add two triangles to form a quad between the inner and outer rings
		AEGfxTriAdd(
			x1_inner, y1_inner, 0xFF888888, 0.0f, 1.0f,  // Inner vertex 1
			x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f,  // Outer vertex 2
			x1_outer, y1_outer, 0xFFFFFFFF, 0.0f, 0.0f   // Outer vertex 1
		);

		AEGfxTriAdd(
			x1_inner, y1_inner, 0xFF888888, 0.0f, 1.0f,  // Inner vertex 1
			x2_inner, y2_inner, 0xFF888888, 1.0f, 1.0f,  // Inner vertex 2
			x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f   // Outer vertex 2
		);
	}

	// End the mesh and return it
	return AEGfxMeshEnd();
}
AEGfxVertexList* CreateWaveMesh(f32 outerRadius, f32 innerRadius, int numSegments) {
	// Start creating the mesh
	AEGfxMeshStart();

	// Loop through the segments to define vertices
	for (int i = 0; i < numSegments; ++i)
	{
		// Calculate angles for the current and next segments

		f32 theta1 = (2.0f * PI * i) / numSegments;
		f32 theta2 = (2.0f * PI * (i + 1)) / numSegments;

		// Outer ring vertices
		f32 x1_outer = outerRadius * cosf(theta1);
		f32 y1_outer = outerRadius * sinf(theta1);
		f32 x2_outer = outerRadius * cosf(theta2);
		f32 y2_outer = outerRadius * sinf(theta2);

		// Inner ring vertices
		f32 x1_inner = innerRadius * cosf(theta1);
		f32 y1_inner = innerRadius * sinf(theta1);
		f32 x2_inner = innerRadius * cosf(theta2);
		f32 y2_inner = innerRadius * sinf(theta2);

		// Add two triangles to form a quad between the inner and outer rings
		AEGfxTriAdd(
			x1_inner, y1_inner, 0xFF888888, 0.0f, 1.0f,  // Inner vertex 1
			x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f,  // Outer vertex 2
			x1_outer, y1_outer, 0xFFFFFFFF, 0.0f, 0.0f   // Outer vertex 1
		);

		AEGfxTriAdd(
			x1_inner, y1_inner, 0xFF888888, 0.0f, 1.0f,  // Inner vertex 1
			x2_inner, y2_inner, 0xFF888888, 1.0f, 1.0f,  // Inner vertex 2
			x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f   // Outer vertex 2
		);
	}

	// End the mesh and return it
	return AEGfxMeshEnd();
}
AEGfxVertexList* Create_LineMesh(bool skew) {
	if (!skew) {

		AEGfxMeshStart();

		// normal square;
		AEGfxTriAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

		AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			-0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		return AEGfxMeshEnd();
	}
	AEGfxMeshStart();

	// corner skewed square;
	AEGfxTriAdd(0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	return AEGfxMeshEnd();
}
