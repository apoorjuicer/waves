#include "Enemy.h"
AEGfxVertexList* createEnemy_mesh(u32 color) {
	AEGfxMeshStart();
	//normalized mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, color, 0.0f, 1.0f,  // 
		0.5f, -0.5f, color, 1.0f, 1.0f,   // 
		-0.5f, 0.5f, color, 0.0f, 0.0f);  // 
	AEGfxTriAdd(
		0.5f, -0.5f, color, 1.0f, 1.0f,   // 
		0.5f, 0.5f, color, 1.0f, 0.0f,    // 
		-0.5f, 0.5f, color, 0.0f, 0.0f);  // 
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
	 //check cell (row, col) and grid[index] is valid
	bool isValid(int row, int col, int grid[][COL])
	{
		return ((row >= 0) && (row < ROW) && (col >= 0)
			&& (col < COL)) && grid[row][col] == 0;
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
			path.push_back({i,j});
			//update i and j to parent,if dont use temp bad diagonal movement occurs
			int temp_i = gridNodes[i][j].parent_i; 
			int temp_j = gridNodes[i][j].parent_j;
			i = temp_i;
			j = temp_j;
		}
		path.push_back({ i,j });
		std::reverse(path.begin(), path.end());
		// print path
		for (const Pair& p : path) {
			printf(":[%d,%d] ", p.first, p.second);
		}

	}//end of reconstructPath

	void processNeighbor(int new_i, int new_j, int i, int j, Pair dest,
		Node gridNodes[][COL], bool closedList[][COL],
		int grid[][COL], std::vector<pPair>& openList,  std::vector<Pair>& path) {

		//if (!isValid(new_i, new_j, grid)) { return ; }

		if (!closedList[new_i][new_j]) {
			double gNew = gridNodes[i][j].g + 1.0;
			double hNew = findHeuristic(new_i, new_j, dest);
			double fNew = gNew + hNew;

			//push unexplored nodes to openList
			if (gridNodes[new_i][new_j].f == VAL_INF || fNew < gridNodes[new_i][new_j].f) {
				openList.push_back({fNew,{new_i, new_j}}); //push to openlist
				std::sort(openList.begin(), openList.end(), sort_lowerF);

				gridNodes[new_i][new_j].f = fNew;
				gridNodes[new_i][new_j].g = gNew;
				gridNodes[new_i][new_j].h = hNew;
				gridNodes[new_i][new_j].parent_i = i;
				gridNodes[new_i][new_j].parent_j = j;
			}
		}
	}//end of processNeighbor

}// end of anon namespace

Path astarsearch(int grid[][COL], Pair src, Pair dest, std::vector<Pair>& path) {

	path.clear(); //reset path
	if (!isValid(dest.first, dest.second,grid) || !isValid(src.first, src.second, grid)) {
		printf("Source or Destination is invalid\n");
		return Path::NO_PATH;
	}
	// If the destination cell is the same as source cell
	if (isDestination(src.first, src.second, dest)){
		printf("Already at the destination\n");
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
			if (isDestination(new_i, new_j, dest)) {
				gridNodes[new_i][new_j].parent_i = i;
				gridNodes[new_i][new_j].parent_j = j;
				printf("The destination cell is found\n");
				//printPath(gridNodes, dest);
				reconstructPath(gridNodes, dest, path);
				return Path::PATH_FOUND;
			}

			processNeighbor(new_i, new_j, i, j, dest, gridNodes, closedList, grid, openList, path);

		}
	}//end of openlist check


	printf("Pathfinding failed");
	return Path::NO_PATH;
}

void EnemyState(Enemy& e) {
	switch (e.state) {
	case IDLE:
		std::cout << "Enemy is idle.\n";
		break;
	case PATROL:
		std::cout << "Enemy is patrolling.\n";
		 // Call function to move enemy
		break;

	case PURSUIT_PLAYER:
		std::cout << "Enemy is chasing player!\n";
		 // Implement chase behavior
		break;

	case PURSUIT_ROCK:
		std::cout << "Enemy is chasing rock!\n";
		// Implement rock pursuit behavior
		break;
	}
}

void UpdateEnemystate(Enemy& e) {
	switch (e.state) {
	case IDLE:break;
		if (/*enemy see rock*/1) {
			e.state = PURSUIT_ROCK;
		}
		else if (/*enemy see player*/1) {
			e.state = PURSUIT_ROCK;
		}
		else {
			/*probably need timer*/
			e.state = PATROL;
		}
	case PATROL:
		if (/*enemy see rock*/1) {
			e.state = PURSUIT_ROCK;
		}
		else if (/*enemy see player*/1) {
			e.state = PURSUIT_ROCK;
		}
		break;
	case PURSUIT_PLAYER:
		if (/*enemy see rock*/1) {
			e.state = PURSUIT_ROCK;
		}
		else if (/*!enemy see player*/1) {
			//e.state = PATROL/IDLE;
		}
		break;
	case PURSUIT_ROCK:
		if (/*at rock pos*/1) {
			//swap idle
		}
		break;
	}
}

//void Enemy::FindPath(int grid[][COL], Pair src, Pair dest) {
//	AstarSearch2(grid, src, dest);
//}

