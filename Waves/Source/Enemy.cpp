/* Start Header ************************************************************************/
/*!
\file		Enemy.cpp
\author		Benjamin Ban
\email		k.ban@digipen.edu
\co-author  Jordain
\email
\co-author  Henry
\email

\brief		This header file declares functions that are necessary for dealing with
			enemy movement and ai path finding

			Functionality includes pathfinding,enemystates, rendering enemy and
			moving enemy

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Enemy.h"

int prevDestX = -1;
int prevDestY = -1;

// Function to create the enemy mesh(normalized)
AEGfxVertexList* createEnemyMesh(u32 enemy_color) {
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, enemy_color, 0.0f, 1.0f,
		0.5f, -0.5f, enemy_color, 1.0f, 1.0f,
		-0.5f, 0.5f, enemy_color, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, enemy_color, 1.0f, 1.0f,
		0.5f, 0.5f, enemy_color, 1.0f, 0.0f,
		-0.5f, 0.5f, enemy_color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

void RenderEnemy_Mesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation)
{
	// Create transform matrices
	AEMtx33 scale_matrix = { 0 };
	AEMtx33 rotate_matrix = { 0 };
	AEMtx33 translate_matrix = { 0 };
	// Apply scaling, rotation, and translation
	AEMtx33Scale(&scale_matrix, scaleX, scaleY);           // Scaling
	AEMtx33Rot(&rotate_matrix, rotation);                  // Rotation
	AEMtx33Trans(&translate_matrix, posX, posY);           // Translation
	// Concatenate the matrices in the correct order (scale, then rotate, then translate)
	AEMtx33 transform_matrix = { 0 };
	AEMtx33Concat(&transform_matrix, &rotate_matrix, &scale_matrix);
	AEMtx33Concat(&transform_matrix, &translate_matrix, &transform_matrix);
	// Apply the transform and draw the mesh
	AEGfxSetTransform(transform_matrix.m);
	AEGfxMeshDraw(NameOfMesh, AE_GFX_MDM_TRIANGLES);
}

namespace {// Utility Functions
		//pathfinding functions
		
	//check cell (row, col) and grid[index] is valid
	bool isValid(int row, int col, int grid[][COL])
	{
		return ((row >= 0) && (row < ROW) && (col >= 0)
			&& (col < COL)) && grid[row][col] != 1;
	}

	// check dest reached
	bool isDestination(int row, int col, Pair dest)
	{
		return (row == dest.first && col == dest.second);
	}

	//calc h cost 
	double findHeuristic(int row, int col, Pair dest)
	{
		//manhattan distance
		return abs(row - dest.first) + abs(col - dest.second);
	}

	bool sort_lowerF(const pPair& a, const pPair& b) {
		return a.first < b.first;  // Sort by lower fcost
	}

	//reconstruct found path and store in vector path(does not add parentlesss node!(start))
	//storing from back to front
	void reconstructPath(Node gridNodes[][COL], Pair dest, std::vector<Pair>& path)
	{
		int i = dest.first;
		int j = dest.second;
		//if it's not its own parent keep looping
		while (!(gridNodes[i][j].parent_i == -1
			&& gridNodes[i][j].parent_j == -1)) {
			path.push_back({j,i });
			//update i and j to parent,if dont use temp bad diagonal movement occurs
			int temp_i = gridNodes[i][j].parent_i;
			int temp_j = gridNodes[i][j].parent_j;
			i = temp_i;
			j = temp_j;
		}
		path.push_back({ j,i });
		std::reverse(path.begin(), path.end());
		// print path
		//for (const Pair& p : path) {
		//	//printf(":[%d,%d] ", p.first, p.second);
		//}

	}//end of reconstructPath

	void processNeighbor(int new_i, int new_j, int i, int j, Pair dest,
		Node gridNodes[][COL], bool closedList[][COL],
		/*int grid[][COL],*/ std::vector<pPair>& openList/*, std::vector<Pair>& path*/) {

		//if (!isValid(new_i, new_j, grid)) { return ; }

		if (!closedList[new_i][new_j]) {
			double gNew = gridNodes[i][j].g + 1.0;
			double hNew = findHeuristic(new_i, new_j, dest);
			double fNew = gNew + hNew;

			//push unexplored nodes to openList
			if (gridNodes[new_i][new_j].f == VAL_INF || fNew < gridNodes[new_i][new_j].f) {
				openList.push_back({ fNew,{new_i, new_j} }); //push to openlist
				std::sort(openList.begin(), openList.end(), sort_lowerF);

				gridNodes[new_i][new_j].f = fNew;
				gridNodes[new_i][new_j].g = gNew;
				gridNodes[new_i][new_j].h = hNew;
				gridNodes[new_i][new_j].parent_i = i;
				gridNodes[new_i][new_j].parent_j = j;
			}
		}
	}//end of processNeighbor

	//state functions
	void MovetoPatrolpoint(Enemy& e) {
		// If the enemy is NOT at startPatrol, force it to go to startPatrol
		e.path.clear();
		e.pathIndex = 0;
		Pair enemyIndex = { e.gridX, e.gridY };
		astarsearch(maze, enemyIndex, e.startPatrol, e.path);
	/*	if (e.path.empty()) {
			std::cout << "Failed to find path to start patrol point.\n";
		}
		else {
			std::cout << "Path to start patrol point calculated successfully.\n";
		}*/
	}

	void chasePlayer(Enemy& e) {
		// Convert player's world position to grid coordinates
		/*int playerGridX = static_cast<int>(round(player.x / TILE_SIZE));
		int playerGridY = static_cast<int>(round(player.y / -TILE_SIZE));*/
		int playerGridX = static_cast<int>((player.x + TILE_SIZE / 2) / TILE_SIZE);
		int playerGridY = static_cast<int>((player.y - TILE_SIZE / 2) / TILE_SIZE) * -1;

		// Convert enemy's current position to grid coordinates
		int enemyGridX = e.gridX;
		int enemyGridY = e.gridY;

		// Always recalculate the path to the player's position
		e.path.clear();   // Clear old path
		e.pathIndex = 0;  // Reset path index

		Pair enemyPos = { enemyGridX, enemyGridY };
		Pair playerPos = { playerGridX, playerGridY };

		astarsearch(maze, enemyPos, playerPos, e.path);  // Find path to player

		/*if (e.path.empty()) {
			std::cout << "Failed to find a path to the player!\n";
		}
		else {
			std::cout << "Path to player calculated successfully.\n";
		}*/
	}

	bool seePlayer(Enemy& e, float sightRange) {
		float dist_x = player.x - e.x;
		float dist_y = player.y - e.y;
		return sqrt(dist_x * dist_x + dist_y * dist_y) <= sightRange;
	}

	bool seeRock(Enemy& e, float sightRange) {
		float dist_x = lastRock.x - e.x;
		float dist_y = lastRock.y - e.y;
		return sqrt(dist_x * dist_x + dist_y * dist_y) <= sightRange;
	}


}// end of anon namespace

