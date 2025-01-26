#include "AEEngine.h"
#include "Player.h"

void Boundary_Clamp(player* p, f32 max_x, f32 max_y, f32 min_x, f32 min_y) {
	p->x = AEClamp(p->x, min_x, max_x);
	p->y = AEClamp(p->y, min_y, max_y);
}