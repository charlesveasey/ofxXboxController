#pragma once
#include "ofxXboxController.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <commdlg.h>
#include <basetsd.h>
#include <objbase.h>

#ifdef USE_DIRECTX_SDK
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\include\xinput.h>
#pragma comment(lib,"xinput.lib")
#elif (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------

struct CONTROLLER_STATE{
    XINPUT_STATE state;
    bool bConnected;
    XINPUT_VIBRATION vibration;
};

CONTROLLER_STATE g_Controllers[MAX_CONTROLLERS];
WCHAR	g_szMessage[4][1024] = {0};

//--------------------------------------------------------------
void ofxXboxController::setup(){
	HINSTANCE hInstance;
	hInstance = GetModuleHandle(NULL);

   // Initialize COM
    HRESULT hr;
    if( FAILED( hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED) ) )
        return;

    // Init state
    ZeroMemory( g_Controllers, sizeof( CONTROLLER_STATE ) * MAX_CONTROLLERS );

	#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) || defined(USE_DIRECTX_SDK)
		XInputEnable( TRUE );
	#endif


    for( DWORD i = 0; i < MAX_CONTROLLERS; i++ ){
		Gamepad g;
		gamepads.push_back(g);
		gamepads[i].enableDeadZone = true;
	}
}

//--------------------------------------------------------------
void ofxXboxController::update(){
    DWORD dwResult;
    for(DWORD i = 0; i < MAX_CONTROLLERS; i++){
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState( i, &g_Controllers[i].state );

        if( dwResult == ERROR_SUCCESS )
            g_Controllers[i].bConnected = true;
        else
            g_Controllers[i].bConnected = false;
    }

	WCHAR sz[4][1024];
    for( DWORD i = 0; i < MAX_CONTROLLERS; i++ ){

        if( g_Controllers[i].bConnected ) {

            WORD wButtons = g_Controllers[i].state.Gamepad.wButtons;

            if( gamepads[i].enableDeadZone ){

                // Zero value if thumbsticks are within the dead zone 
                if( ( g_Controllers[i].state.Gamepad.sThumbLX < INPUT_DEADZONE &&
                      g_Controllers[i].state.Gamepad.sThumbLX > -INPUT_DEADZONE ) &&
                    ( g_Controllers[i].state.Gamepad.sThumbLY < INPUT_DEADZONE &&
                      g_Controllers[i].state.Gamepad.sThumbLY > -INPUT_DEADZONE ) )
                {
                    g_Controllers[i].state.Gamepad.sThumbLX = 0;
                    g_Controllers[i].state.Gamepad.sThumbLY = 0;
                }

                if( ( g_Controllers[i].state.Gamepad.sThumbRX < INPUT_DEADZONE &&
                      g_Controllers[i].state.Gamepad.sThumbRX > -INPUT_DEADZONE ) &&
                    ( g_Controllers[i].state.Gamepad.sThumbRY < INPUT_DEADZONE &&
                      g_Controllers[i].state.Gamepad.sThumbRY > -INPUT_DEADZONE ) )
                {
                    g_Controllers[i].state.Gamepad.sThumbRX = 0;
                    g_Controllers[i].state.Gamepad.sThumbRY = 0;
                }
            }
        }

	}

	for( DWORD i = 0; i < MAX_CONTROLLERS; i++ ){

		gamepads[i].connected = g_Controllers[i].bConnected;

		if (gamepads[i].connected){

			WORD wButtons = g_Controllers[i].state.Gamepad.wButtons;
			gamepads[i].dPadUp = (wButtons & XINPUT_GAMEPAD_DPAD_UP ) ? true : false;
			gamepads[i].dPadDown = ( wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) ? true : false;
			gamepads[i].dPadLeft = ( wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) ? true : false;
			gamepads[i].dPadRight = ( wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) ? true : false;
			gamepads[i].start = ( wButtons & XINPUT_GAMEPAD_START ) ? true : false;
			gamepads[i].back = ( wButtons & XINPUT_GAMEPAD_BACK ) ? true : false;
			gamepads[i].leftThumb = ( wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) ? true : false;
			gamepads[i].rightThumb = ( wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) ? true : false;
			gamepads[i].leftShoulder = ( wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ) ? true : false;
			gamepads[i].rightShoulder = ( wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) ? true : false;
			gamepads[i].a = ( wButtons & XINPUT_GAMEPAD_A ) ? true : false;
			gamepads[i].b = ( wButtons & XINPUT_GAMEPAD_B ) ? true : false;
			gamepads[i].x = ( wButtons & XINPUT_GAMEPAD_X ) ? true : false;
			gamepads[i].y = ( wButtons & XINPUT_GAMEPAD_Y ) ? true : false;
			gamepads[i].leftTrigger = ofMap(g_Controllers[i].state.Gamepad.bLeftTrigger, 0, 255, 0, 1);
			gamepads[i].rightTrigger = ofMap(g_Controllers[i].state.Gamepad.bRightTrigger, 0, 255, 0, 1);
			gamepads[i].thumbLX = ofMap(g_Controllers[i].state.Gamepad.sThumbLX, -32767, 32767, -1, 1);
			gamepads[i].thumbLY = ofMap(g_Controllers[i].state.Gamepad.sThumbLY, -32767, 32767, -1, 1);
			gamepads[i].thumbRX = ofMap(g_Controllers[i].state.Gamepad.sThumbRX, -32767, 32767, -1, 1);
			gamepads[i].thumbRY = ofMap(g_Controllers[i].state.Gamepad.sThumbRY, -32767, 32767, -1, 1);

			if (gamepads[i].thumbLX < -1) gamepads[i].thumbLX = -1;
			if (gamepads[i].thumbLY < -1) gamepads[i].thumbLY = -1;
			if (gamepads[i].thumbRX < -1) gamepads[i].thumbRX = -1;
			if (gamepads[i].thumbRY < -1) gamepads[i].thumbRY = -1;

			g_Controllers[i].vibration.wLeftMotorSpeed = (WORD)ofMap(gamepads[i].leftVibration, 0, 1, 0, 65535);
			g_Controllers[i].vibration.wRightMotorSpeed = (WORD)ofMap(gamepads[i].rightVibration, 0, 1, 0, 65535);

			XInputSetState( i, &g_Controllers[i].vibration );

		}
	}

}

