
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
// WAVE CLASS
//=============================================================================
// constructor
wave_c::wave_c() : x{}, y{}, curr_size{}, rotation{}, growth_rate{ 50.f }, mesh{}, max_size{ WAVES_SIZE }, is_active{ false } { /*empty by design*/ }
// utility
void wave_c::spawn(f32 x_in, f32 y_in, AEGfxVertexList* mesh_in, f32 rot) {
	// make wave active;
	is_active = true;
	// set position to input;
	x = x_in; y = y_in;
	// set parameters;
	curr_size = 0.f;
	max_size = WAVES_SIZE;
	growth_rate = 50.f;

	rotation = rot;
	mesh = mesh_in;

	//std::cout << "wave spawn\n";

	return;
}

void wave_c::update() {
	// quick exit for non-active wave;
	if (!is_active) {
		return;
	}
	
	if (curr_size >= max_size) {
		is_active = false;

		curr_size = 0.f;
		growth_rate = 50.f;

		reflected = { 0,0,0,0 };

		return;
	}

	f32 dt = (f32)AEFrameRateControllerGetFrameTime();

	curr_size += growth_rate * dt;
	growth_rate *= 1.05f;

	//std::cout << "wave update: " << curr_size << "\n";
	for (bool& i : reflected) {
		std::cout << "reflected: " << i << '\n';
	}

	return;
}

void wave_c::draw() {
	// quick exit for non-active wave;
	if (!is_active) {
		return;
	}
	// set transforms;
	AEMtx33 transform = { 0 }; AEMtx33 scale = { 0 };
	AEMtx33 translate = { 0 }; AEMtx33 rotate = { 0 };

	AEMtx33RotDeg(&rotate, rotation);
	AEMtx33Scale(&scale, curr_size, curr_size);
	AEMtx33Trans(&translate, x, y);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);
	// draw wave;
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	//std::cout << "wave draw" << '\n';

	return;
}

void wave_c::check_collision(int wall_x, int wall_y, f32 tile_size) {
	// quick exit for non-active waves;
	if (!is_active) {
		return;
	}

	// location of current tile;
	f32 posX = 0.0f + (wall_x * tile_size);
	f32 posY = tile_size - ((wall_y + 1) * tile_size);

	// clamp nearest x and y from tile;
	f32 near_x = clamp(x, posX - tile_size / 2.f, posX + tile_size / 2.f);
	f32 near_y = clamp(y, posY - tile_size / 2.f, posY + tile_size / 2.f);

	// calculate the distance from the circle center to this closest point
	f32 dx = x - near_x;
	f32 dy = y - near_y;

	// if the distance is less than or equal to the circle's radius, they collide
	f32 distanceSquared = (dx * dx) + (dy * dy);
	f32 radiusSquared = (curr_size) * (curr_size);

	int collsion_idx = 0;

	if (distanceSquared <= radiusSquared) {
		std::cout << "Wave collided with Wall[" << wall_y << "][" << wall_x << "]\n";

		// update array of bools with reference to direction from tile;
		if (posX >= (x - curr_size) && posX <= (x + curr_size)) {	// vertically aligned with current wave;
			// collision top;
			if (posY > y) {
				std::cout << "Collision TOP ^^^\n";
				collsion_idx = TOP;
			}
			// collision bot;
			else {
				std::cout << "Collision BOT vvv\n";
				collsion_idx = BOT;
			}
		}
		else {	// horizontally aligned with current wave;
			// collision left;
			if (posX < x) {
				std::cout << "Collision LEFT <<<\n";
				collsion_idx = LEFT;
			}
			// collision right;
			else {
				std::cout << "Collision RIGHT >>>\n";
				collsion_idx = RIGHT;
			}
		}
		// update collision array
		reflected[collsion_idx]	= true;

		return;
	}

	return;
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

void draw_wave(struct wave_s const& wave, AEGfxVertexList const* mesh) {
	if (!wave.alive) {
		return;
	}

	AEMtx33 transform = { 0 }; AEMtx33 scale = { 0 };
	AEMtx33 translate = { 0 }; AEMtx33 rotate = { 0 };

	AEMtx33RotDeg(&rotate, wave.rotation);
	AEMtx33Scale(&scale, wave.current_size, wave.current_size);
	AEMtx33Trans(&translate, wave.x, wave.y);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(wave.mesh, AE_GFX_MDM_TRIANGLES);

	//std::cout << "Wave draw - wave current size: " << wave.current_size << "\n";
}

//=============================================================================
// reflection waves
//=============================================================================
// check collision with wall; circle rect collision;
bool wave_collision(struct wave_s wave, struct wall_s const wall) {

	if (!wave.alive) {
		return false;
	}
	f32 near_x = clamp(wave.x, wall.x - wall.width / 2.f, wall.x + wall.width / 2.f);
	f32 near_y = clamp(wave.y, wall.y - wall.height / 2.f, wall.y + wall.height / 2.f);

	// Calculate the distance from the circle center to this closest point
	f32 dx = wave.x - near_x;
	f32 dy = wave.y - near_y;

	// If the distance is less than or equal to the circle's radius, they collide
	f32 distanceSquared = (dx * dx) + (dy * dy);
	f32 radiusSquared = (wave.current_size) * (wave.current_size);

	if (distanceSquared <= radiusSquared) {
		return true;
	}

	return false;

}

// make a reflection wave come alive;
void spawn_reflection_wave(struct wave_s &wave, struct wall_s const(&wall), struct wave_s(&reflections)[MAX_NUM_REFLECTIONS]) {

	for (wave_s& r : reflections) {

		if (!r.alive) {

			r.alive = true;
			r.reflected = true;

			r.current_size = wave.current_size;
			r.growth_rate = wave.growth_rate;

			//=============================================================================
			// initialize reflection wave;
			//=============================================================================
			f32 wall_left = wall.x - (wall.width / 2.f);
			f32 wall_right = wall.x + (wall.width / 2.f);
			f32 wall_top = wall.y + (wall.height / 2.f);
			f32 wall_bot = wall.y - (wall.height / 2.f);

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
			else if (wave.y > wall_top){	// if above wall;
				dist_y = wave.y - wall_top;
				r.y = wall_top - dist_y;
			}
			else {
				r.y = wave.y;
			}

			if (abs(wave.x - wall.x) > abs(wave.y - wall.y)) {
				if (wave.x < wall.x) {
					r.rotation = 120;	// bounce -> left
				}
				else {
					r.rotation = -60;	// bounce -> right
				}
			}
			else {
				if (wave.y < wall.y) {
					r.rotation = 210; // bounce -> down
				}
				else {
					r.rotation = 30;	// bounce -> up
				}
			}
			return;
			//=============================================================================
		}

	}

}

// init array of reflection waves; use before game loop;
void init_reflection_waves(wave_s (&reflection_waves)[MAX_NUM_REFLECTIONS], AEGfxVertexList* mesh) {
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
		line.alive = false;
		line.width = line.height = line.growth_rate = 0;

		time = 0.f;
	}

	if (Top == line.type || Bottom == line.type) {
		line.width += line.growth_rate * dt;
	}
	else {
		line.height += line.growth_rate * dt;
	}

	line.growth_rate *= 1.05f;
	if (line.width >= MAX_LINE_LENGTH || line.height >= MAX_LINE_LENGTH) {
		line.growth_rate = 0.f;
	}
}

