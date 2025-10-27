#include "Input.h"

void Inputs::Finalize()
{
	keyboard->Unacquire();
	mouse->Unacquire();
	directInput->Release();
	directInputForMouse->Release();
	keyboard->Release();
	mouse->Release();
}