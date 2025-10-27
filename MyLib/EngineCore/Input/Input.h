#pragma once
#include <dinput.h>

struct Inputs
{
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;
	//mouse
	IDirectInput8* directInputForMouse = nullptr;
	IDirectInputDevice8* mouse = nullptr;

	void Finalize();

};
