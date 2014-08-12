#pragma once

#include "ofMain.h"
#include "ofxXboxController.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void exit();

		ofxXboxController xbox;
		bool deadZone;
		Gamepad* gamepad;
		ofSpherePrimitive sphere;

};