Path astarsearch(int grid[ROW][COL], Pair src, Pair dst, std::vector<Pair>& path) {

	src = { src.second,src.first };
	dst = { dst.second,dst.first };

	path.clear(); //reset path
	if (!isValid(dst.first, dst.second, grid) || !isValid(src.first, src.second, grid)) {
		//printf("Source or Destination is invalid\n");
		return Path::NO_PATH;
	}
	// If the destination cell is the same as source cell
	if (isDestination(src.first, src.second, dst)) {
		//printf("Already at the destination\n");
		return Path::NO_PATH;
	}

	//ini closed list and grid nodes
	bool closedList[ROW][COL]{};
	//2D array
	Node gridNodes[ROW][COL]{};

	//initialize grid
	int i, j; //i is Y-axis , j is X-axis
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			gridNodes[i][j].f = VAL_INF;
			gridNodes[i][j].g = VAL_INF;
			gridNodes[i][j].h = VAL_INF;
			gridNodes[i][j].parent_i = 0;
			gridNodes[i][j].parent_j = 0;
		}
	}

	// Initialising the parameters of the starting node
	i = src.first, j = src.second;
	gridNodes[i][j].f = 0.0;
	gridNodes[i][j].g = 0.0;
	gridNodes[i][j].h = 0.0;
	gridNodes[i][j].parent_i = -1;
	gridNodes[i][j].parent_j = -1;
	std::vector<pPair> openList;

	//add start node to end of openlist
	openList.push_back({ 0.0, {i, j} }); //push pair to vector
	std::sort(openList.begin(), openList.end(), sort_lowerF); //sort vector

	int directions[4][2] = {
	{-1, 0},  // North  (++y)
	{1, 0},   // South  (--y)
	{0, -1},  // West   (--x)
	{0, 1}    // East   (++x)
	};


	while (!openList.empty()) { //if openlist not empty
		pPair p = *openList.begin();
		//erase point in open list
		openList.erase(openList.begin());

		//add point to closed list
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		// Loop over all four directions
		for (int d = 0; d < 4; d++) {
			int new_i = i + directions[d][0];  // Update row
			int new_j = j + directions[d][1];  // Update column

			if (!isValid(new_i, new_j, grid)) { continue; }
			if (isDestination(new_i, new_j, dst)) {
				gridNodes[new_i][new_j].parent_i = i;
				gridNodes[new_i][new_j].parent_j = j;
				//printf("The destination cell is found\n");
				//printPath(gridNodes, dest);
				reconstructPath(gridNodes, dst, path);
				return Path::PATH_FOUND;
			}

			//processNeighbor(new_i, new_j, i, j, dst, gridNodes, closedList, grid, openList/*, path*/);
			processNeighbor(new_i, new_j, i, j, dst, gridNodes, closedList, openList);
		}
	}//end of openlist check


	//printf("Pathfinding failed");
	return Path::NO_PATH;
}



