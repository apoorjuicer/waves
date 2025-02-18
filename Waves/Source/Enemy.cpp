#include "Enemy.h"


//// Creating a shortcut for int, int pair type
//typedef pair<int, int> Pair;
//
//// Creating a shortcut for pair<int, pair<int, int>> type
//typedef pair<double, pair<int, int> > pPair;

//int grid[ROW][COL] = {
//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
//	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
//	{ 1, 0, 1, 1, 1, 0, 1, 0, 1, 1 },
//	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
//	{ 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
//	{ 1, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
//	{ 1, 0, 1, 1, 1, 1, 0, 1, 0, 1 },
//	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
//	{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
//};

AEGfxVertexList* createEnemy_mesh(u32 color) {
	AEGfxMeshStart();
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

void drawEnemy(Enemy& enemy,u32 colour) {
	
	AEGfxVertexList* enemy_mesh = createEnemy_mesh(colour);
	RenderMesh(enemy_mesh, enemy.x, enemy.y, enemy.w, enemy.h, 0);
}



// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col)
{
	// Returns true if row number and column number
	// is in range
	return (row >= 0) && (row < ROW) && (col >= 0)
		&& (col < COL);
}

// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int grid[][COL], int row, int col)
{
	// Returns true if the cell is not blocked else false
	if (grid[row][col] == 0)
		return (true);
	else
		return (false);
}

// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Pair dest)
{
	// Return using the distance formula
	return ((double)sqrt(
		(row - dest.first) * (row - dest.first)
		+ (col - dest.second) * (col - dest.second)));
}

// A Utility Function to trace the path from the source
// to destination
void tracePath(cell cellDetails[][COL], Pair dest)
{
	printf("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;

	std::stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
		&& cellDetails[row][col].parent_j == col)) {
		Path.push(std::make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(std::make_pair(row, col));
	while (!Path.empty()) {
		Pair p = Path.top();
		Path.pop();
		printf("-> (%d,%d) ", p.first, p.second);
	}

	return;
}//end of tracepath

// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
void aStarSearch(int grid[][COL], Pair src, Pair dest)
{
	// If the source is out of range
	if (isValid(src.first, src.second) == false) {
		printf("Source is invalid\n");
		return;
	}

	// If the destination is out of range
	if (isValid(dest.first, dest.second) == false) {
		printf("Destination is invalid\n");
		return;
	}

	// Either the source or the destination is blocked
	if (isUnBlocked(grid, src.first, src.second) == false
		|| isUnBlocked(grid, dest.first, dest.second)
		== false) {
		printf("Source or the destination is blocked\n");
		return;
	}

	// If the destination cell is the same as source cell
	if (isDestination(src.first, src.second, dest)
		== true) {
		printf("We are already at the destination\n");
		return;
	}

	// Create a closed list and initialise it to false which
	// means that no cell has been included yet This closed
	// list is implemented as a boolean 2D array
	bool closedList[ROW][COL];
	memset(closedList, false, sizeof(closedList));

	// Declare a 2D array of structure to hold the details
	// of that cell
	cell cellDetails[ROW][COL];

	int i, j;

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	/*
	 Create an open list having information as-
	 <f, <i, j>>
	 where f = g + h,
	 and i, j are the row and column index of that cell
	 Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	 This open list is implemented as a set of pair of
	 pair.*/
	std::set<pPair> openList;

	// Put the starting cell on the open list and set its
	// 'f' as 0
	openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

	// We set this boolean value as false as initially
	// the destination is not reached.
	bool foundDest = false;

	while (!openList.empty()) {
		pPair p = *openList.begin();

		// Remove this vertex from the open list
		openList.erase(openList.begin());

		// Print current node being processed
		//printf("Visiting: (%d, %d)\n", p.first, p.second);


		// Add this vertex to the closed list
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		/*
		Generating all the 8 successor of this cell

			      N   
			      |   
				  |  
			W----Cell----E
				  | 
			      |  
			      S   

		Cell-->Popped Cell (i, j)
		N -->  North       (i-1, j)
		S -->  South       (i+1, j)
		E -->  East        (i, j+1)
		W -->  West           (i, j-1)
		//dont need diag movement
		N.E--> North-East  (i-1, j+1)
		N.W--> North-West  (i-1, j-1)
		S.E--> South-East  (i+1, j+1)
		S.W--> South-West  (i+1, j-1)*/

		// To store the 'g', 'h' and 'f' of the 4 successors
		double gNew, hNew, fNew;
	
		//----------- 1st Successor (North) ------------

		// Only process this cell if this is a valid one
		if (isValid(i - 1, j) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i - 1, j, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i - 1][j] == false
				&& isUnBlocked(grid, i - 1, j)
				== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i - 1, j, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i - 1][j].f == FLT_MAX
					|| cellDetails[i - 1][j].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i - 1, j)));

					// Update the details of this cell
					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
				}
			}
		}

		//----------- 2nd Successor (South) ------------

		// Only process this cell if this is a valid one
		if (isValid(i + 1, j) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i + 1, j, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i + 1][j] == false
				&& isUnBlocked(grid, i + 1, j)
				== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i + 1, j, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i + 1][j].f == FLT_MAX
					|| cellDetails[i + 1][j].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i + 1, j)));
					// Update the details of this cell
					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

		//----------- 3rd Successor (East) ------------

		// Only process this cell if this is a valid one
		if (isValid(i, j + 1) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i, j + 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}

			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i][j + 1] == false
				&& isUnBlocked(grid, i, j + 1)
				== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j + 1, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i][j + 1].f == FLT_MAX
					|| cellDetails[i][j + 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i, j + 1)));

					// Update the details of this cell
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}

		//----------- 4th Successor (West) ------------

		// Only process this cell if this is a valid one
		if (isValid(i, j - 1) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i, j - 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}

			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i][j - 1] == false
				&& isUnBlocked(grid, i, j - 1)
				== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j - 1, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i][j - 1].f == FLT_MAX
					|| cellDetails[i][j - 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i, j - 1)));

					// Update the details of this cell
					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}

	// When the destination cell is not found and the open
	// list is empty, then we conclude that we failed to
	// reach the destination cell. This may happen when the
	// there is no way to destination cell (due to
	// blockages)
	

	}//end of openliost check

	if (foundDest == false) {
		printf("Failed to find the Destination Cell\n");
	}

	return;
} //end of findSuccessor

