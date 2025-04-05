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

	
namespace {// Utility Functions
	//pathfinding functions

//check cell (row, col) and grid[index] is valid
	bool isValid(int row, int col, int grid[][MAX_COL])
	{
		return ((row >= 0) && (row < MAX_ROW) && (col >= 0)
			&& (col < MAX_COL)) && grid[row][col] != 1;
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
	void reconstructPath(Node gridNodes[][MAX_COL], Pair dest, std::vector<Pair>& path)
	{
		int i = dest.first;
		int j = dest.second;
		//if it's not its own parent keep looping
		while (!(gridNodes[i][j].parent_i == -1
			&& gridNodes[i][j].parent_j == -1)) {
			path.push_back({ j,i });
			//update i and j to parent,if dont use temp bad diagonal movement occurs
			int temp_i = gridNodes[i][j].parent_i;
			int temp_j = gridNodes[i][j].parent_j;
			i = temp_i;
			j = temp_j;
		}
		path.push_back({ j,i });
		std::reverse(path.begin(), path.end());
		// print path
		/*for (const Pair& p : path) {
			std::cout << ":[" << p.first << ", " << p.second << "] ";
		}*/

	}//end of reconstructPath

	void processNeighbor(int new_i, int new_j, int i, int j, Pair dest,
		Node gridNodes[][MAX_COL], bool closedList[][MAX_COL],
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


	bool IsOnPatrolPath(const Pair& current, const std::vector<Pair>& patrolPath) {
		return std::find(patrolPath.begin(), patrolPath.end(), current) != patrolPath.end();
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

}// end of anon namespace



	void Enemy::setEnemyPatrol(int grid[MAX_ROW][MAX_COL]) {
		patrolPath = findPatrolPoints(grid);
		//printEnemyPatrolpoints();
	}





	//class

	void Enemy::updateGridPosition() {
		/*gridX = static_cast<int>(round(x / TILE_SIZE));
		gridY = static_cast<int>(round(y / -TILE_SIZE));*/
		//manual offset superior
		gridX = static_cast<int>((x + TILE_SIZE / 2) / TILE_SIZE);
		gridY = static_cast<int>((y - TILE_SIZE / 2) / -TILE_SIZE);
	}
	// Display enemy position, for debug
	void Enemy::printPosition() const {
		std::cout << "Enemy Position in World: (" << x << ", " << y << ")\n";
		std::cout << "Enemy Position in Grid: (" << gridX << ", " << gridY << ")\n";
	}

	// Moves enemy along the computed path
	void Enemy::moveEnemy(Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
		(void)MainWaves;
		(void)mesh;

		//std::cout << "moving Enemy\n";
		if (path.empty()) {
			//std::cout << "path is empty\n";
			return;
		}

		// Skip the first step if it's the same as the current position
		if (pathIndex == 0 && path[0] == Pair(gridX, gridY)) {
			//std::cout << "skip start pos: (" << path[0].first << ", " << path[0].second << ")\n";
			pathIndex++; // Move to the next valid step
		}

		if (pathIndex < path.size()) {
			Pair nextPos = path[pathIndex];  // Get next position
			//grid to world
			//f32 targetX = nextPos.second * TILE_SIZE;  // Update enemy's row
			////f32 targetY = nextPos.first * -TILE_SIZE; // Update enemy's col

			f32 targetX = nextPos.first * TILE_SIZE;  // Update enemy's row
			f32 targetY = nextPos.second * -TILE_SIZE; // Update enemy's col
			// 
			////movement dir vectpr
			f32 dirX = targetX - x;
			f32 dirY = targetY - y;
			f32 distance = sqrt(dirX * dirX + dirY * dirY);
			//normalize dir vector and apply vel
			if (distance > ENEMY_SPEED) {
				x += (dirX / distance) * ENEMY_SPEED;
				y += (dirY / distance) * ENEMY_SPEED;
				//pathIndex++;  // Move to the next step in path
			}
			else {
				// Snap to the exact target when close enough
				//void spawn_wave(Wave(&arr)[MAX_NUM_WAVES], f32 pos_x, f32 pos_y, AEGfxVertexList** mesh_in, f32 max_size = WAVES_SIZE);

				x = targetX;
				y = targetY;
				pathIndex++;  // Move to the next step in path
			}

			//std::cout << "Moving to: (" << nextPos.first << ", " << nextPos.second << ")\n";
		}

		updateGridPosition();
	}


	void Enemy::enemy_wave(Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) const {
		static f32 time{};
		if (state != IDLE) {
			time += static_cast<f32>(AEFrameRateControllerGetFrameTime());
			if (time > 1.0) {
				spawn_wave(MainWaves, x, y, mesh, 50.f);
				time = 0;
			}

		}
	}

	std::string  Enemy::GetEnemyStateName() const {
		switch (state) {
		case IDLE: return "IDLE";
		case PATROL: return "PATROL";
		case PURSUIT_PLAYER: return "PURSUIT_PLAYER";
		case PURSUIT_ROCK: return "PURSUIT_ROCK";
		default: return "UNKNOWN_STATE,error";
		}
	}

	void Enemy::Update_EnemyPos(float& AT, float UT, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
		if (AT >= UT) {
			if (!path.empty() && pathIndex < path.size()) {
				//std::cout << "Moving Enemy\n";
				moveEnemy((&MainWaves)[MAX_NUM_WAVES], mesh);
			}
			AT = 0.0f;
		}
	}


	void Enemy::Update_Patrol(bool in_PatrolPath, Pair currIndexpos, float& AT, float UT, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
		if (!in_PatrolPath && path.empty()) {
			EnterPatrolPath();
		}
		// Check if we reached the last target in the path
		if (path.empty() || pathIndex >= path.size()) {
			// If the enemy has reached the patrol point, switch to the other patrol target
			if (currIndexpos == startPatrol) {
				//std::cout << "Reached start patrol, switching to end patrol.\n";
				if (astarsearch(maze, currIndexpos, endPatrol, path) == Path::NO_PATH) {
					//std::cout << "Path find Error end patrol";
				}
				pathIndex = 0; // Reset path index
			}
			else if (currIndexpos == endPatrol) {
				//std::cout << "Reached end patrol, switching to start patrol.\n";
				//astarsearch(maze, currIndex, e.startPatrol, e.path);
				if (astarsearch(maze, currIndexpos, startPatrol, path) == Path::NO_PATH) {
					//std::cout << "Path find Error start patrol";
				}
				pathIndex = 0; // Reset path index
			}
		}
		// Call function to move enemy
		Update_EnemyPos(AT, UT, MainWaves, mesh);
	}




	void Enemy::chasePlayer(float& AT, float UT, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
		// Convert player's world position to grid coordinates
		//std::cout << "chasePlayer called\n";
		
		/*int playerGridX = static_cast<int>(round(player.x / TILE_SIZE));
		int playerGridY = static_cast<int>(round(player.y / -TILE_SIZE));*/
		static int lastPlayerX = -1, lastPlayerY = -1;
		int playerGridX = static_cast<int>((player.x + TILE_SIZE / 2) / TILE_SIZE);
		int playerGridY = static_cast<int>((player.y - TILE_SIZE / 2) / TILE_SIZE) * -1;
		//if (state == PURSUIT_PLAYER && playerGridX == lastPlayerX && playerGridY == lastPlayerY) {
		//	return; // Skip recalculating path if player hasn't moved
		//}

		if (state == PURSUIT_PLAYER) {
			if (playerGridX != lastPlayerX || playerGridY != lastPlayerY || path.empty()) {
				lastPlayerX = playerGridX;
				lastPlayerY = playerGridY;

				// Recalculate path
				Pair enemyPos = { gridX, gridY };
				Pair playerPos = { playerGridX, playerGridY };

				path.clear();
				pathIndex = 0;

				if (astarsearch(maze, enemyPos, playerPos, path) == Path::NO_PATH) {
					//std::cout << "No path to player, switching to PATROL.\n";
					EnterPatrolPath();
					state = PATROL;
					return;
				}
			}

			Update_EnemyPos(AT, UT, MainWaves, mesh);
		}

		//lastPlayerX = playerGridX;
		//lastPlayerY = playerGridY;
		//// Convert enemy's current position to grid coordinates
		//int enemyGridX = gridX;
		//int enemyGridY = gridY;

		//// Always recalculate the path to the player's position
		//path.clear();   // Clear old path
		//pathIndex = 0;  // Reset path index

		//Pair enemyPos = { enemyGridX, enemyGridY };
		//Pair playerPos = { playerGridX, playerGridY };

		////astarsearch(maze, enemyPos, playerPos, e.path);  // Find path to player

		//if (astarsearch(maze, enemyPos, playerPos, path) == Path::NO_PATH) {
		//	std::cout << "Path find Error chasePlayer";
		//	EnterPatrolPath();  // Recalculate path to patrol point
		//	state = PATROL;  // Return to patrol if no path is found
		//}

		//Update_EnemyPos(AT, UT, MainWaves, mesh);

		/*if (e.path.empty()) {
			std::cout << "Failed to find a path to the player!\n";
		}
		else {
			std::cout << "Path to player calculated successfully.\n";
		}*/

	}//end of func


	void Enemy::EnemyState(Player& p,float dt, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh) {
		static float accumulatedTime = 0.0f;
		const float updateInterval = 0.1f;  // Move every 0.1 seconds
		accumulatedTime += dt;
		Pair currIndex = { gridX,gridY };
		bool inPatrolPath = std::find(patrolPath.begin(), patrolPath.end(), currIndex) != patrolPath.end();
		//std::cout << "current enemy index :" << currIndex.first << "," << currIndex.second << "\n";
		//pursuit rock
		static int destX = static_cast<int>((p.lastRockPos.x + TILE_SIZE / 2) / TILE_SIZE);
		static int destY = static_cast<int>((p.lastRockPos.y - (TILE_SIZE / 2)) / TILE_SIZE) * -1;
		// Enemy position conversion
		int srcX = static_cast<int>(x / TILE_SIZE);
		int srcY = static_cast<int>(y / TILE_SIZE) * -1;

		switch (state) {

		case IDLE:
			/*std::cout << "Enemy is idle.\n";*/
			break;
		case PATROL: Update_Patrol(inPatrolPath, currIndex, accumulatedTime, updateInterval, MainWaves, mesh);
			break;
		case PURSUIT_PLAYER:
			//std::cout << "Enemy is chasing player!\n";
			// Implement chase behavior
			chasePlayer(accumulatedTime, updateInterval, MainWaves, mesh);
			break;
		case PURSUIT_ROCK:
			//std::cout << "Enemy is chasing rock!\n";

			// If the destination (rock's position) has changed, recalculate the path
			if (destX != prevDestX || destY != prevDestY) {
				//std::cout << "Rock moved! Stopping enemy & recalculating path...\n";
				path.clear();   // Clear old path
				pathIndex = 0;  // Reset path index
				x = srcX * TILE_SIZE;  // Stop enemy at current position
				y = -srcY * TILE_SIZE; // Stop enemy at current position
				Pair src = { srcX, srcY };
				Pair dest = { destX, destY };
				//astarsearch(maze, src, dest, e.path);
				if (astarsearch(maze, src, dest, path) == Path::NO_PATH) {
					//std::cout << "No path pursuit_rock";
				}
				// Update previous destination
				prevDestX = destX;
				prevDestY = destY;
			}

			if (path.empty() && accumulatedTime == 0.0f) {
				EnterPatrolPath();
			}

			//std::cout << "Is path empty?" << path.empty() << "\n";
			// Move enemy if enough time has passed
			if (accumulatedTime >= updateInterval) {
				//if (e.pathIndex < e.path.size()) {
				if (!path.empty() && pathIndex < path.size()) {
					//std::cout << "move this enemy";
					moveEnemy((&MainWaves)[MAX_NUM_WAVES], mesh);
					//printPosition();
				}
				accumulatedTime = 0.0f;
			}

			if (path.empty() && accumulatedTime == 0.0f) {
				EnterPatrolPath();
				state = PATROL;
			}

			//std::cout << accumulatedTime<<"\n";

			//std::cout << updateInterval << "\n";
			break;

		}
	}

	void Enemy::UpdateEnemystate(Player& p,float dt, Wave(&MainWaves)[MAX_NUM_WAVES], AEGfxVertexList** mesh, DirectionalWave dir_wave) {

		// Clear path and reset index when state changes
		if (previousState != state) {
			/*std::cout << "State changed from " << GetEnemyStateName(e.previousState)
				<< " to " << GetEnemyStateName(e.state) << "\n";*/
			path.clear();      // Clear old path when switching states
			pathIndex = 0;     // Reset path index

			previousState = state;  // Update previous state
		}


		int destX = static_cast<int>((p.lastRockPos.x + (TILE_SIZE / 2)) / TILE_SIZE);
		int destY = static_cast<int>((p.lastRockPos.y - (TILE_SIZE / 2)) / TILE_SIZE) * -1;
		enemy_wave(MainWaves, mesh);

		//const float IDLE_TIME = idle_timer;       // Time to stay idle
		//const float PATROL_TIME = patrol_timer;     // Time to stay patrol 
		//const float PURSUIT_TIME = pursuit_timer;    // Time to pursue rock
		
		AEVec2 wave_col_pos{};

		// float IDLE_TIME = e.idle_timer;       // Time to stay idle
		 //float PATROL_TIME = e.patrol_timer;     // Time to stay patrol 
		 //float PURSUIT_TIME = e.pursuit_timer;    // Time to pursue rock

		stateTimer -= dt;

		switch (state) {
		case IDLE:

			if (seeRock(p,sightRangeRock) && state != PURSUIT_ROCK) {
				SetPursuitRockState();
			}
			else if (hitbywave(dir_wave, wave_col_pos) && state != PURSUIT_PLAYER) {
				//std::cout << "change state to pursuit player!\n";
				SetPursuitPlayerState();
			}
			else if (seePlayer(sightRangePlayer)) {
				SetPursuitPlayerState();
			}
			else if (stateTimer <= 0.0f) {

				SetPatrolState();
			}

			break;
		case PATROL:
			if (seeRock(p,sightRangeRock) && state != PURSUIT_ROCK) {
				SetPursuitRockState();
			}
			else if (hitbywave(dir_wave,wave_col_pos) && state != PURSUIT_PLAYER) {
				//std::cout << "change state to pursuit player!\n";
				SetPursuitPlayerState();
			}
			else if (seePlayer(sightRangePlayer)) {
				SetPursuitPlayerState();
			}
			else if (stateTimer <= 0.0f) { //idle
				SetIdleState();
			}
			else if (path.empty()) {
				EnterPatrolPath();
				SetPatrolState();
			}

			break;
		case PURSUIT_PLAYER:
			if (seeRock(p,sightRangeRock) && state != PURSUIT_ROCK) {
				SetPursuitRockState();
			}
			else if (!seePlayer(sightRangePlayer) && (stateTimer <= 0.0f)) { //player zao alr
				SetPatrolState();
			}

			break;
		case PURSUIT_ROCK:
			if (gridX == destX && gridY == destY || stateTimer<0.0f) {
				//std::cout << "Enemy reached the rock!\n";
				if (seePlayer(sightRangePlayer) && state != PURSUIT_PLAYER) {
					SetPursuitPlayerState();
					//std::cout << "Switching to PURSUIT_PLAYER.\n";
				}
				else{
					SetIdleState();
				 }
				
			
				
			}
			else if (hitbywave(dir_wave, wave_col_pos) && state != PURSUIT_PLAYER) {
				//std::cout << "change state to pursuit player!\n";
				SetPursuitPlayerState();
			}
			else if (seePlayer(sightRangePlayer) && state != PURSUIT_PLAYER) {
				SetPursuitPlayerState();
			}
			else if (!seePlayer(sightRangePlayer) || stateTimer <= 0.0f) { //player zao alr
				SetPatrolState();

			}

			break;
		}

		//call enemystate after update
		//std::cout << "enemy state is "<< GetEnemyStateName(e.state) << "\n";
		updateGridPosition();
		EnemyState(p,dt, (&MainWaves)[MAX_NUM_WAVES], mesh);
	}


	void Enemy::EnterPatrolPath() {
		// If the enemy is NOT at startPatrol, force it to go to startPatrol
		//std::cout << "calling  MovetoPatrolpoint\n";
		path.clear();
		pathIndex = 0;
		Pair enemyIndex = { gridX, gridY };
		if (astarsearch(maze, enemyIndex, startPatrol, path) == Path::NO_PATH) {
		}
	}


	bool Enemy::seePlayer(float sightRange) const {
		float dist_x = player.x - x;
		float dist_y = player.y - y;
		//return sqrt(dist_x * dist_x + dist_y * dist_y) <= sightRange;
		float distSq = dist_x * dist_x + dist_y * dist_y;
		float rangeSq = sightRange * sightRange;

		return distSq <= rangeSq;  // Compare squared values
	}

	bool Enemy::seeRock(Player& p,float sightRange) const {
		float dist_x = p.lastRockPos.x - x;
		float dist_y = p.lastRockPos.y - y;
		//return sqrt(dist_x * dist_x + dist_y * dist_y) <= sightRange;
		float distSq = dist_x * dist_x + dist_y * dist_y;
		float rangeSq = sightRange * sightRange;

		return distSq <= rangeSq;  // Compare squared values
	}


	//wave touch enemy
	bool Enemy::hitbywave(DirectionalWave dir_wave, AEVec2 & out) const{ //position of wave on col	
		//if hit by wave change state
		// enemy go to wave when hit by wave

		AEVec2 Enemy_Pos{ x,y };
			if (circle_square_collision(dir_wave.pos, dir_wave.scale.x, Enemy_Pos, w)) {
				//std::cout << "Enemy is hit by wave\n";
				out = dir_wave.pos;
				return true;
			}		
		return false;
	}


	/*
	* print functions
	*/
	void Enemy::printEnemyPatrolpoints() const {
		// Print sorted patrol points for confirmation
		//for (const Pair& p : patrolPath) {
			//std::cout << "Patrol point: (" << p.first << ", " << p.second << ")\n";
		//}
	}

	void Enemy::print_debug() const {
		std::cout << GetEnemyStateName() << "\n"; //print enemy state
		printPosition(); //print enemy position
		//std::cout << "state timer:" << stateTimer << "\n";
		//printEnemyPatrolpoints();

		for (Pair p : path) {
			std::cout << p.first << "," << p.second << "\n";
		}
	}


	void Enemy::SetIdleState() {

		state = IDLE;
		stateTimer = idle_timer;

	}
	void Enemy::SetPatrolState() {
	
		state = PATROL;
		stateTimer = patrol_timer;

	}
	void Enemy::SetPursuitPlayerState() {

		state = PURSUIT_PLAYER;
		stateTimer = pursuit_timer;
	}
	void Enemy::SetPursuitRockState() {

		state = PURSUIT_ROCK;
		stateTimer = pursuit_timer;
	}
	

	/*
	* noon-member functions
	* 
	*/
	

	Path astarsearch(int grid[MAX_ROW][MAX_COL], Pair src, Pair dst, std::vector<Pair>& path) {

		src = { src.second,src.first };
		dst = { dst.second,dst.first };

		path.clear(); //reset path
		if (!isValid(dst.first, dst.second, grid) || !isValid(src.first, src.second, grid)) {
			//std::cout << "Source or Destination is invalid\n";
			return Path::NO_PATH;
		}
		// If the destination cell is the same as source cell
		if (isDestination(src.first, src.second, dst)) {
			//std::cout << "Already at the destination\n";
			return Path::NO_PATH;
		}

		//ini closed list and grid nodes
		bool closedList[MAX_ROW][MAX_COL]{};
		//2D array
		Node gridNodes[MAX_ROW][MAX_COL]{};

		//initialize grid
		int i, j; //i is Y-axis , j is X-axis
		for (i = 0; i < MAX_ROW; i++) {
			for (j = 0; j < MAX_COL; j++) {
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
					//std::cout << "The destination cell is found\n";
					//printPath(gridNodes, dest);
					reconstructPath(gridNodes, dst, path);
					return Path::PATH_FOUND;
				}

				//processNeighbor(new_i, new_j, i, j, dst, gridNodes, closedList, grid, openList/*, path*/);
				processNeighbor(new_i, new_j, i, j, dst, gridNodes, closedList, openList);
			}
		}//end of openlist check

		//if (!path.empty()) {
		//	std::cout << "Astar found path" << "\n";
		//	return Path::PATH_FOUND;
		//}
		//else {
		//	std::cout << "Astar NO path" << "\n";
		//	return Path::NO_PATH;
		//}



		return Path::NO_PATH;
	}


	std::vector<Pair> findPatrolPoints(int grid[MAX_ROW][MAX_COL]) {
		std::vector<Pair> patrolPoints;

		for (int y = 0; y < MAX_ROW; y++) {       // Row (Y)
			for (int x = 0; x < MAX_COL; x++) { // Column (X)
				if (grid[y][x] == 9) {
					patrolPoints.push_back({ x, y });
				}
			}
		}

		// Z sorting, y first then X
		std::sort(patrolPoints.begin(), patrolPoints.end(), [](const Pair& a, const Pair& b) {
			// Check row y is odd or even
			bool isOddRow = (a.second % 2 == 1);
			if (a.second == b.second) {
				// Reverse X for odd rows, normal order for even rows
				return isOddRow ? a.first > b.first : a.first < b.first;
			}
			return a.second < b.second;  // Sort y
			});

		return patrolPoints;
	}




	/*
	* render functions
	*/
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