void EnemyState(Enemy& e,float dt, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
	static float accumulatedTime = 0.0f;
	const float updateInterval = 0.1f;  // Move every 0.1 seconds
	accumulatedTime += dt;
	Pair currIndex = { e.gridX,e.gridY };

	bool inPatrolPath = std::find(e.patrolPath.begin(), e.patrolPath.end(), currIndex) != e.patrolPath.end();
	//std::cout << "current enemy index :" << currIndex.first << "," << currIndex.second << "\n";
	//pursuit rock
	int destX = static_cast<int>((lastRock.x + (TILE_SIZE / 2)) / TILE_SIZE);
	int destY = static_cast<int>((lastRock.y - (TILE_SIZE / 2)) / TILE_SIZE) * -1;
	// Enemy position conversion
	int srcX = static_cast<int>(e.x / TILE_SIZE);
	int srcY = static_cast<int>(e.y / TILE_SIZE) * -1;

	switch (e.state) {
	
	case IDLE:
		/*std::cout << "Enemy is idle.\n";*/
		break;
	case PATROL:
		//std::cout << "Enemy is patrolling.\n";
		//std::cout << "current index :"<<currIndex.first <<","<< currIndex.second << "\n";
		//std::cout << "start patrol index :" << e.startPatrol.first << "," << e.startPatrol.second << "\n";
		//std::cout << "end patrol index :" << e.endPatrol.first << "," << e.endPatrol.second << "\n";
		//std::cout << "Enemy path index: " << e.pathIndex << "\n";
		//std::cout << "Enemy path size: " << e.path.size() << "\n";		
		if (!inPatrolPath && e.path.empty()) {
			MovetoPatrolpoint(e);
		}
		// Check if we reached the last target in the path
		if (e.path.empty() || e.pathIndex >= e.path.size()) {
			// If the enemy has reached the patrol point, switch to the other patrol target
			if (currIndex == e.startPatrol) {
				//std::cout << "Reached start patrol, switching to end patrol.\n";
				astarsearch(maze, currIndex, e.endPatrol, e.path);
				e.pathIndex = 0; // Reset path index
			}
			else if (currIndex == e.endPatrol) {
				//std::cout << "Reached end patrol, switching to start patrol.\n";
				astarsearch(maze, currIndex, e.startPatrol, e.path);
				e.pathIndex = 0; // Reset path index
			}
		}
		// Call function to move enemy
		if (accumulatedTime >= updateInterval) {
			if (!e.path.empty() && e.pathIndex < e.path.size()) {
				e.moveEnemy((&MainWaves)[MAX_NUM_WAVES],mesh);
			}
			accumulatedTime = 0.0f;
		}	
		break;
	case PURSUIT_PLAYER:
		//std::cout << "Enemy is chasing player!\n";
		// Implement chase behavior
		chasePlayer(e);

		// Move enemy if enough time has passed
		if (accumulatedTime >= updateInterval) {
			if (!e.path.empty() && e.pathIndex < e.path.size()) {
				e.moveEnemy((&MainWaves)[MAX_NUM_WAVES], mesh);
			}
			accumulatedTime = 0.0f;
		}
		break;
	case PURSUIT_ROCK:
		//std::cout << "Enemy is chasing rock!\n";

		// If the destination (rock's position) has changed, recalculate the path
		if (destX != prevDestX || destY != prevDestY) {
			//std::cout << "Rock moved! Stopping enemy & recalculating path...\n";

			e.path.clear();   // Clear old path
			e.pathIndex = 0;  // Reset path index
			e.x = srcX * TILE_SIZE;  // Stop enemy at current position
			e.y = -srcY * TILE_SIZE; // Stop enemy at current position

			Pair src = { srcX, srcY };
			Pair dest = { destX, destY };

			astarsearch(maze, src, dest, e.path);

			// Update previous destination
			prevDestX = destX;
			prevDestY = destY;
		}
		// Move enemy if enough time has passed
		if (accumulatedTime >= updateInterval) {
			if (e.pathIndex < e.path.size()) {
				e.moveEnemy((&MainWaves)[MAX_NUM_WAVES], mesh);
				e.printPosition();
			}
			accumulatedTime = 0.0f;
		}
		break;

	}
}

