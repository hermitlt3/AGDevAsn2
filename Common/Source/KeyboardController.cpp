#include "KeyboardController.h"

KeyboardController::KeyboardController()
{
}

KeyboardController::~KeyboardController()
{
}

void KeyboardController::UpdateKeyboardStatus(unsigned char _slot, bool _isPressed)
{
	currStatus.set(_slot, _isPressed);
}

void KeyboardController::EndFrameUpdate()
{
	prevStatus = currStatus;
}

bool KeyboardController::IsKeyDown(unsigned char _slot)
{
	return currStatus.test(_slot);
}

bool KeyboardController::IsKeyUp(unsigned char _slot)
{
	return !currStatus.test(_slot);
}

bool KeyboardController::IsKeyPressed(unsigned char _slot)
{
	return IsKeyDown(_slot) && !prevStatus.test(_slot);
}

bool KeyboardController::IsKeyReleased(unsigned char _slot)
{
	return IsKeyUp(_slot) && prevStatus.test(_slot);
}

int KeyboardController::ReturnKey()
{
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i) {
		if (IsKeyPressed(i))
			return i;
	}
}

bool KeyboardController::AnyKeyPressed()
{
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i) {
		if (IsKeyPressed(i))
			return true;
	}
}