#pragma once

#include "ofMain.h"
#define MAX_CONTROLLERS 4  // XInput handles up to 4 controllers 
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.


class Gamepad{

	public: 
		bool connected;
		bool enableDeadZone;

		bool dPadUp;
		bool dPadDown;
		bool dPadLeft;
		bool dPadRight;
		bool start;
		bool back;
		bool leftThumb;
		bool rightThumb;
		bool leftShoulder;
		bool rightShoulder;
		bool a;
		bool b;
		bool x;
		bool y;
		float leftTrigger;
		float rightTrigger;
		float thumbLX;
		float thumbLY;
		float thumbRX;
		float thumbRY;
		float leftVibration;
		float rightVibration;
};


class ofxXboxController{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		Gamepad getGamepad(int i);
		Gamepad* getGamepadPtr(int i);
		void enableDeadZone(int i, bool val);

	private:
		vector<Gamepad> gamepads;
};