void draw_outlines(struct line_s& line, AEGfxVertexList* mesh) {
	if (!line.alive) {
		return;
	}

	AEMtx33 transform = { 0 }; AEMtx33 scale = { 0 }; 
	AEMtx33 translate = { 0 }; //AEMtx33 rotate = { 0 };

	AEMtx33Scale(&scale, line.width, line.height);
	//AEMtx33RotDeg(&rotate, line.rotation);
	AEMtx33Trans(&translate, line.x, line.y);

	//AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &scale);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void spawn_outlines(struct wave_s const& wave, struct wall_s const(&wall), struct line_s(&line)[MAX_NUM_OUTLINES]) {
	f32 right = wall.x + (wall.width / 2.f);
	f32 left = wall.x - (wall.width / 2.f);
	f32 top = wall.y + (wall.height / 2.f);
	f32 bot = wall.y - (wall.height / 2.f);

	for (line_s& l : line) {
		if (!l.alive) {
			l.alive = true;
			l.width = l.height = 2.f;
			l.growth_rate = wave.growth_rate;

			//l.maxheight = wall.height;
			//l.maxwidth = wall.width;

			if (wave.x >= left && wave.x <= right) {
				l.x = wave.x;
				/*l.y = top + 2.f;
				l.type = Bottom;*/

				if (wave.y < wall.y) {
					l.type = Bottom;
					l.y = bot - 1.f;
				}
				else {
					l.type = Top;
					l.y = top + 1.f;
				}
			}
			else if (wave.y >= bot && wave.y <= top) {
				l.y = wave.y;

				if (wave.x > wall.x) {
					l.type = Right;
					l.x = right + 1.f;
				}
				else {
					l.type = Left;
					l.x = left -1.f;
				}
			}

			//if (wave.x < wall.x) {
			//	if (wave.y < wall.y) {
			//		l.type = BotLeft;

			//		l.x = left - 1.f;
			//		l.y = bot - 1.f;
			//		l.rotation = 0.f;
			//	}
			//	else {
			//		l.type = TopLeft;

			//		l.x = left - 1.f;
			//		l.y = top + 1.f;
			//		l.rotation = 90.f;
			//	}
			//}
			//else if (wave.y < wall.y) {
			//	l.type = BotRight;

			//	l.x = right + 1.f;
			//	l.y = bot - 1.f;
			//	l.rotation = -90.f;
			//}
			//else {
			//	l.type = TopRight;

			//	l.x = right + 1.f;
			//	l.y = top + 1.f;
			//	l.rotation = 180.f;
			//}

			return;
		}

	}
	
}

//=============================================================================
// temporary testing functions
//=============================================================================
void draw_wall(struct wall_s wall, AEGfxVertexList* mesh) {
	AEMtx33 scale = { 0 }; AEMtx33 translate = { 0 };


	AEMtx33Trans(&translate, wall.x, wall.y);
	AEMtx33Scale(&scale, wall.width, wall.height);
	AEMtx33Concat(&wall.transform, &translate, &scale);

	AEGfxSetTransform(wall.transform.m);

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

//=============================================================================
// temp meshes for testing
//=============================================================================
AEGfxVertexList* Create_SemiCircle_Mesh(f32 outerRadius, f32 innerRadius, int numSegments) {
	// Start creating the mesh
	AEGfxMeshStart();

	// Loop through the segments to define vertices
	for (int i = 0; i < (numSegments / 3); ++i)
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
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments) {
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

