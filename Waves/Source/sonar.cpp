
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
				dist_x = (wall.x - wall.width / 2.f) - wave.x;
				r.x = (wall.x - wall.width / 2.f) + dist_x;
			}
			else if (wave.x > wall_right) {	// else wave is right of wall;
				dist_x = wave.x - (wall.x + wall.width / 2.f);
				r.x = (wall.x + wall.width / 2.f) - dist_x;
			}
			else {
				r.x = wave.x;
			}

			if (wave.y < wall_bot) {		// if below wall;
				dist_y = (wall.y - wall.height / 2.f) - wave.y;
				r.y = (wall.y - wall.height / 2.f) + dist_y;
			}
			else if (wave.y > wall_top){
				dist_y = wave.y - (wall.x + wall.width / 2.f);
				r.y = (wall.y + wall.height / 2.f) + dist_y;
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
