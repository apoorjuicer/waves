#include "AEEngine.h"
#include "Rock.h"
#include "Player.h"

namespace rock {
	AEVec2 rock(AEVec2& player) {
		s32 mouseX = 0, mouseY = 0;
		AEVec2 normalisedVec{ 0.0f, 0.0f };

		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			AEInputGetCursorPosition(&mouseX, &mouseY);
			AEVec2 mouse{ (f32)mouseX, (f32)mouseY };
			AEVec2 direction = { mouse.x - player.x, mouse.y - player.y };
			AEVec2Normalize(&normalisedVec, &direction);
		}

		return normalisedVec;

	}



}