void UpdateEnemystate(Enemy& e,float dt, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
	float sightRangeRock = 3.0f * TILE_SIZE;  // Enemy detection range for rock
	float sightRangePlayer = 1.0f * TILE_SIZE;  // Enemy detection range for player
	int destX = static_cast<int>((lastRock.x + (TILE_SIZE / 2)) / TILE_SIZE);
	int destY = static_cast<int>((lastRock.y - (TILE_SIZE / 2)) / TILE_SIZE) * -1;

	switch (e.state) {
	case IDLE:
		if (seeRock(e, sightRangeRock)) {
			e.state = PURSUIT_ROCK;
		}
		else if (seePlayer(e, sightRangePlayer)) {
			e.state = PURSUIT_PLAYER;
		}
		else {
			e.state = PATROL;
		}
		break;
	case PATROL:
		if (seeRock(e, sightRangeRock)) {
			e.state = PURSUIT_ROCK;
		}
		else if (seePlayer(e, sightRangePlayer)) {
			e.state = PURSUIT_PLAYER;
		}
		break;
	case PURSUIT_PLAYER:
		if (seeRock(e, sightRangeRock)) {
			e.state = PURSUIT_ROCK;
		}
		else if (!seePlayer(e, sightRangePlayer)) { //player zao alr
			e.state = PATROL;
		}

		break;
	case PURSUIT_ROCK:
		if (e.gridX == destX &&
			e.gridY == destY ){
			//std::cout << "Enemy reached the rock!\n";
			if (seePlayer(e, sightRangePlayer)) {
				e.state = PURSUIT_PLAYER;
				//std::cout << "Switching to PURSUIT_PLAYER.\n";
			}
			else {
				e.state = IDLE;  // Reached the rock
			}	
		}
		else if (seePlayer(e, sightRangePlayer)) { 
			e.state = PURSUIT_PLAYER;
		}
		else if (!seePlayer(e, sightRangePlayer)) { //player zao alr
			e.state = PATROL;
		}
		break;
	}

	//call enemystate after update
	EnemyState(e, dt, (&MainWaves)[MAX_NUM_WAVES], mesh);
}