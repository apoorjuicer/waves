// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"
#include "Enemy.h"


// ---------------------------------------------------------------------------
// main
u32 white_colour = 0xFFFFFFFF;
u32 red_colour = 0xFFFF0000; // Full opacity, full red, no green, no blue

//f32 tilesize;
AEGfxVertexList* pDonutMesh;
AEGfxVertexList* PlayerMesh;
AEGfxVertexList* wallMesh;
AEGfxVertexList* pathMesh;
AEGfxVertexList* enemyMesh;
//printf("Hello World\n");
f32 MoveX;
f32 MoveY;
f32 dt;
//
Enemy enemy;

void init() {
	// tilesize = 100.0f;
	pDonutMesh = CreateDonutMesh(1000.0f, 200.0f, 48, 0x00000000);
	PlayerMesh = Player_Mesh(48, 0x00000000, 10.0f);
	enemyMesh = createEnemy_mesh(red_colour);
	//change first parameter for tile size
	if (!enemyMesh) {
		printf("error enemy mesh");
	}

	wallMesh = CreateSquareMesh(TILE_SIZE, 0xFF0000FF); // Blue for walls
	pathMesh = CreateSquareMesh(TILE_SIZE, 0xFFFFFFFF); // White for paths
	//printf("Hello World\n");
	 MoveX = 0.0f;
	 MoveY = 0.0f;


	 enemy.x = 100.f; //starting position x
	 enemy.y = -100.f; //starting position y
	 enemy.w = 50.f;
	 enemy.h = 50.f;	
}

void update() {


	//player movement
	//if (AEInputCheckCurr(AEVK_W)){MoveY += 1000 * dt;}
	//if (AEInputCheckCurr(AEVK_S)){MoveY -= 1000 * dt;}
	//if (AEInputCheckCurr(AEVK_A)){MoveX -= 1000 * dt;}
	//if (AEInputCheckCurr(AEVK_D)){MoveX += 1000 * dt;}

	//if (AEInputCheckCurr(AEVK_W)) { PlayerMovement(playermove, 0.0f, 100.0f, dt, maze, tilesize); }
	//if (AEInputCheckCurr(AEVK_S)) { PlayerMovement(playermove, 0.0f, -100.0f, dt, maze, tilesize); }
	//if (AEInputCheckCurr(AEVK_A)) { PlayerMovement(playermove, -100.0f, 0.0f, dt, maze, tilesize); }
	//if (AEInputCheckCurr(AEVK_D)) { PlayerMovement(playermove, 100.0f, 0.0f, dt, maze, tilesize); }

	//float dt = (float)AEFrameRateControllerGetFrameTime();
	dt = (f32)AEFrameRateControllerGetFrameTime();
	static float accumulatedTime = 0.0f;
	const float updateInterval = 0.1f;  // Move every 0.1 seconds
	// get dt for this frame
	accumulatedTime += dt;
	float speed = 250.0f; // Speed in units per second

	if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, dt, maze, TILE_SIZE); // Move up
	if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, dt, maze, TILE_SIZE);  // Move down
	if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, dt, maze, TILE_SIZE); // Move left
	if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, dt, maze, TILE_SIZE);  // Move right


	Pair src = {1, 1}, dest = { 8, 8 };

	if (!enemy.reachPath) {  // Only calculate path once
		astarsearch(maze, src, dest, enemy.path);
		enemy.reachPath = true;  //  Mark path as calculated
	}

	// Move enemy if enough time has passed
	if (accumulatedTime >= updateInterval) {
		if (enemy.pathIndex < enemy.path.size()) {
			enemy.moveEnemy();
			enemy.printPosition(); 
		}
		accumulatedTime = 0.0f; 
	}

}

void draw() {
	RenderMaze(maze, TILE_SIZE, wallMesh, pathMesh); // 100.0f is the tile size
	//RenderPlayer(player, PlayerMesh);
	//RenderEnemy_Mesh(enemyMesh, enemy.x, enemy.y, enemy.w, enemy.h, enemy.)

	//RenderMesh(enemyMesh, enemy.x, enemy.y, 1.0f, 1.0f, 0.f);
	//RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
	//RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
	if (!enemyMesh) {
		printf("Error: enemyMesh is nullptr\n");
	}
	else {
		RenderMesh(enemyMesh, enemy.x, enemy.y, enemy.w, enemy.h, 0.f);
	}

	AEGfxSetCamPosition(player.x, player.y);
	// Informing the system about the loop's end
}

void free(){
	//AEGfxVertexList* pDonutMesh;
	//AEGfxVertexList* PlayerMesh;
	//AEGfxVertexList* wallMesh;
	//AEGfxVertexList* pathMesh;
	AEGfxMeshFree(pDonutMesh);
	AEGfxMeshFree(PlayerMesh);
	AEGfxMeshFree(wallMesh);
	AEGfxMeshFree(pathMesh);
	AEGfxMeshFree(enemyMesh);
	
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);
	// Initialization of your own variables go here

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();
	init();

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetBackgroundColor(0.5, 0.5, 0.5);
		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Your own update logic goes here
	
		update();
		// Your own rendering logic goes here
		draw();
		AESysFrameEnd();
	}
	free();


	// free the system
	AESysExit();
}