//--------------------------------------------------------------
void ofxXboxController::draw(){
	WCHAR sz[4][1024];

	ofDrawBitmapString("The state of all 4 XInput controllers\nPress 'd' to toggle dead zone clamping.", 20, 20);
    
	for( DWORD i = 0; i < MAX_CONTROLLERS; i++ ){

		string cval = gamepads[i].connected ? "true" : "false";

        ofDrawBitmapString( "Controller " + ofToString(i) + ": " + cval, 20, 15*i+60 );

		WORD wButtons = g_Controllers[i].state.Gamepad.wButtons;

		if(gamepads[i].connected) {

			swprintf_s( sz[i], 1024,
								L"Controller %u: Connected \n" 
								L"  Buttons: %s%s%s%s%s%s%s%s%s%s%s%s%s%s\n"
								L"  Left Trigger: %f\n"
								L"  Right Trigger: %f\n"
								L"  Left Thumbstick: %f/%f \n"
								L"  Right Thumbstick: %f/%f", i,
								( wButtons & XINPUT_GAMEPAD_DPAD_UP ) ? L"DPAD_UP " : L"",
								( wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) ? L"DPAD_DOWN " : L"",
								( wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) ? L"DPAD_LEFT " : L"",
								( wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) ? L"DPAD_RIGHT " : L"",
								( wButtons & XINPUT_GAMEPAD_START ) ? L"START " : L"",
								( wButtons & XINPUT_GAMEPAD_BACK ) ? L"BACK " : L"",
								( wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) ? L"LEFT_THUMB " : L"",
								( wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) ? L"RIGHT_THUMB " : L"",
								( wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ) ? L"LEFT_SHOULDER " : L"",
								( wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) ? L"RIGHT_SHOULDER " : L"",
								( wButtons & XINPUT_GAMEPAD_A ) ? L"A " : L"",
								( wButtons & XINPUT_GAMEPAD_B ) ? L"B " : L"",
								( wButtons & XINPUT_GAMEPAD_X ) ? L"X " : L"",
								( wButtons & XINPUT_GAMEPAD_Y ) ? L"Y " : L"",
								gamepads[i].leftTrigger,
								gamepads[i].rightTrigger,
								gamepads[i].thumbLX,
								gamepads[i].thumbLY,
								gamepads[i].thumbRX,
								gamepads[i].thumbRY);


			//convert from wide char to narrow char array
			char ch[1024];
			char DefChar = ' ';
			WideCharToMultiByte(CP_ACP,0,sz[i],-1, ch,260,&DefChar, NULL);
    
			//A std:string  using the char* constructor.
			std::string ss(ch);

			//draw string
			ofDrawBitmapString( ss, 20, 100*i+150 );
		}

    }

}

//--------------------------------------------------------------
void ofxXboxController::exit(){ 

	for( DWORD i = 0; i < MAX_CONTROLLERS; i++ ){
		g_Controllers[i].vibration.wLeftMotorSpeed = 0;
		g_Controllers[i].vibration.wRightMotorSpeed = 0;
		XInputSetState( i, &g_Controllers[i].vibration );
	}

	CoUninitialize();
}

//--------------------------------------------------------------
Gamepad ofxXboxController::getGamepad(int i){ 
	if (i < 0) i = 0;
	if (i > 3) i = 3;
	return gamepads[i];
}

//--------------------------------------------------------------
Gamepad* ofxXboxController::getGamepadPtr(int i){ 
	if (i < 0) i = 0;
	if (i > 3) i = 3;
	return &gamepads[i];
}

//--------------------------------------------------------------
void ofxXboxController::enableDeadZone(int i, bool val){ 
	if (i < 0) i = 0;
	if (i > 3) i = 3;
	gamepads[i].enableDeadZone = val;
}