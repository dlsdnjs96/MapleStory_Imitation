
#include "stdafx.h"

#include "Camera.h"
#include "Util.h"
#include "Player.h"

void CAMERA::Update()
{
	moveMainCam();
}

void CAMERA::moveMainCam()
{
	mainCam.Location = { Util::Lerp(mainCam.Location[0], PLAYER.GetPosition()[0], 0.3f), Util::Lerp(mainCam.Location[1], PLAYER.GetPosition()[1], 0.3f) };
	mainCam.Set();
}
