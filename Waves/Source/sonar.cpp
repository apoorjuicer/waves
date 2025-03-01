
#include "AEEngine.h"

#include "sonar.hpp"
#include <iostream>

//=============================================================================
// helper functions
//=============================================================================
namespace {
	
	f32 clamp(f32 value, f32 min, f32 max) {
		if (value < min) return min;    // If value is less than min, return min
		if (value > max) return max;    // If value is greater than max, return max
		return value;                   // Otherwise, return the value itself
	}
	int clamp_int(int value, int min, int max) {
		if (value < min) return min;    // If value is less than min, return min
		if (value > max) return max;    // If value is greater than max, return max
		return value;                   // Otherwise, return the value itself
	}

	f32 float_abs(f32 num) {
		return (num < 0) ? -num : num;
	}

	bool point_rect_collision(f32 x, f32 y, f32 rx, f32 ry, f32 width, f32 height) {
		f32 left = rx - width / 2.f;
		f32 right = rx + width / 2.f;
		f32 bot = ry - height / 2.f;
		f32 top = ry + height / 2.f;

		return ((x >= left && x <= right) && (y <= top && y >= bot)) ? true : false;
	}
}
//=============================================================================
// general wave functions
//=============================================================================
// general update and draw; can be used for both main waves and reflections;
void update_waves(struct wave_s& wave) {

	if (!wave.alive) {
		return;
	}

	if (wave.current_size >= wave.max_size) {
		wave.alive = false;

		wave.current_size = 0;
		wave.growth_rate = 50.f;
		wave.reflected = false;

		return;
	}

	f32 dt = (f32)AEFrameRateControllerGetFrameTime();

	wave.current_size += wave.growth_rate * dt;
	wave.growth_rate *= 1.05f;

}

void draw_wave(struct wave_s(&wave)[MAX_NUM_WAVES]) {
	for (wave_s const& i : wave) {
		if (!i.alive) {
			return;
		}

		AEMtx33 transform = { 0 }; AEMtx33 scale = { 0 };
		AEMtx33 translate = { 0 }; AEMtx33 rotate = { 0 };

		AEMtx33RotDeg(&rotate, i.rotation);
		AEMtx33Scale(&scale, i.current_size, i.current_size);
		AEMtx33Trans(&translate, i.x, i.y);

		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(i.mesh, AE_GFX_MDM_TRIANGLES);
	}

}

void spawn_wave(struct wave_s(&MainWaves)[MAX_NUM_WAVES], f32 x, f32 y, AEGfxVertexList* mesh) {
	for (wave_s& i : MainWaves) {
		// if wave is already live, skip it;
		if (i.alive) {
			continue;
		}
		// else wave is not live;
		else {
			// make wave live;
			i.alive = true;
			// set position to input;
			i.x = x; i.y = y;
			// set parameters;
			i.current_size = 0.f;
			i.max_size = WAVES_SIZE;
			i.growth_rate = 50.f;

			i.reflected = false;
			i.mesh = mesh;

			return;
		}
	}
}

//=============================================================================
// reflection waves
//=============================================================================
// check collision with wall; circle rect collision;
bool wave_collision(struct wave_s wave, int index_x, int index_y, float tileSize) {
	if (!wave.alive) {
		return false;
	}

	// location of current tile;
	f32 posX = 0.0f + (index_x * tileSize);
	f32 posY = tileSize - ((index_y + 1) * tileSize);

	f32 near_x = clamp(wave.x, posX - tileSize / 2.f, posX + tileSize / 2.f);
	f32 near_y = clamp(wave.y, posY - tileSize / 2.f, posY + tileSize / 2.f);

	// Calculate the distance from the circle center to this closest point
	f32 dx = wave.x - near_x;
	f32 dy = wave.y - near_y;

	// If the distance is less than or equal to the circle's radius, they collide
	f32 distanceSquared = (dx * dx) + (dy * dy);
	f32 radiusSquared = (wave.current_size) * (wave.current_size);

	if (distanceSquared <= radiusSquared) {
		//std::cout << "Wave collided with Wall[" << index_y << "][" << index_x << "]\n";
		return true;
	}
	
	return false;
}

