// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "draw_map.h"
#include "enemy_henrey.h"
#include "Player.h"
#include <cstdlib>
#include "boundary.h"
#include <iostream>
// ---------------------------------------------------------------------------
// main
f32 max_x;
f32 max_y;
f32 min_x;
f32 min_y;
player Player;
//f32 &MoveX =Player.position.x;
//f32& MoveY =Player.position.y;
f32 player_scalex;
f32 player_scaley;
f32 player_radius;
f32 dt;
f32 wallW;
f32 wallH;
bool collidePtoRect;

AEGfxVertexList* wallMesh;
AEGfxVertexList* boundaryMesh;
AEGfxVertexList* enemy1;
//
AEGfxVertexList* pDonutMesh;
AEGfxVertexList* PlayerMesh;
//AEGfxVertexList* testing;
//

Rect mid_obs;
bool collide_mid_obs = false;

void init() {
	wallMesh = createWallMesh(0xFF008080);
	boundaryMesh  = createBoundaryMesh(0xFF654321);
	enemy1 = createEnemyMesh(0xFFFF0000);
	player_scalex = 10.f;
	player_scaley = 10.f;
	player_radius = 10.f;
	wallW = 600.f;
	wallH = 300.f;
	//wall, = { {0.f,0.f},wallW,wallH };
	mid_obs.center.x = 0.f;
	mid_obs.center.y = 0.f;
	mid_obs.width = wallW;
	mid_obs.height = wallH;
	//Player, = { {-400.f,250.f}, 10.f};
	Player.position.x = -400.f;
	Player.position.y = 250.f;
	Player.radius = 10.f;

	addWall(0.0f, 0.0f, wallW, wallH); //draw wall
	addBoundary(0.0f, 300.0f, 1000.0f, 50.0f);
	addBoundary(-520.0f, 0.0f, 50.0f, 650.0f);
	addBoundary(0.0f, -300.0f, 1000.0f, 50.0f);
	addBoundary(520.0f, 0.0f, 50.0f, 650.0f);

	// Test the walls array
	for (int i = 0; i < wallCount; i++) {
		printf("Wall %f: x=%f, y=%f, width=%f, height=%f\n", i, wallList[i].x, wallList[i].y, wallList[i].scale_x, wallList[i].scale_y);
	}

	// Test the boundaries array
	for (int i = 0; i < boundaryCount; i++) {
		printf("Boundary %f: x=%f, y=%f, width=%f, height=%f\n", i, boundaryList[i].x, boundaryList[i].y, boundaryList[i].scale_x, boundaryList[i].scale_y);
	}
	pDonutMesh = CreateDonutMesh(1000.0f, 200.0f, 48, 0x00000000);
	PlayerMesh = Player_Mesh(48, 0x00000000);
	 //testing = testing_only(); //to delete for testing only

	//printf("Hello World\n");
	// MoveX = -400.0f;
	// MoveY = 250.0f;
	  max_x = 520-35;
	  max_y = 300 - 35;
	  min_x = -520 + 35;
	  min_y = -300 + 35;
  
	 collidePtoRect = false;
	 
	 dt = (f32)AEFrameRateControllerGetFrameTime();
}

void update() {
	//player movement

	//AEVec2 playerCenter = { player_scalex, player_scaley };
	//AEVec2 mapCenter = { wallW,wallH };
	//collidePtoRect = AETestCircleToRect(
	//	&playerCenter, player_radius * player_scalex,
	//	&mapCenter, mid_obs.width, mid_obs.height);

	//printf("Player Center: (%.2f, %.2f)\n", playerCenter.x, playerCenter.y);
	//printf("Player Radius: %.2f\n", player_radius * player_scalex);
	//printf("Map Center: (%.2f, %.2f)\n", mapCenter.x, mapCenter.y);
	//printf("Map Size: (%.2f, %.2f)\n", wallW, wallH);

	if (AEInputCheckCurr(AEVK_W))
	{
		Player.position.y += 1000 * dt;
	}
	if (AEInputCheckCurr(AEVK_S))
	{
		Player.position.y -= 1000 * dt;
	}
	if (AEInputCheckCurr(AEVK_A))
	{
		Player.position.x -= 1000 * dt;
	}
	if (AEInputCheckCurr(AEVK_D))
	{
		Player.position.x += 1000 * dt;
	}
	AEVec2 movement = { 0.0f, 0.0f };
	// Normalize movement if diagonal
	f32 length = sqrt(movement.x * movement.x + movement.y * movement.y);
	if (length > 0.0f) {
		movement.x /= length;
		movement.y /= length;
	}

	Player.position.x += movement.x * 1000 * dt;
	Player.position.y += movement.y * 1000 * dt;

	

	AEVec2 playerCenter = { Player.position.x, Player.position.y };
	AEVec2 obsCenter = { mid_obs.center.x,mid_obs.center.y };

	collide_mid_obs = AETestCircleToRect(
		&playerCenter, player_radius,
		&obsCenter, mid_obs.width, mid_obs.height);

	if (collide_mid_obs) {
		std::cout << "\ncollide_mid_obs :" << collide_mid_obs;
		collide_obstacle(Player, mid_obs);
	}
	else {

	}

	Boundary_Clamp(Player, max_x, max_y, min_x, min_y);
}

void draw() {
	drawWalls(wallMesh);
	drawBoundaries(boundaryMesh);
	RenderMesh(enemy1, 450.0f, -200.0f, 50.0f, 50.0f, 0.0f);
	//drawing the donut mesh
	RenderMesh(PlayerMesh, Player.position.x, Player.position.y, player_scalex, player_scaley, 45.0f);
	//RenderMesh(testing, 0.0f, 0.0f, 100, 100, 0.0f);//to delete for testing only
	RenderMesh(pDonutMesh, Player.position.x, Player.position.y, 1.0f, 1.0f, 45.0f);

	AEGfxSetCamPosition(Player.position.x, Player.position.y);
}


void free() {
	AEGfxMeshFree(wallMesh);
	AEGfxMeshFree(boundaryMesh);
	AEGfxMeshFree(enemy1);
	free(wallList);
	free(boundaryList);
	AEGfxMeshFree(pDonutMesh);
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

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);
	
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
		// Informing the system about the loop's end
		AESysFrameEnd();
	}

	free();
	// free the system
	AESysExit();
}