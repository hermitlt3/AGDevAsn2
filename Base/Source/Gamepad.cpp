#include "Gamepad.h"

#pragma comment(lib,"Xinput.lib")

XButtonIDs XButtons;

Gamepad::Gamepad()
{

}
Gamepad::Gamepad(int a_iIndex)
{
	m_iGamepadIndex = a_iIndex - 1;

	for (int i = 0; i < ButtonCount; i++)
	{
		bPrev_ButtonStates[i] = false;
		bButtonStates[i] = false;
		bGamepad_ButtonsDown[i] = false;
	}
}
Gamepad::~Gamepad()
{

}

XButtonIDs::XButtonIDs()
{
	// These values are used to index the XINPUT_Buttons array,
	// accessing the matching XINPUT button value

	A = 0;
	B = 1;
	X = 2;
	Y = 3;

	DPad_Up = 4;
	DPad_Down = 5;
	DPad_Left = 6;
	DPad_Right = 7;

	L_Shoulder = 8;
	R_Shoulder = 9;
	 
	L_Thumbstick = 10;
	R_Thumbstick = 11;

	Start = 12;
	Back = 13;
}

XINPUT_STATE Gamepad::Getstate()
{
	XINPUT_STATE GamepadState;

	//Zero Memory
	ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

	//Get the state of the gamepad
	XInputGetState(m_iGamepadIndex, &GamepadState);

	//Returns the state
	return GamepadState;
}

void Gamepad::RefreshState()
{
	memcpy(bPrev_ButtonStates, bButtonStates,
		sizeof(bPrev_ButtonStates));
}

//Return Gamepad Index
int Gamepad::Getindex()
{
	return m_iGamepadIndex;
}

bool Gamepad::Connected()
{
	//Zero memory
	ZeroMemory(&m_state,sizeof(XINPUT_STATE));

	//Get the state of the gamepad
	DWORD Result = XInputGetState(m_iGamepadIndex, &m_state);

	if (Result == ERROR_SUCCESS)
	{
		return true;	//Gamepad is connected
	}
	else
	{
		return false;	//It isn't connected
	}
}

void Gamepad::Update()
{
	m_state = Getstate(); //Obtains Current state

	for (int i = 0; i < ButtonCount; i++)
	{
		// Set button state for current frame
		bButtonStates[i] = (m_state.Gamepad.wButtons &
			XINPUT_Buttons[i]) == XINPUT_Buttons[i];

		// Set 'DOWN' state for current frame
		bGamepad_ButtonsDown[i] = !bPrev_ButtonStates[i] &&
			bButtonStates[i];
	}
}

bool Gamepad::LStick_InDeadzone()
{
	// Obtain the X & Y axes of the stick
	short sX = m_state.Gamepad.sThumbLX;
	short sY = m_state.Gamepad.sThumbLY;

	// X axis is outside of deadzone
	if (sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		return false;

	// Y axis is outside of deadzone
	if (sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		return false;

	// One (or both axes) axis is inside of deadzone
	return true;
}

// Deadzone check for Right Thumbstick
bool Gamepad::RStick_InDeadzone()
{
	// Obtain the X & Y axes of the stick
	short sX = m_state.Gamepad.sThumbRX;
	short sY = m_state.Gamepad.sThumbRY;

	// X axis is outside of deadzone
	if (sX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		sX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		return false;

	// Y axis is outside of deadzone
	if (sY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		sY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		return false;

	// One (or both axes) axis is inside of deadzone
	return true;
}


// Return X axis of left stick
float Gamepad::LeftStick_X()
{
	// Obtain X axis of left stick
	short sX = m_state.Gamepad.sThumbLX;

	// Return axis value, converted to a float
	return (static_cast<float>(sX) / 32768.0f);
}

// Return Y axis of left stick
float Gamepad::LeftStick_Y()
{
	// Obtain Y axis of left stick
	short sY = m_state.Gamepad.sThumbLY;

	// Return axis value, converted to a float
	return (static_cast<float>(sY) / 32768.0f);
}

// Return X axis of right stick
float Gamepad::RightStick_X()
{
	// Obtain X axis of right stick
	short sX = m_state.Gamepad.sThumbRX;

	// Return axis value, converted to a float
	return (static_cast<float>(sX) / 32768.0f);
}

// Return Y axis of right stick
float Gamepad::RightStick_Y()
{
	// Obtain the Y axis of the left stick
	short sY = m_state.Gamepad.sThumbRY;

	// Return axis value, converted to a float
	return (static_cast<float>(sY) / 32768.0f);
}

float Gamepad::LeftTrigger()
{
	// Obtain value of left trigger
	BYTE Trigger = m_state.Gamepad.bLeftTrigger;

	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return Trigger / 255.0f;

	return 0.0f; // Trigger was not pressed
}

// Return value of right trigger
float Gamepad::RightTrigger()
{
	// Obtain value of right trigger
	BYTE Trigger = m_state.Gamepad.bRightTrigger;

	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return Trigger / 255.0f;

	return 0.0f; // Trigger was not pressed
}

// Vibrate the gamepad (values of 0.0f to 1.0f only)
void Gamepad::Rumble(float a_fLeftMotor, float a_fRightMotor)
{
	// Vibration state
	XINPUT_VIBRATION VibrationState;

	// Zero memory
	ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

	// Calculate vibration values
	int iLeftMotor = int(a_fLeftMotor * 65535.0f);
	int iRightMotor = int(a_fRightMotor * 65535.0f);

	// Set vibration values
	VibrationState.wLeftMotorSpeed = iLeftMotor;
	VibrationState.wRightMotorSpeed = iRightMotor;

	// Set the vibration state
	XInputSetState(m_iGamepadIndex, &VibrationState);
}

// Return true if button is pressed, false if not
bool Gamepad::GetButtonPressed(int a_iButton)
{
	if (m_state.Gamepad.wButtons & XINPUT_Buttons[a_iButton])
	{
		return true; // The button is pressed
	}

	return false; // The button is not pressed
}

// Frame-specific version of 'GetButtonPressed' function
bool Gamepad::GetButtonDown(int a_iButton)
{
	return bGamepad_ButtonsDown[a_iButton];
}