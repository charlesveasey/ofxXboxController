#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	
	xbox.setup();
	gamepad = xbox.getGamepadPtr(0);
	deadZone = true;
	
	sphere.setRadius(100);

	ofImage img;
	img.getTextureReference();
}

//--------------------------------------------------------------
void ofApp::update(){
	gamepad->leftVibration = gamepad->leftTrigger;
	gamepad->rightVibration = gamepad->rightTrigger;

	xbox.update();
	
	sphere.move(gamepad->thumbLX, -gamepad->thumbLY, 0);
	float ry = ofMap(gamepad->thumbRX, -1, 1, -360, 360); 
	sphere.setOrientation(ofVec3f(0, ry, 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(200,40,30);
	sphere.drawWireframe();

	ofSetColor(255);
	xbox.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'd'){
		for (int i=0; i<4; i++){
			xbox.enableDeadZone(i, !deadZone);
		}
	}
}

//--------------------------------------------------------------
void ofApp::exit(){ 
	xbox.exit();
}