void findSuccessor(int new_i, int new_j, int i, int j, Pair dest,
	cell cellDetails[][COL], bool closedList[][COL],
	int grid[][COL], std::set<pPair>& openList, bool& foundDest) {
	
	if (!isValid(new_i, new_j)) return;

	if (isDestination(new_i, new_j, dest)) {
		cellDetails[new_i][new_j].parent_i = i;
		cellDetails[new_i][new_j].parent_j = j;
		printf("The destination cell is found\n");
		tracePath(cellDetails, dest);
		foundDest = true;
		return;
	}

	if (!closedList[new_i][new_j] && isUnBlocked(grid, new_i, new_j)) {
		double gNew = cellDetails[i][j].g + 1.0;
		double hNew = calculateHValue(new_i, new_j, dest);
		double fNew = gNew + hNew;

		if (cellDetails[new_i][new_j].f == FLT_MAX || cellDetails[new_i][new_j].f > fNew) {
			openList.insert(std::make_pair(fNew, std::make_pair(new_i, new_j)));

			cellDetails[new_i][new_j].f = fNew;
			cellDetails[new_i][new_j].g = gNew;
			cellDetails[new_i][new_j].h = hNew;
			cellDetails[new_i][new_j].parent_i = i;
			cellDetails[new_i][new_j].parent_j = j;
		}
	}

}//end of findSuccessor



void Enemy::AstarSearch2(int grid[][COL], Pair src, Pair dest) {

	// If the source is out of range
	if (isValid(src.first, src.second) == false) {
		printf("Source is invalid\n");
		return;
	}
	// If the destination is out of range
	if (isValid(dest.first, dest.second) == false) {
		printf("Destination is invalid\n");
		return;
	}

	// Either the source or the destination is blocked
	if (isUnBlocked(grid, src.first, src.second) == false
		|| isUnBlocked(grid, dest.first, dest.second)
		== false) {
		printf("Source or the destination is blocked\n");
		return;
	}

	// If the destination cell is the same as source cell
	if (isDestination(src.first, src.second, dest)
		== true) {
		printf("We are already at the destination\n");
		return;
	}

	// Create a closed list and initialise it to false which
	// means that no cell has been included yet This closed
	// list is implemented as a boolean 2D array
	bool closedList[ROW][COL] = {};
	// Need to reset closedList before the second search!
	//std::fill(&closedList[0][0], &closedList[0][0] + ROW * COL, false);
	//memset(closedList, false, sizeof(closedList));

	// Declare a 2D array of structure to hold the details
	// of that cell
	cell cellDetails[ROW][COL];

	int i, j;

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	/*
	 Create an open list having information as-
	 <f, <i, j>>
	 where f = g + h,
	 and i, j are the row and column index of that cell
	 Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	 This open list is implemented as a set of pair of
	 pair.*/
	std::set<pPair> openList;

	// Put the starting cell on the open list and set its
	// 'f' as 0
	openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

	// We set this boolean value as false as initially
	// the destination is not reached.
	bool foundDest = false;
	while (!openList.empty()) {
		pPair p = *openList.begin();

		// Remove this vertex from the open list
		openList.erase(openList.begin());

		// Add this vertex to the closed list
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		findSuccessor(i - 1, j, i, j, dest, cellDetails, closedList, grid, openList, foundDest); // North
		if (foundDest) return;

		findSuccessor(i + 1, j, i, j, dest, cellDetails, closedList, grid, openList, foundDest); // South
		if (foundDest) return;

		findSuccessor(i, j + 1, i, j, dest, cellDetails, closedList, grid, openList, foundDest); // East
		if (foundDest) return;

		findSuccessor(i, j - 1, i, j, dest, cellDetails, closedList, grid, openList, foundDest); // West
		if (foundDest) return;

		// When the destination cell is not found and the open
		// list is empty, then we conclude that we failed to
		// reach the destination cell. This may happen when the
		// there is no way to destination cell (due to
		// blockages)


	}//end of openlist check

	if (foundDest == false) {
		printf("Failed to find the Destination Cell\n");
	}

	return;
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