// spawns reflections and outlines based on maze array and tilesize;
void reflections_spawner(wave_s wave, int const (&maze)[20][20], f32 tilesize, struct wave_s(&reflections)[MAX_NUM_WAVES], struct line_s(&line)[MAX_NUM_OUTLINES], AEGfxVertexList* (&mesh)[NUM_WAVE_MESHES]) {
	// iterate though index of wall
	for (int y = 0; y < 20; ++y) {
		for (int x = 0; x < 20; ++x) {
			if (maze[y][x]) {
				// location of current tile;
				f32 posX = 0.0f + (x * tilesize);
				f32 posY = tilesize - ((y + 1) * tilesize);
				f32 tilehalf = tilesize / 2.f;

				if (wave_collision(wave, x, y, tilesize)) {
					// =============================================================================================================
					// OUTLINES
					// =============================================================================================================
					// check position of wall with relation to wave;
					f32 line_x{}, line_y{}; int origin{};

					// spawn location of line: x, y;
					if (wave.y < (posY - tilehalf)) {
						line_y = posY - tilehalf - 1;	// bot;

						if (wave.x < (posX - tilehalf)) {
							line_x = posX - tilehalf - 1;	// left;
							origin = BottomLeft;
						}
						else if (wave.x > (posX + tilehalf)) {
							line_x = posX + tilehalf + 1;	// right;
							origin = BottomRight;
						}
						else {
							line_x = posX;
							origin = OriginH;
						}
					}
					else if (wave.y > (posY + tilehalf)) {
						line_y = posY + tilehalf + 1;	// top;

						if (wave.x < (posX - tilehalf)) {
							line_x = posX - tilehalf - 1;	// left;
							origin = TopLeft;
						}
						else if (wave.x > (posX + tilehalf)) {
							line_x = posX + tilehalf + 1;	// right;
							origin = TopRight;
						}
						else {
							line_x = posX;
							origin = OriginH;
						}
					}
					else {
						line_y = posY;				// wave is within y bounds of wall;
						
						if (wave.x < (posX - tilehalf)) {
							line_x = posX - tilehalf - 1;	// left;
							origin = OriginV;
						}
						else {
							line_x = posX + tilehalf + 1;	// right;
							origin = OriginV;
						}

					}
					spawn_outlines(wave, line_x, line_y, tilesize, line, origin);
 
					// =============================================================================================================
					// REFLECTION WAVES
					// =============================================================================================================
					// wave is beyond left or right bound of current wall;
					if (wave.x < (posX - tilehalf) || wave.x >(posX + tilehalf)) {
						// if wave is above top bound of wall, and above is a wall, skip;
						if (wave.y < (posY - tilehalf) && maze[clamp_int(y + 1, 0, 10)][x]) {
							continue;
						}
						// if wave is below bot bound of wall, and below is a wall, skip;
						if (wave.y > (posY + tilehalf) && maze[clamp_int(y - 1, 0, 10)][x]) {
							continue;
						}
					}
					// wave is beyond top or bot bound of current wall;
					if (wave.y < (posY - tilehalf) || wave.y >(posY + tilehalf)) {
						// if wave is beyond left bound, and left is a wall, skip;
						if (wave.x < (posX - tilehalf) && maze[y][clamp_int(x - 1, 0, 10)]) {
							continue;
						}
						// if wave is beyond right bound, and right is a wall, skip;
						if (wave.x > (posX + tilehalf) && maze[y][clamp_int(x + 1, 0, 10)]) {
							continue;
						}
					}
					spawn_reflection_wave(wave, x, y, tilesize, reflections, mesh[1]);
				}
			}
		}
	}
}

// make a reflection wave come alive;
void spawn_reflection_wave(struct wave_s& wave, int index_x, int index_y, float tileSize, struct wave_s(&reflections)[MAX_NUM_WAVES], AEGfxVertexList* mesh) {
	// if wave was reflected before
	/*if (wave.reflected) {
		return;
	}*/
	
	// location of current tile;
	f32 posX = 0.0f + (index_x * tileSize);
	f32 posY = tileSize - ((index_y + 1) * tileSize);


	for (wave_s& r : reflections) {

		if (!r.alive) {

			r.alive = true;
			r.reflected = true;
			wave.reflected = true;

			r.current_size = wave.current_size - 20.f;
			r.growth_rate = wave.growth_rate - 20.f;
			r.mesh = mesh;

			//=============================================================================
			// initialize reflection wave;
			//=============================================================================
			f32 wall_left = posX - (tileSize / 2.f);
			f32 wall_right = posX + (tileSize / 2.f);
			f32 wall_top = posY + (tileSize / 2.f);
			f32 wall_bot = posY - (tileSize / 2.f);

			f32 dist_x{}, dist_y{};

			if (wave.x < wall_left) {		// if wave to left of wall;
				dist_x = wall_left - wave.x;
				r.x = wall_left + dist_x;
			}
			else if (wave.x > wall_right) {	// else wave is right of wall;
				dist_x = wave.x - wall_right;
				r.x = wall_right - dist_x;
			}
			else {
				r.x = wave.x;
			}

			if (wave.y < wall_bot) {		// if below wall;
				dist_y = wall_bot - wave.y;
				r.y = wall_bot + dist_y;
			}
			else if (wave.y > wall_top) {	// if above wall;
				dist_y = wave.y - wall_top;
				r.y = wall_top - dist_y;
			}
			else {
				r.y = wave.y;
			}

			if (wave.y >= wall_bot && wave.y <= wall_top) {
				if (wave.x < posX) {
					r.rotation = 90;	// bounce -> left
				}
				else {
					r.rotation = -90;	// bounce -> right
				}
			}
			else {
				if (wave.y < posY) {
					r.rotation = 180; // bounce -> down
				}
				else {
					r.rotation = 0;	// bounce -> up ; arc circle values : 120, -60, 210, 30
				}
			}
			return;
			//=============================================================================
		}

	}

}

