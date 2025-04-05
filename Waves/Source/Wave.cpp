/* Start Header ************************************************************************/
/*!
\file       Wave.cpp
\author     Chewn Thing Kwan
\email      kwan.c@digipen.edu

\brief      This header file declares functions that are necessary for dealing with
			echolocation waves.

			Functionality includes spawning and destroying waves, reflection waves and
			outlines logic, and drawing echolocation elements.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Wave.h"

namespace {

	f32 set_rotation(DIRECTION dir_in) {
		f32 rot{};

		switch (dir_in) {
			case DIR_LEFT:
				rot = 90; // 135
				break;
			case DIR_RIGHT:
				rot = -90; // -45
				break;
			case DIR_DOWN:
				rot = 180; // 225
				break;
			default:
				rot = 0; // 45
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

	DIRECTION find_direction(AEVec2 dist, AEVec2 pos1, AEVec2 pos2) {

		AEVec2 V{ pos1.x - pos2.x, pos1.y - pos2.y }; AEVec2 Vn{ V.y, -V.x };
		DIRECTION dir = DIR_NULL;

		// four corner vectors;
		AEVec2 topleft{ -1, 1 }, topright{ 1, 1 }, botleft{ -1, -1 }, botright{ 1, -1 };
		// four directions
		AEVec2 up{ 0, 1 }, down{ 0, -1 }, left{ -1, 0 }, right{ 1, 0 };

		if (dist.y > dist.x) {
			// mouse above player
			if (pos1.y > pos2.y && (AEVec2DotProduct(&V, &up) >= 0) && (AEVec2DotProduct(&Vn, &topleft) * AEVec2DotProduct(&Vn, &topright) < 0)) {
				dir = DIR_UP;
			}
			// mouse below player
			else if (pos1.y < pos2.y && (AEVec2DotProduct(&V, &down) >= 0) && (AEVec2DotProduct(&Vn, &botleft) * AEVec2DotProduct(&Vn, &botright) < 0)) {
				dir = DIR_DOWN;
			}
		}
		else {
			// mouse left of player
			if (pos1.x < pos2.x && (AEVec2DotProduct(&V, &left) >= 0) && (AEVec2DotProduct(&Vn, &topleft) * AEVec2DotProduct(&Vn, &botleft) < 0)) {
				dir = DIR_LEFT;
			}
			// mouse right of player
			else if (pos1.x > pos2.x && (AEVec2DotProduct(&V, &right) >= 0) && (AEVec2DotProduct(&Vn, &botright) * AEVec2DotProduct(&Vn, &topright) < 0)) {
				dir = DIR_RIGHT;
			}
		}

		return dir;
	}

	f32 outline_maxlen(std::pair<int, int> wave_pos, std::pair<int, int> wall_pos, f32& line, f32 tilesize, DIRECTION dir) {
			
		switch (dir) {
		case DIR_LEFT:
			for (int t{ wall_pos.first - 1 }; t > 0; --t) {
				bool blocked_hor = (wave_pos.second > wall_pos.second) ? maze[clamp(wall_pos.second + 1, 0, MAX_ROW)][t] == 1
					: maze[clamp(wall_pos.second - 1, 0, MAX_ROW)][t] == 1;

				if (maze[wall_pos.second][t] != 1 || maze[wave_pos.second][t] == 1 || blocked_hor) {
					break;
				}
				
				line += tilesize;
			}
			break;
		case DIR_RIGHT:
			for (int t{ wall_pos.first + 1 }; t < MAX_COL; ++t) {
				bool blocked_hor = (wave_pos.second > wall_pos.second) ? maze[clamp(wall_pos.second + 1, 0, MAX_ROW)][t] == 1
					: maze[clamp(wall_pos.second - 1, 0, MAX_ROW)][t] == 1;

				if (maze[wall_pos.second][t] != 1 || maze[wave_pos.second][t] == 1 || blocked_hor) {
					break;
				}
				line += tilesize;
			}
			break;
		case DIR_UP:
			for (int t{ wall_pos.second - 1 }; t > 0; --t) {
				bool blocked_ver = (wave_pos.first > wall_pos.first) ? maze[t][clamp(wall_pos.first + 1, 0, MAX_COL)] == 1
					: maze[t][clamp(wall_pos.first - 1, 0, MAX_COL)] == 1;

				if (maze[t][wall_pos.first] != 1 || maze[t][wave_pos.first] == 1 || blocked_ver) {
					break;
				}
				line += tilesize;
			}
			break;
		case DIR_DOWN:
			for (int t{ wall_pos.second + 1 }; t < MAX_ROW; ++t) {
				bool blocked_ver = (wave_pos.first > wall_pos.first) ? maze[t][clamp(wall_pos.first + 1, 0, MAX_COL)] == 1
					: maze[t][clamp(wall_pos.first - 1, 0, MAX_COL)] == 1;

				if (maze[t][wall_pos.first] != 1 || maze[t][wave_pos.first] == 1 || blocked_ver) {
					break;
				}
				line += tilesize;
			}
			break;
		}
		return line;
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
// TO PRINT DIRECTIONS TO STD::COUT FOR DEBUGGING;
//static std::ostream& operator<<(std::ostream& os, DIRECTION& dir) {
//
//	switch (dir) {
//	case (DIR_LEFT):
//		os << "Direction Left";
//		break;
//	case (DIR_RIGHT):
//		os << "Direction Right";
//		break;
//	case (DIR_UP):
//		os << "Direction Up";
//		break;
//	case (DIR_DOWN):
//		os << "Direction Down";
//		break;
//	default:
//		os << "Direction Null";
//	}
//
//	return os;
//
//}

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

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);

	// set transforms;
	AEMtx33 transform = set_transform(scale, pos, rot);
	AEGfxSetTransform(transform.m);
	// draw wave;
	AEGfxMeshDraw(*mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	return;

}

// update collision check array in Wave object; spawns reflections;
void Wave::echolocation(f32 tilesize, Wave(&reflections)[MAX_NUM_REFLECTIONS], Outline(&outlines)[MAX_NUM_OUTLINES]) {

	for (int y{}; y < activeRow; ++y) {
		for (int x{}; x < activeCol; ++x) {

			// location of current tile;
			AEVec2 wpos{ (x * tilesize), tilesize - ((y + 1) * tilesize) };
			// if wall is active && wave is colliding with current wall;
			if (maze[y][x] == 1 && circle_square_collision(pos, scale.x, wpos, tilesize)) {

				// boundary of wall;
				f32 wall_left = wpos.x - (tilesize / 2.f);
				f32 wall_right = wpos.x + (tilesize / 2.f);
				f32 wall_top = wpos.y + (tilesize / 2.f);
				f32 wall_bot = wpos.y - (tilesize / 2.f);

				// distance of wave position from wall position;
				f32 dist_x{ abs(pos.x - wpos.x) }, dist_y{ abs(pos.y - wpos.y) };

				std::pair<int, int> wave_index = find_wave_index(pos);

				// variables for creating reflections;
				AEGfxVertexList** r_mesh{ nullptr };
				AEVec2 rpos{};
				f32 r_max{}, r_scale{};
				bool r_change{ false };

				// variables for creating pairs of outlines;
				std::pair<DIRECTION, DIRECTION> l_dir{};
				AEVec2 l_maxlen{}, l_temp{}, l_pos{};

				// ================================================================================================
				// CALCULATE REFLECTIONS & OUTLINES
				// ================================================================================================
				// DIRECT COLLISIONS ------------------------------------------------------------------------------
				if ((pos.x >= wall_left && pos.x <= wall_right)
					|| (pos.y >= wall_bot && pos.y <= wall_top)) {

					// reflection init specific to direct collisions;
					rpos = locate_reflection(pos, wpos, tilesize);
					r_mesh = mesh + 1;
					r_max = REFLECTIONS_MAX_SIZE; r_scale = scale.x * 0.8f;

					if (dist_x > dist_y) {

						// directions for each line components;
						l_dir = { DIR_UP, DIR_DOWN }; l_pos.y = pos.y;

						// initialize max length of each line component;
						l_maxlen.x = (wall_top - pos.y);
						l_maxlen.y = (pos.y - wall_bot);

						// wave left of wall;
						if (pos.x < wall_left && maze[y][clamp(x - 1, 0, MAX_COL)] != 1 && r_collision[0] < 2) {

							// set outline to slightly left of wall;
							l_pos.x = wall_left - 1;
							// update wave collision check array;
							r_collision[0] += 1;
							r_change = true;

						}
						// wave right of wall;
						if (pos.x > wall_right && maze[y][clamp(x + 1, 0, MAX_COL)] != 1 && r_collision[1] < 2) {

							// set outline to slightly right of wall;
							l_pos.x = wall_right + 1;
							// update wave collision check array;
							r_collision[1] += 1;
							r_change = true;

						}
					}
					else if (dist_x < dist_y) {

						// directions for each line components;
						l_dir = { DIR_LEFT, DIR_RIGHT }; l_pos.x = pos.x;

						// initialize max length of each line component;
						l_maxlen.x = (pos.x - wall_left);
						l_maxlen.y = (wall_right - pos.x);

						// wave above wall;
						if (pos.y > wall_top && maze[clamp(y - 1, 0, MAX_ROW)][x] != 1 && r_collision[2] < 2) {

							// set outline to slightly above wall;
							l_pos.y = wall_top + 1;
							// update wave collision check array;
							r_collision[2] += 1;
							r_change = true;

						}
						// wave below wall;
						else if (pos.y < wall_bot && maze[clamp(y + 1, 0, MAX_ROW)][x] != 1 && r_collision[3] < 2) {

							// set outline to slightly below wall;
							l_pos.y = wall_bot - 1;
							// update wave collision check array;
							r_collision[3] += 1;
							r_change = true;

						}
					}
				}
				// CORNER COLLISIONS -----------------------------------------------------------------------------
				if ((pos.x < wall_left || pos.x > wall_right)
					&& (pos.y < wall_bot || pos.y > wall_top)) {

					// reflection init specific to corner collisions;
					r_mesh = mesh;
					r_max = REFLECTIONS_MAX_SIZE / 2.f; r_scale = scale.x / 8.f;

					l_maxlen.x = l_maxlen.y = tilesize;

					if (pos.y >= wall_top && maze[clamp(y - 1, 0, MAX_ROW)][x] != 1 && !(r_collision[2])) {
						// TOP LEFT CORNER ------------
						if (pos.x <= wall_left && maze[y][clamp(x - 1, 0, MAX_COL)] != 1 && r_collision[4] < 2 && !(r_collision[0])) {

							// directions for each line components;
							l_dir = { DIR_DOWN, DIR_RIGHT };
							l_pos.x = wall_left - 1; l_pos.y = wall_top + 1;

							r_collision[4] += 1;
							r_change = true;
							rpos = { wall_left, wall_top };
						}
						// TOP RIGHT CORNER -----------
						if (pos.x >= wall_right && maze[y][clamp(x + 1, 0, MAX_COL)] != 1 && r_collision[5] < 2 && !(r_collision[1])) {

							// directions for each line components;
							l_dir = { DIR_DOWN, DIR_LEFT };
							l_pos.x = wall_right + 1; l_pos.y = wall_top + 1;

							r_collision[5] += 1;
							r_change = true;
							rpos = { wall_right, wall_top };
						}

					}
					if (pos.y <= wall_bot && maze[clamp(y + 1, 0, MAX_ROW)][x] != 1 && !(r_collision[3])) {
						// BOT LEFT CORNER ------------
						if (pos.x <= wall_left && maze[y][clamp(x - 1, 0, MAX_COL)] != 1 && r_collision[6] < 2 && !(r_collision[0])) {

							// directions for each line components;
							l_dir = { DIR_UP, DIR_RIGHT };
							l_pos.x = wall_left - 1; l_pos.y = wall_bot - 1;

							r_collision[6] += 1;
							r_change = true;
							rpos = { wall_left, wall_bot };
						}
						// BOT RIGHT CORNER -----------
						if (pos.x >= wall_right && maze[y][clamp(x + 1, 0, MAX_COL)] != 1 && r_collision[7] < 2 && !(r_collision[1])) {

							// directions for each line components;
							l_dir = { DIR_UP, DIR_LEFT };
							l_pos.x = wall_right + 1; l_pos.y = wall_bot - 1;

							r_collision[7] += 1;
							r_change = true;
							rpos = { wall_right, wall_bot };
						}
					}
				}
				// extending line until either: end of wall; wave path is blocked;
				l_temp.x = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.x, tilesize, l_dir.first);
				l_temp.y = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.y, tilesize, l_dir.second);

				// limit the maximum length of outline to WAVES_SIZE;
				l_maxlen.x = (l_temp.x > WAVES_SIZE) ? WAVES_SIZE : l_maxlen.x;
				l_maxlen.y = (l_temp.y > WAVES_SIZE) ? WAVES_SIZE : l_maxlen.y;

				// ================================================================================================
				// CREATE REFLECTIONS & OUTLINES
				// ================================================================================================
				// spawn reflections;
				for (Wave& r : reflections) {
					if (!r.is_active) {

						DIRECTION dir{ DIR_LEFT };
						// iterate through directions
						for (int i{}; i < 8; ++i, ++dir) {

							// only spawn 1 time and if r_collision_check array has been updated;
							if (1 == r_collision[i] && r_change) {

								if (i < 4) {
									// create reflection
									r.spawn(rpos, r_mesh, r_max, growth_rate * 0.7f, r_scale, dir);
								}
								// create outline;
								create_two_outlines(outlines, l_pos, l_maxlen, l_dir, mesh + 2, growth_rate * 0.7f);

								r_collision[i] += 1;
							}
						}
					}
				}
			}
		}
	}
}

DirectionalWave::DirectionalWave() : direction{DIR_NULL}, prevp{}, lifetime{} {}

void DirectionalWave::spawn(AEVec2 pos_in, AEGfxVertexList** mesh_in, f32 max_in, DIRECTION dir_in, f32 grow_r) {
	// GameObj variables;
	mesh = mesh_in;
	type = TYPE_WAVE;
	is_active = true;

	rot = set_rotation(dir_in);

	scale.x = scale.y = 0.f;

	// Wave variables;
	max_size = max_in;
	growth_rate = grow_r * 2.f;

	// DirWave unique;
	direction = dir_in;
	prevp = pos = pos_in;
	lifetime = 0.f;

	return;
}

// reset corner collisions when gap;
void DirectionalWave::reset_corner() {
	std::pair<int, int> windex = find_wave_index(pos);

	if (direction == DIR_LEFT) {
		// bottom left;
		if (maze[clamp(windex.second + 1, 0, MAX_ROW)][windex.first] != 1) { r_collision[6] = 0; }
		// top left;
		if (maze[clamp(windex.second - 1, 0, MAX_ROW)][windex.first] != 1) { r_collision[4] = 0; }
	}
	else if (direction == DIR_RIGHT) {
		// bottom right;
		if (maze[clamp(windex.second - 1, 0, MAX_ROW)][windex.first] != 1) { r_collision[7] = 0; }
		// top right;
		if (maze[clamp(windex.second - 1, 0, MAX_ROW)][windex.first] != 1) { r_collision[5] = 0; }
	}
	else if (direction == DIR_UP) {
		// top left;
		if (maze[windex.second][clamp(windex.first - 1, 0, MAX_COL)] != 1) { r_collision[4] = 0; }
		// top right;
		if (maze[windex.second][clamp(windex.first + 1, 0, MAX_COL)] != 1) { r_collision[5] = 0; }
	}
	else if (direction == DIR_DOWN) {
		// bottom left;
		if (maze[windex.second][clamp(windex.first - 1, 0, MAX_COL)] != 1) { r_collision[6] = 0; }
		// bottom right;
		if (maze[windex.second][clamp(windex.first + 1, 0, MAX_COL)] != 1) { r_collision[7] = 0; }
	}
}

// despawns directional wave when it hits a wall head-on;
void DirectionalWave::check_collide() {

	if (!is_active) {
		return;
	}

	std::pair<int, int> windex = find_wave_index(pos);
	if (maze[windex.second][windex.first] == 1) {
		is_active = false;
	}

	AEVec2 tmp = pos;

	switch (direction) {
	case DIR_LEFT:
		tmp.x = tmp.x - (0.5f * scale.x);
		break;
	case DIR_RIGHT:
		tmp.x = tmp.x + (0.5f * scale.x);
		break;
	case DIR_DOWN:
		tmp.y = tmp.y - (0.5f * scale.y);
		break;
	case DIR_UP:
		tmp.y = tmp.y + (0.5f * scale.y);
		break;
	}

	windex = find_wave_index(tmp);
	if (maze[windex.second][windex.first] == 1) {
		is_active = false;
		scale.x = 0.f;
		growth_rate = GROWTH_R * 2.f;

		for (short& i : r_collision) {
			i = 0;
		}
		lifetime = 0.f;
		return;
	}
}

// echolocation override specific to directional wave;
void DirectionalWave::echolocation(f32 tilesize, Outline(&outlines)[MAX_NUM_OUTLINES]){

	for (int y{}; y < activeRow; ++y) {
		for (int x{}; x < activeCol; ++x) {

			// location of current tile;
			AEVec2 wpos{ (x * tilesize), tilesize - ((y + 1) * tilesize) };
			// if wall is active && wave is colliding with current wall;
			if (maze[y][x] == 1 && circle_square_collision(pos, scale.x, wpos, tilesize)) {

				f32 dir_wave_max = 350.f;
				reset_corner();

				// boundary of wall;
				f32 wall_left = wpos.x - (tilesize / 2.f);
				f32 wall_right = wpos.x + (tilesize / 2.f);
				f32 wall_top = wpos.y + (tilesize / 2.f);
				f32 wall_bot = wpos.y - (tilesize / 2.f);

				// distance of wave position from wall position;
				f32 dist_x{ abs(pos.x - wpos.x) }, dist_y{ abs(pos.y - wpos.y) };

				std::pair<int, int> wave_index = find_wave_index(pos);

				// variables for creating reflections;
				AEGfxVertexList** r_mesh{ nullptr };
				AEVec2 rpos{};
				f32 r_max{}, r_scale{};
				bool r_change{ false };

				// variables for creating pairs of outlines;
				std::pair<DIRECTION, DIRECTION> l_dir{};
				AEVec2 l_maxlen{}, l_temp{}, l_pos{};

				// ================================================================================================
				// EARLY SKIP IF NOT IN DIRECTION;
				// ================================================================================================
				DIRECTION wall_dir = find_direction(AEVec2{ dist_x, dist_y }, wpos, pos);

				// skip opposites;
				if (direction == DIR_LEFT && wall_dir == DIR_RIGHT) { continue; }
				if (direction == DIR_RIGHT && wall_dir == DIR_LEFT) { continue; }
				if (direction == DIR_UP && wall_dir == DIR_DOWN) { continue; }
				if (direction == DIR_DOWN && wall_dir == DIR_UP) { continue; }

				// ================================================================================================
				// CALCULATE REFLECTIONS & OUTLINES
				// ================================================================================================
				// DIRECT COLLISIONS ------------------------------------------------------------------------------
				if ((pos.x >= wall_left && pos.x <= wall_right)
					|| (pos.y >= wall_bot && pos.y <= wall_top)) {

					// reflection init specific to direct collisions;
					rpos = locate_reflection(pos, wpos, tilesize);
					r_mesh = mesh + 1;
					r_max = REFLECTIONS_MAX_SIZE; r_scale = scale.x * 0.8f;

					if (dist_x > dist_y) {

						// directions for each line components;
						l_dir = { DIR_UP, DIR_DOWN }; l_pos.y = pos.y;

						// initialize max length of each line component;
						l_maxlen.x = (wall_top - pos.y);
						l_maxlen.y = (pos.y - wall_bot);

						// wave left of wall;
						if (pos.x < wall_left && maze[y][clamp(x - 1, 0, MAX_COL)] != 1 && r_collision[0] < 2) {

							// set outline to slightly left of wall;
							l_pos.x = wall_left - 1;
							// update wave collision check array;
							r_collision[0] += 1;
							r_change = true;
						}
						// wave right of wall;
						if (pos.x > wall_right && maze[y][clamp(x + 1, 0, MAX_COL)] != 1 && r_collision[1] < 2) {

							// set outline to slightly right of wall;
							l_pos.x = wall_right + 1;
							// update wave collision check array;
							r_collision[1] += 1;
							r_change = true;
						}

						// extending line until either: end of wall; wave path is blocked;
						l_temp.x = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.x, tilesize, l_dir.first);
						l_temp.y = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.y, tilesize, l_dir.second);
						
						// limit the maximum length of outline to WAVES_SIZE;
						if (direction == DIR_UP || direction == DIR_DOWN) {
							l_maxlen.x = (l_temp.x > dir_wave_max) ? dir_wave_max : l_maxlen.x;
							l_maxlen.y = (l_temp.y > dir_wave_max) ? dir_wave_max : l_maxlen.y;
						}
						else {
							l_maxlen.x = (l_temp.x > max_size) ? max_size : l_maxlen.x;
							l_maxlen.y = (l_temp.y > max_size) ? max_size : l_maxlen.y;
						}

						switch (direction) {
						case DIR_DOWN:
							l_maxlen.x = (wall_top - pos.y);
							break;
						case DIR_UP:
							l_maxlen.y = (pos.y - wall_bot);
							break;
						}
					}
					else if (dist_x < dist_y) {

						// directions for each line components;
						l_dir = { DIR_LEFT, DIR_RIGHT }; l_pos.x = pos.x;

						// initialize max length of each line component;
						l_maxlen.x = (pos.x - wall_left);
						l_maxlen.y = (wall_right - pos.x);

						// wave above wall;
						if (pos.y > wall_top && maze[clamp(y - 1, 0, MAX_ROW)][x] != 1 && r_collision[2] < 2) {

							// set outline to slightly above wall;
							l_pos.y = wall_top + 1;
							// update wave collision check array;
							r_collision[2] += 1;
							r_change = true;

						}
						// wave below wall;
						else if (pos.y < wall_bot && maze[clamp(y + 1, 0, MAX_ROW)][x] != 1 && r_collision[3] < 2) {

							// set outline to slightly below wall;
							l_pos.y = wall_bot - 1;
							// update wave collision check array;
							r_collision[3] += 1;
							r_change = true;

						}

						// extending line until either: end of wall; wave path is blocked;
						l_temp.x = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.x, tilesize, l_dir.first);
						l_temp.y = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.y, tilesize, l_dir.second);

						// limit the maximum length of outline to WAVES_SIZE;
						if (direction == DIR_LEFT || direction == DIR_RIGHT) {
							l_maxlen.x = (l_temp.x > dir_wave_max) ? dir_wave_max : l_maxlen.x;
							l_maxlen.y = (l_temp.y > dir_wave_max) ? dir_wave_max : l_maxlen.y;
						}
						else {
							l_maxlen.x = (l_temp.x > max_size) ? max_size : l_maxlen.x;
							l_maxlen.y = (l_temp.y > max_size) ? max_size : l_maxlen.y;
						}

						switch (direction) {
						case DIR_RIGHT:
							l_maxlen.x = (pos.x - wall_left);
							break;
						case DIR_LEFT:
							l_maxlen.y = (wall_right - pos.x);
							break;
						}
					}
				}
				// CORNER COLLISIONS -----------------------------------------------------------------------------
				if ((pos.x < wall_left || pos.x > wall_right)
					&& (pos.y < wall_bot || pos.y > wall_top)) {

					// reflection init specific to corner collisions;
					r_mesh = mesh;
					r_max = REFLECTIONS_MAX_SIZE / 2.f; r_scale = scale.x / 8.f;

					l_maxlen.x = l_maxlen.y = tilesize;

					if (pos.y >= wall_top && maze[clamp(y - 1, 0, MAX_ROW)][x] != 1 && direction != DIR_UP) {
						// TOP LEFT CORNER ------------
						if (pos.x <= wall_left && maze[y][clamp(x - 1, 0, MAX_COL)] != 1 && r_collision[4] < 2 && direction != DIR_LEFT) {

							// directions for each line components;
							l_dir = { DIR_DOWN, DIR_RIGHT };
							l_pos.x = wall_left - 1; l_pos.y = wall_top + 1;

							r_collision[4] += 1;
							r_change = true;
							rpos = { wall_left, wall_top };

							// extending line until either: end of wall; wave path is blocked;
							if (direction == DIR_RIGHT) {
								l_temp.y = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.y, tilesize, l_dir.second);
							}
							else { l_temp.y = max_size / 8.f; }

						}
						// TOP RIGHT CORNER -----------
						if (pos.x >= wall_right && maze[y][clamp(x + 1, 0, MAX_COL)] != 1 && r_collision[5] < 2 && direction != DIR_RIGHT) {

							// directions for each line components;
							l_dir = { DIR_DOWN, DIR_LEFT };
							l_pos.x = wall_right + 1; l_pos.y = wall_top + 1;

							r_collision[5] += 1;
							r_change = true;
							rpos = { wall_right, wall_top };

							// extending line until either: end of wall; wave path is blocked;
							if (direction == DIR_LEFT) {
								l_temp.y = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.y, tilesize, l_dir.second);
							}
							else { l_temp.y = max_size / 8.f; }
						}

						if (direction == DIR_DOWN) {
							l_temp.x = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.x, tilesize, l_dir.first);
						}
						else { l_temp.x = max_size / 8.f; }

					}
					if (pos.y <= wall_bot && maze[clamp(y + 1, 0, MAX_ROW)][x] != 1 && direction != DIR_DOWN) {
						// BOT LEFT CORNER ------------
						if (pos.x <= wall_left && maze[y][clamp(x - 1, 0, MAX_COL)] != 1 && r_collision[6] < 2 && direction != DIR_LEFT) {

							// directions for each line components;
							l_dir = { DIR_UP, DIR_RIGHT };
							l_pos.x = wall_left - 1; l_pos.y = wall_bot - 1;

							r_collision[6] += 1;
							r_change = true;
							rpos = { wall_left, wall_bot };

							// extending line until either: end of wall; wave path is blocked;
							if (direction == DIR_RIGHT) {
								l_temp.y = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.y, tilesize, l_dir.second);
							}
							else { l_temp.y = max_size / 8.f; }
						}
						// BOT RIGHT CORNER -----------
						if (pos.x >= wall_right && maze[y][clamp(x + 1, 0, MAX_COL)] != 1 && r_collision[7] < 2 && direction != DIR_RIGHT) {

							// directions for each line components;
							l_dir = { DIR_UP, DIR_LEFT };
							l_pos.x = wall_right + 1; l_pos.y = wall_bot - 1;

							r_collision[7] += 1;
							r_change = true;
							rpos = { wall_right, wall_bot };

							// extending line until either: end of wall; wave path is blocked;
							if (direction == DIR_LEFT) {
								l_temp.y = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.y, tilesize, l_dir.second);
							}
							else { l_temp.y = max_size / 8.f; }
						}

						if (direction == DIR_UP) {
							l_temp.x = outline_maxlen(wave_index, std::pair<int, int>{x, y}, l_maxlen.x, tilesize, l_dir.first);
						}
						else { l_temp.x = max_size / 8.f; }
					}

					// limit the maximum length of outline to WAVES_SIZE;
					l_maxlen.x = (l_temp.x > dir_wave_max) ? dir_wave_max : l_maxlen.x;
					l_maxlen.y = (l_temp.y > dir_wave_max) ? dir_wave_max : l_maxlen.y;
				}

				// ================================================================================================
				// CREATE REFLECTIONS & OUTLINES
				// ================================================================================================
				DIRECTION dir{ DIR_LEFT };
				// iterate through directions
				for (int i{}; i < 8; ++i, ++dir) {

					// only spawn 1 time and if r_collision_check array has been updated;
					if (1 == r_collision[i] && r_change) {
						// create outline;
						create_two_outlines(outlines, l_pos, l_maxlen, l_dir, mesh + 2, growth_rate);

						r_collision[i] += 1;
					}
				}		
			}
		}
	}
}

// update loop for directional wave;
void DirectionalWave::update() {
	// quick exit for non-active wave;
	if (!is_active) {
		return;
	}

	if (lifetime > 0.8f) {
		is_active = false;

		scale.x = 0.f;
		growth_rate = GROWTH_R * 2.f;

		for (short& i : r_collision) {
			i = 0;
		}
		lifetime = 0.f;
		return;
	}

	// check if wave has grown to max_size;
	if (scale.x >= max_size || scale.y >= max_size) {
		f32 move = (growth_rate / GROWTH_R - 1.f) * 0.8f;

		switch (direction) {
		case DIR_LEFT:
			pos.x -= move;
			break;
		case DIR_RIGHT:
			pos.x += move;
			break;
		case DIR_UP:
			pos.y += move;
			break;
		case DIR_DOWN:
			pos.y -= move;
			break;
		}

	}

	// update wave size;
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
	lifetime += dt;

	if (scale.x <= max_size && scale.y <= max_size) {
		scale.x += growth_rate * dt;
		scale.y += growth_rate * dt;
	}
	else {
		scale.x += growth_rate * dt * 0.1f;
		scale.y += growth_rate * dt * 0.1f;
	}
	growth_rate *= 1.05f;

	return;
}
// draw directional wave;
void DirectionalWave::draw() {
	// quick exit for non-active wave;
	if (!is_active) {
		return;
	}

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);

	// set transforms;
	AEMtx33 transform = set_transform(scale, pos, rot + 45.f);
	AEGfxSetTransform(transform.m);
	// draw wave;
	AEGfxMeshDraw(mesh[1], AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	return;
}

//==================================================================================================================
// OUTLINE MEMBER FUNCTIONS;
//==================================================================================================================
Outline::Outline() : max_len{}, growth_rate{}, lifetime{} {}

void Outline::spawn(AEVec2 pos_in, f32 max_in, f32 grow_r, AEGfxVertexList** mesh_in, DIRECTION dir_in) {

	// GameObj variables;
	mesh = mesh_in;
	type = TYPE_OUTLINE;
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

	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
	lifetime += dt;

	if (lifetime >= 1.5f) {
		is_active = false;

		// reset variables;
		scale.x = scale.y = 2.f;
		growth_rate = GROWTH_R;

		lifetime = 0.f;
	}

	if (scale.y > max_len) {
		scale.y = max_len;
		growth_rate = 0.f;
	}

	scale.y += growth_rate * dt;
	growth_rate *= 1.05f;

	return;

}

void Outline::draw() {

	// quick exit for non-active wave;
	if (!is_active) {
		return;
	}

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);

	// set transforms;
	AEMtx33 transform = set_transform(scale, pos, rot);
	AEGfxSetTransform(transform.m);
	// draw wave;
	AEGfxMeshDraw(*mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

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
// finds x and y index of wave with respect to maze and size of each tile;
std::pair<int, int> find_wave_index(AEVec2 wpos) {

	int x_idx{}, y_idx{};

	// maze begins at 0,0; x -> positive, y -> negative;
	x_idx = static_cast<int>((wpos.x - TILE_SIZE / 2.f + TILE_SIZE) / TILE_SIZE);
	y_idx = abs(static_cast<int>((wpos.y - TILE_SIZE / 2.f) / TILE_SIZE));

	return { x_idx, y_idx };
}

// spawn a wave from array of waves;
void spawn_wave(Wave(&wave)[MAX_NUM_WAVES], f32 pos_x, f32 pos_y, AEGfxVertexList** mesh_in, f32 max_size) {
	for (Wave& i : wave) {
		if (!i.is_active) {
			i.spawn(AEVec2{ pos_x, pos_y }, mesh_in, max_size);
			return;
		}
	}
}
// spawn directional wave with reference to mouse position;
void spawn_dir_wave(DirectionalWave& wave, AEVec2 ppos, AEGfxVertexList** mesh_in) {
	// init mouse store;
	s32 mx{}, my{};
	// calculate position with respect to cam;
	AEInputGetCursorPosition(&mx, &my);

	f32 mpos_x = static_cast<f32>(mx - AEGfxGetWindowWidth() / 2) + ppos.x;
	f32 mpos_y = static_cast<f32>(AEGfxGetWindowHeight() / 2 - my) + ppos.y;

	// distance between player and mouse position;
	f32 dist_x = abs(ppos.x - mpos_x);
	f32 dist_y = abs(ppos.y - mpos_y);

	DIRECTION dir = find_direction(AEVec2{ dist_x, dist_y }, AEVec2{ mpos_x, mpos_y }, ppos);
	
	wave.spawn(ppos, mesh_in, 100.f, dir);
}

// creates a pair of outlines;
void create_two_outlines(Outline(&outlines)[MAX_NUM_OUTLINES], AEVec2 pos_in, AEVec2 maxlen_in,
	std::pair<DIRECTION, DIRECTION> dir_pair, AEGfxVertexList** mesh_in, f32 g_rate) {
	bool spawned{ false };

	for (Outline& l : outlines) {

		if (!l.is_active) {

			if (!spawned) {
				l.spawn(pos_in, maxlen_in.x, g_rate, mesh_in, dir_pair.first);
				spawned = true;
			}
			else {
				l.spawn(pos_in, maxlen_in.y, g_rate, mesh_in, dir_pair.second);
				break;
			}
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
			x1_inner, y1_inner, 0xFFFFFFFF, 0.0f, 1.0f,  // Inner vertex 1
			x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f,  // Outer vertex 2
			x1_outer, y1_outer, 0xFFFFFFFF, 0.0f, 0.0f   // Outer vertex 1
		);

		AEGfxTriAdd(
			x1_inner, y1_inner, 0xFFFFFFFF, 0.0f, 1.0f,  // Inner vertex 1
			x2_inner, y2_inner, 0xFFFFFFFF, 1.0f, 1.0f,  // Inner vertex 2
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
			x1_inner, y1_inner, 0xFFFFFFFF, 0.0f, 1.0f,  // Inner vertex 1
			x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f,  // Outer vertex 2
			x1_outer, y1_outer, 0xFFFFFFFF, 0.0f, 0.0f   // Outer vertex 1
		);

		AEGfxTriAdd(
			x1_inner, y1_inner, 0xFFFFFFFF, 0.0f, 1.0f,  // Inner vertex 1
			x2_inner, y2_inner, 0xFFFFFFFF, 1.0f, 1.0f,  // Inner vertex 2
			x2_outer, y2_outer, 0xFFFFFFFF, 1.0f, 0.0f   // Outer vertex 2
		);
	}

	// End the mesh and return it
	return AEGfxMeshEnd();
}
AEGfxVertexList* Create_LineMesh() {

	// skewed square;
	AEGfxTriAdd(-0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		-0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);		// bottom left aligned triangle;

	AEGfxTriAdd(-0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);		// top right aligned triangle;
	return AEGfxMeshEnd();

}
