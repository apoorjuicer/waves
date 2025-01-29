
#include "AEEngine.h"

#include "sonar.hpp"
#include <iostream>

namespace {
	f32 clamp(f32 value, f32 min, f32 max) {
		if (value < min) return min;    // If value is less than min, return min
		if (value > max) return max;    // If value is greater than max, return max
		return value;                   // Otherwise, return the value itself
	}
}

void update_wave_size(struct wave_s& wave) {

	if (!wave.alive) {
		return;
	}

	f32 dt = (f32)AEFrameRateControllerGetFrameTime();

	if (wave.current_size >= wave.max_size) {
		wave.alive = false;

		wave.current_size = 0;
		wave.growth_rate = 50.f;

		return;
	}

	wave.current_size += wave.growth_rate * dt;
	wave.growth_rate *= 1.05f;

	//std::cout << "Wave update - wave current size: " << wave.current_size << "\n";
}

void draw_wave(struct wave_s& wave) {
	AEMtx33 scale = { 0 }; AEMtx33 translate = { 0 };
		
	AEMtx33Trans(&translate, wave.x, wave.y);
	AEMtx33Scale(&scale, wave.current_size, wave.current_size);
	AEMtx33Concat(&wave.transform, &translate, &scale);
		
	AEGfxSetTransform(wave.transform.m);
		
	AEGfxMeshDraw(wave.mesh, AE_GFX_MDM_TRIANGLES);

	//std::cout << "Wave draw - wave current size: " << wave.current_size << "\n";
}


bool wave_collision(struct wave_s wave, struct wall_s wall) {

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

f32 float_abs(f32 num) {
	return (num < 0) ? -num : num;
}

void bounce_wave(struct wave_s& wave, struct wall_s const& wall, AEGfxVertexList* mesh) {
	// early escape
	if (!wave_collision(wave, wall)) {
		//wave2.alive = false;
		return;
	}

	//wave2.alive = true;

	// draw bounce wave
	f32 dist_x{}, dist_y{}, bounce_x{}, bounce_y{};
	f32 rotation{};

	if (wave.x < wall.x) {
		dist_x = (wall.x - wall.width / 2.f) - wave.x;
		bounce_x = (wall.x - wall.width / 2.f) + dist_x;
	}
	else {
		dist_x = wave.x - (wall.x + wall.width / 2.f);
		bounce_x = (wall.x + wall.width / 2.f) - dist_x;
	}

	if (wave.y < wall.y) {
		dist_y = (wall.y - wall.height / 2.f) - wave.y;
		bounce_y = (wall.y - wall.height / 2.f) + dist_y;
	}
	else {
		dist_y = wave.y - (wall.x + wall.width / 2.f);
		bounce_y = (wall.y + wall.height / 2.f) + dist_y;
	}

	if (abs(wave.x - wall.x) > abs(wave.y - wall.y)) {
		if (wave.x < wall.x) {
			rotation = 130;	// bounce -> left
		}
		else {
			rotation = -50;	// bounce -> right
		}
	}
	else {
		if (wave.y < wall.y) {
			rotation = -140; // bounce -> down
		}
		else {
			rotation = 40;	// bounce -> up
		}
	}

	AEMtx33 transform = { 0 }; AEMtx33 scale = { 0 };
	AEMtx33 translate = { 0 }; AEMtx33 rotate = { 0 };

	AEMtx33RotDeg(&rotate, rotation);
	AEMtx33Scale(&scale, wave.current_size, wave.current_size);
	AEMtx33Trans(&translate, bounce_x, bounce_y);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

}

void draw_bounce_wave(struct wave_s& wave, struct wall_s wall) {
	// draw bounce wave
	f32 dist_x{}, dist_y{}, bounce_x{}, bounce_y{};

	if (wave.x < wall.x) {
		dist_x = (wall.x - wall.width / 2.f) - wave.x;
		bounce_x = (wall.x - wall.width / 2.f) + dist_x;
	}
	else {
		dist_x = wave.x - (wall.x + wall.width / 2.f);
		bounce_x = (wall.x + wall.width / 2.f) + dist_x;
	}

	if (wave.y < wall.y) {
		dist_y = (wall.y - wall.height / 2.f) - wave.y;
		bounce_y = (wall.y - wall.height / 2.f) + dist_y;
	}
	else {
		dist_y = wave.y - (wall.x + wall.width / 2.f);
		bounce_y = (wall.y + wall.height / 2.f) + dist_y;
	}

	AEMtx33 transform = { 0 }; AEMtx33 scale = { 0 };
	AEMtx33 translate = { 0 }; AEMtx33 rotate = { 0 };

	AEMtx33RotDeg(&rotate, 90.f);
	AEMtx33Scale(&scale, wave.current_size, wave.current_size);
	AEMtx33Trans(&translate, bounce_x, bounce_y);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(wave.mesh, AE_GFX_MDM_TRIANGLES);
}

void draw_wall(struct wall_s wall, AEGfxVertexList* mesh) {
	AEMtx33 scale = { 0 }; AEMtx33 translate = { 0 };


	AEMtx33Trans(&translate, wall.x, wall.y);
	AEMtx33Scale(&scale, wall.width, wall.height);
	AEMtx33Concat(&wall.transform, &translate, &scale);

	AEGfxSetTransform(wall.transform.m);

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
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