// init array of reflection waves; use before game loop;
void init_reflection_waves(wave_s (&reflection_waves)[MAX_NUM_WAVES], AEGfxVertexList* mesh) {
	for (wave_s& i : reflection_waves) {
		i.x = 0; i.y = 0;
		i.current_size = 0.f;
		i.max_size = REFLECTIONS_MAX_SIZE;

		i.alive = false;
		i.reflected = true;

		i.growth_rate = 50.f;
		i.mesh = mesh;

		i.rotation = 0.f;
	}
}

//=============================================================================
// outlines
//=============================================================================

void update_outlines(struct line_s& line) {

	if (!line.alive) {
		return;
	}
	static f32 time = 0.f; f32 dt = (f32)AEFrameRateControllerGetFrameTime();
	time += dt;

	if (time >= 1.0f) {
		line.width = line.height = line.growth_rate = 0;
		line.alive = false;

		time = 0.f;
	}

	if (Horizontal == line.type) {
		line.width += line.growth_rate * dt;
	}
	else if (Vertical == line.type) {
		line.height += line.growth_rate * dt;
	}
	else {
		line.width += line.growth_rate * dt;
		line.height += line.growth_rate * dt;
	}

	line.growth_rate *= 1.05f;
	if (line.width >= MAX_LINE_LENGTH || line.height >= MAX_LINE_LENGTH) {
		line.growth_rate = 0.f;

		switch (line.type) {
		case Horizontal:
			line.width = MAX_LINE_LENGTH;
			break;
		case Vertical:
			line.height = MAX_LINE_LENGTH;
			break;
		default:
			line.width = MAX_LINE_LENGTH;
			line.height = MAX_LINE_LENGTH;
		}
	
	}
}

void draw_outlines(struct line_s& line, AEGfxVertexList* (&mesh)[NUM_WAVE_MESHES]) {
	if (!line.alive) {
		return;
	}

	AEMtx33 transform = { 0 }; AEMtx33 scale = { 0 }; 
	AEMtx33 translate = { 0 }; AEMtx33 rotate = { 0 };

	AEMtx33Scale(&scale, line.width, line.height);
	AEMtx33RotDeg(&rotate, line.rotation);
	AEMtx33Trans(&translate, line.x, line.y);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);
	
	// skewed square mesh
	if (Corner == line.type) {
		AEGfxMeshDraw(mesh[3], AE_GFX_MDM_TRIANGLES);
	}
	// regular square mesh
	else {
		AEGfxMeshDraw(mesh[2], AE_GFX_MDM_TRIANGLES);
	}
}

void spawn_outlines(struct wave_s const& wave, f32 posX, f32 posY, f32 tilesize, struct line_s(&line)[MAX_NUM_OUTLINES], int origin) {
	(void)tilesize;
	for (line_s& l : line) {
		if (!l.alive) {
			// make line live;
			l.alive = true;
			// set parameters;
			l.x = posX; l.y = posY;
			l.width = l.height = 2.f;
			l.growth_rate = wave.growth_rate;

			// set type and orientation;
			if (OriginH == origin) {
				l.type = Horizontal;
				l.rotation = 0;
				l.growth_rate += 20;
			}
			else if (OriginV == origin) {
				l.type = Vertical;
				l.rotation = 0;
				l.growth_rate += 20;
			}
			else {
				l.type = Corner;

				if (BottomLeft == origin) {
					l.rotation = 0;
				}
				else if (TopLeft == origin) {
					l.rotation = -90;
				}
				else if (TopRight == origin) {
					l.rotation = -180;
				}
				else {
					l.rotation = -270;
				}
			}
			return;
		}
	}
}

//=============================================================================
// create meshes for waves and outlines
//=============================================================================
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

