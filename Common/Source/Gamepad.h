#ifndef GAMEPAD_H_
#define GAMEPAD_H_
#include <windows.h>
#include <Xinput.h>

// XInput Button values
static const WORD XINPUT_Buttons[] = {
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK
};

// XInput Button IDs
struct XButtonIDs
{
	// Function prototypes

	XButtonIDs(); // Default constructor

	// Member variables

	int A, B, X, Y; // 'Action' buttons

	// Directional Pad (D-Pad)
	int DPad_Up, DPad_Down, DPad_Left, DPad_Right;

	// Shoulder ('Bumper') buttons
	int L_Shoulder, R_Shoulder;

	// Thumbstick buttons
	int L_Thumbstick, R_Thumbstick;

	int Start; // 'START' button
	int Back;  // 'BACK' button
};

class Gamepad
{
public:
	Gamepad();
	Gamepad(int a_iIndex);
	~Gamepad();

	void Update(); // Update state of controller
	void RefreshState();

	//Thumbstick Func
	bool LStick_InDeadzone();
	bool RStick_InDeadzone();

	float LeftStick_X();	//Returns X axis of Left stick
	float LeftStick_Y();	//Returns Y axis of Left stick
	float RightStick_X();	//Returns X axis of Right stick
	float RightStick_Y();	//Returns Y axis of Right stick

	//Trigger Func
	float LeftTrigger();
	float RightTrigger();

	//Button Func
	bool GetButtonPressed(int a_iButton);
	bool GetButtonDown(int a_iButton);

	//Vibrate Func
	void Rumble(float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f);

	//Utility Func
	XINPUT_STATE Getstate();	//Returns gamepadState
	int Getindex();				//Return gamepad index
	bool Connected();			//Return true if gamepad is connected
private:
	XINPUT_STATE m_state;
	int m_iGamepadIndex;
	static const int ButtonCount = 14;    // Total gamepad buttons
	bool bPrev_ButtonStates[ButtonCount]; // Previous frame button states
	bool bButtonStates[ButtonCount];      // Current frame button states

	// Buttons pressed on current frame
	bool bGamepad_ButtonsDown[ButtonCount];
};


#endif

extern XButtonIDs XButtons;