#include "AEEngine.h"
#include "boundary.h"
#include "Player.h"
#include <iostream>

void Boundary_Clamp(player& p, f32 max_x, f32 max_y, f32 min_x, f32 min_y) {
	p.position.x = AEClamp(p.position.x, min_x, max_x);
	p.position.y = AEClamp(p.position.y, min_y, max_y);
}

void collide_obstacle(player& p, Rect& obstacle) {

	std::cout << "\ncollision_occurs :";
	f32 closestX = AEClamp(p.position.x, obstacle.center.x - obstacle.width / 2.0f, obstacle.center.x + obstacle.width / 2.0f);
	f32 closestY = AEClamp(p.position.y, obstacle.center.y - obstacle.height / 2.0f, obstacle.center.y + obstacle.height / 2.0f);
	std::cout << "\nclosestX: " << closestX << ", closestY: " << closestY;
	// Vector from player's position to the closest point
	f32 deltaX = p.position.x - closestX;
	f32 deltaY = p.position.y - closestY;
	// Distance from the player to the rectangle
	f32 distanceSquared = deltaX * deltaX + deltaY * deltaY;
	f32 playerRadius = p.radius;//radius * playerWidth;
	// Check if collision occurred
	if (distanceSquared <= playerRadius * playerRadius) {
		// Calculate the collision normal (direction to push the player out)
		f32 distance = sqrt(distanceSquared);
		f32 normalX = (distance > 0.0f) ? deltaX / distance : 1.0f;
		f32 normalY = (distance > 0.0f) ? deltaY / distance : 1.0f;
		// Push the player out of the obstacle
		p.position.x = closestX + normalX * playerRadius;
		p.position.y = closestY + normalY * playerRadius;

		std::cout << "\ncollision_occurs";
		std::cout << "\nPlayer Position before: (" << p.position.x << ", " << p.position.y << ")";
		std::cout << "\nClosest Point: (" << closestX << ", " << closestY << ")";
		std::cout << "\nDelta: (" << deltaX << ", " << deltaY << "), Distance^2: " << distanceSquared;
		std::cout << "\nPlayer Radius: " << playerRadius;
		std::cout << "\nCollision Normal: (" << normalX << ", " << normalY << ")";
		std::cout << "\nPlayer Position after: (" << p.position.x << ", " << p.position.y << ")";

	}

}