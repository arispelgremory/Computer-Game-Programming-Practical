// Multibyte
// Not Set
// x86

//	Ask the compiler to include minimal header files for our program.
// #define -> Instruction to communicate with the compiler
#define WIN32_LEAN_AND_MEAN // Only include the skinny(LEAN) and powerful(MEAN) part of Windows.h
#include <Windows.h>
#include <d3d9.h>
#include <iostream>
//	include the D3DX9 library
#include <d3dx9.h>
//	include the Direct Input library.
#include <dinput.h>

#include "FrameTimer.h"
#include "Player.h"
using namespace std;

#pragma comment(lib, "d3d9.lib ")
#pragma comment(lib, "d3dx9.lib ")
#pragma comment(lib, "dinput8.lib ")
#pragma comment(lib, "dxguid.lib ")

#define WindowWidth 800
#define WindowHeight 600
#define BUTTONDOWN (name, key) (name.rgbButtons[key] & 0x80)

//--------------------------------------------------------------------

//	Window handle
HWND g_hWnd = NULL;
// Window's Structure  /DESIGN PATTERN SINGLETON
WNDCLASS wndClass;

// DX globals
IDirect3DDevice9* d3dDevice;

//	Direct Input object.
LPDIRECTINPUT8 dInput;

//	Direct Input keyboard device.
LPDIRECTINPUTDEVICE8  dInputKeyboardDevice;
LPDIRECTINPUTDEVICE8  dInputMouseDevice;
DIMOUSESTATE mouseState;

//	Key input buffer
BYTE  diKeys[256];

LPCSTR texturePath = "Assets/pointer.png";
LPCSTR texturePath1 = "Assets/space.png";

//Mouse Texture stuff
// LPDIRECT3DTEXTURE9 mouseTexture = NULL;
RECT mouseRect;
RECT mouseColRect;
D3DXVECTOR3 mousePosition;

LONG currentXpos;
LONG currentYpos;

// Game level globals
LPDIRECT3DTEXTURE9 playerTexture = NULL;
LPD3DXSPRITE spriteBrush = NULL;
LPD3DXSPRITE spriteFontBrush = NULL;
LPD3DXFONT font = NULL;
LPD3DXLINE line = NULL;

// Text
RECT textRect;

// Results
HRESULT hr;

// Player Actions
bool leftKeyPressed = false;
bool rightKeyPressed = false;
bool upKeyPressed = false;
bool downKeyPressed = false;

bool aKeyPressed = false;
bool dKeyPressed = false;
bool wKeyPressed = false;
bool sKeyPressed = false;

// Players
Player* player1 = NULL;
Player* player2 = NULL;

// timer
FrameTimer* timer = NULL;

// Game Settings
float gravity = 0;
float friction = 0;

int red = 0;
int green = 0;
int blue = 0;

//	Window Procedure, for event handling
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message){
		//	The message is post when we destroy the window.

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN: {
		switch (wParam) {
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
	}
}

bool CollisionDetection(RECT A, RECT B) {
	if (A.left > B.right) {
		return false;
	}
	if (A.right < B.left) {
		return false;
	}
	if (A.top > B.bottom) {
		return false;
	}
	if (A.bottom < B.top) {
		return false;
	}
	
	return false;
}

bool CircleCollisionDetection(int radiusA, int radiusB, D3DXVECTOR2 positionA, D3DXVECTOR2 positionB) {
	
	D3DXVECTOR2 distance = positionB - positionA;
	if (radiusA + radiusB < D3DXVec2Length(&distance)) {
		return false;
	}
	return true;
}

void CreateMyWindow(){
	//	Set all members in wndClass to 0.
	ZeroMemory(&wndClass, sizeof(wndClass));

	//	Filling wndClass. You are to refer to MSDN for each of the members details.
	//	These are the fundamental structure members to be specify, in order to create your window.
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.lpfnWndProc = WindowProcedure; // Long Pointer to a window procedure function
	wndClass.lpszClassName = "My Window";
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	//	Register the window.
	RegisterClass(&wndClass);

	//	You are to refer to MSDN for each of the parameters details.
	//  Create instance of window
	g_hWnd = CreateWindowEx(0, wndClass.lpszClassName, "My Window's Name", WS_OVERLAPPEDWINDOW, 0, 100, WindowWidth, WindowHeight, NULL, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(g_hWnd, 1);
}

void CreateMyDX() {
	//	Define Direct3D 9.
	IDirect3D9* direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	//	Define how the screen presents.
	D3DPRESENT_PARAMETERS d3dPP;
	ZeroMemory(&d3dPP, sizeof(d3dPP));

	//	Refer to Direct3D 9 documentation for the meaning of the members.
	d3dPP.Windowed = true;
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dPP.BackBufferCount = 1;
	d3dPP.BackBufferWidth = WindowWidth;
	d3dPP.BackBufferHeight = WindowHeight;
	d3dPP.hDeviceWindow = g_hWnd;

	//	Create a Direct3D 9 device.
	hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);

	if (FAILED(hr))
		cout << "Create DX failed";

	//	Create sprite. Study the documentation. 
	hr = D3DXCreateSprite(d3dDevice, &spriteBrush);

	if (FAILED(hr)) {
		cout << "Create sprite failed" << endl;
	}

	hr = D3DXCreateFont(d3dDevice, 25, 0, 0, 1, false,
		DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);
	
	if (FAILED(hr)) {
		cout << "Create font failed" << endl;
	}

	hr = D3DXCreateLine(d3dDevice, &line);
	if (FAILED(hr)) {
		cout << "Create line failed" << endl;
	}

}

void CreateMyDInput() {
	//	Create the Direct Input object.
	hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);

	if (FAILED(hr)) {
		cout << "Create direct input failed" << endl;
	}

	//	Create the keyboard device.
	hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);

	if (FAILED(hr)) {
		cout << "Create direct input device failed" << endl;
	}

	//	Set the input data format.
	hr = dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(hr)) {
		cout << "Failed to create keyboard data!" << endl;
	}

	//	Set the cooperative level.
	//	To Do:
	//	Try with different combination.
	hr = dInputKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr)) {
		cout << "Cooperative Level Failed!" << endl;
	}

    // Mouse stuffs
	hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);

	if (FAILED(hr)) {
		cout << "Create mouse failed!" << endl;
	}

	// Set data input format
	hr = dInputMouseDevice->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hr)) {
		cout << "Create mouse input data failed!" << endl;
	}
	// Check the return code for the SetDataFormat function
	
	hr = dInputKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    if (FAILED(hr)) {
        cout << "cooperative level fail" << endl;
    }

}

void InitializeLevel() {
	//	Create texture. Study the documentation.
	//hr = D3DXCreateTextureFromFile(d3dDevice, texturePath, &mouseTexture);
	//if (FAILED(hr)) {
		//cout << "Create mouse texture failed!" << endl;
	//}

	hr = D3DXCreateTextureFromFile(d3dDevice, texturePath1, &playerTexture);
	//hr = D3DXCreateTextureFromFileEx(/* Your Direct3D device */, "01.bmp", D3DX_DEFAULT, D3DX_DEFAULT, 
	//									D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
	//									D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), 
	//									NULL, NULL, &texture);

	if (FAILED(hr)) {
		cout << "Create player texture failed!" << endl;
	}

	// Mouse Initializations
	mouseRect.left = 0;
	mouseRect.right = 32;
	mouseRect.top = 0;
	mouseRect.bottom = 32;
	
    currentXpos = WindowWidth / 2;
	currentYpos = WindowHeight / 2;

	mousePosition.x = currentXpos;
	mousePosition.y = currentYpos;
	mousePosition.z = 0;

	// Mouse Collisions
	mouseColRect.left = 0;
	mouseColRect.top = 0;
	mouseColRect.right = 32;
	mouseColRect.bottom = 32;

    //text
    textRect.left = 100;
    textRect.top = 100;
    textRect.right = 450;
    textRect.bottom = 125;

	// Game Settings
	gravity = 2;
	friction = 0.01;

	// Players
	player1 = new Player();
	player2 = new Player();

	player1->Init(1, 100, 300);
	player2->Init(2, 600, 300);

	/*
	playerTextureWidth = 64;
	playerTextureHeight = 64;
	playerTextureCol = 2;
	playerTextureRow = 2;
	GetSpriteWidth() = playerTextureWidth / playerTextureCol;
	playerSpriteHeight = playerTextureHeight / playerTextureRow;
	playerSpriteFPS = 10;
	

	// Player 1
	player1->playerPosition = D3DXVECTOR2(700, 300);
	player1->playerVelocity = D3DXVECTOR2(0, 0);
	player1->playerAcceleration = D3DXVECTOR2(0, 0);
	player1EnginerForce = 1;
	player1->playerDirection = 0;
	player1->GetMass() = 1;
	player1->playerFrameCounter = 0;
	player1->playerNumber = 1;
	player1MaxFrame = 1;
	player2->playerRotationSpeed = 2;
	

	player1->playerAnimRect.top = 0;
	player1->playerAnimRect.bottom = player1->playerAnimRect.top + playerSpriteHeight;
	player1->playerAnimRect.left = (player1->playerNumber - 1) * GetSpriteWidth();
	player1->playerAnimRect.right = player1->playerAnimRect.left + GetSpriteWidth();

	// Player 2
	player2->playerPosition = D3DXVECTOR2(700, 300);
	player2->playerVelocity = D3DXVECTOR2(0, 0);
	player2->playerAcceleration = D3DXVECTOR2(0, 0);
	player2EnginerForce = 1;
	player2->playerDirection = 0;
	player2->GetMass() = 1;
	player2->playerFrameCounter = 0;
	player2Number = 2;
	player1MaxFrame = 2;
	player2->playerRotationSpeed = 2;

	player2->playerAnimRect.top = 0;
	player2->playerAnimRect.bottom = player2->playerAnimRect.top + playerSpriteHeight;
	player2->playerAnimRect.left = (player2Number - 1) * GetSpriteWidth();;
	player2->playerAnimRect.right = player2->playerAnimRect.left + GetSpriteWidth();
	*/
	

	

}

bool WindowIsRunning() {
	MSG msg;
	ZeroMemory(&msg, sizeof(msg)); // Set memory to zero (To clear the memory)

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//	Receive a quit message
		if (msg.message == WM_QUIT)
			return false;
		//	Translate the message 
		TranslateMessage(&msg);
		//	Send message to your window procedure
		DispatchMessage(&msg);
	}
	return true;
}

void GetInput() {
	//	Get immediate Keyboard Data.
	dInputKeyboardDevice->GetDeviceState(256, diKeys);
	//	Acquire the device.
	dInputKeyboardDevice->Acquire();
	dInputMouseDevice->Acquire();

	//player1 movement
	if (diKeys[DIK_UP] & 0x80) {
		if (player1->GetYPosition() == 0) {
			upKeyPressed = false;
		}
		else {
			upKeyPressed = true;
			cout << "Up Pressed!" << endl;
		}
	}

	if (diKeys[DIK_DOWN] & 0x80) {
		if (player1->GetYPosition() > WindowHeight - player1->GetSpriteWidth()) {
			downKeyPressed = false;
		}
		else {
			downKeyPressed = true;
			cout << "Down Pressed!" << endl;
		}
	}

	if (diKeys[DIK_LEFT] & 0x80) {
		if (player1->GetXPosition() == 0) {
			leftKeyPressed = false;
		}
		else {
			leftKeyPressed = true;
			cout << "Left Pressed!" << endl;
		}
	}

	if (diKeys[DIK_RIGHT] & 0x80) {
		if (player1->GetXPosition() > WindowWidth - player1->GetSpriteWidth()) {
			rightKeyPressed = false;
		}
		else {
			rightKeyPressed = true;
			cout << "Right Pressed!" << endl;
		}
	}

	//player2 movement
	if (diKeys[DIK_W] & 0x80) {
		if (player2->GetYPosition() == 0) {
			wKeyPressed = false;
		}
		else {
			wKeyPressed = true;
			cout << "W Pressed!" << endl;
		}
	}

	if (diKeys[DIK_S] & 0x80) {
		if (player2->GetYPosition() > WindowHeight - player2->GetSpriteWidth()) {
			sKeyPressed = false;
		}
		else {
			sKeyPressed = true;
			cout << "S Pressed!" << endl;
		}
	}

	if (diKeys[DIK_A] & 0x80) {
		if (player2->GetXPosition() == 0) {
			aKeyPressed = false;
		}
		else {
			aKeyPressed = true;
			cout << "A Pressed!" << endl;
		}
	}

	if (diKeys[DIK_D] & 0x80) {
		if (player2->GetXPosition() > WindowWidth - player2->GetSpriteWidth()) {
			dKeyPressed = false;
		}
		else {
			dKeyPressed = true;
			cout << "D Pressed!" << endl;
		}
	}

	dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
}

void Update(int framesToUpdate) {	

	for (int i = 0; i < framesToUpdate; i++) {
		//counter++;
		//player1
		if (leftKeyPressed) {
			//player1->playerDirection -= player1->playerRotationSpeed;
			player1->SetPlayerDirection(player1->GetPlayerDirection() - player1->GetPlayerRotationSpeed());
		}

		if (rightKeyPressed) {
			//player1->playerDirection += player1->playerRotationSpeed;
			player1->SetPlayerDirection(player1->GetPlayerDirection() + player1->GetPlayerRotationSpeed());
		}

		if (upKeyPressed) {
			//if (counter % timer->getFPS() / player1SpriteFPS) {
			//	player1->playerFrameCounter++
			//}
			// player1->playerAcceleration.x = sin(player1->playerDirection) * player1->playerEngineForce / player1->GetMass();
			player1->setPlayerAccelerationX(sin(player1->GetPlayerDirection()) * player1->GetPlayerEngineForce() / player1->GetMass());
			player1->setPlayerAccelerationY(-cos(player1->GetPlayerDirection()) * player1->GetPlayerEngineForce() / player1->GetMass());
			//player1->playerAcceleration.y = -cos(player1->playerDirection) * player1->playerEngineForce / player1->GetMass();
		}

		if (downKeyPressed) {
			player1->playerAcceleration.x = -sin(player1->playerDirection) * player1->playerEngineForce / player1->GetMass();
			player1->playerAcceleration.y = cos(player1->playerDirection) * player1->playerEngineForce / player1->GetMass();
		}

		player1->SetVelocity(player1->GetVelocity() + player1->GetAcceleration());
		//friction
		//player1->playerVelocity *= 1 - friction;
		player1->SetVelocity(player1->GetVelocity() * (1 - friction));

		player1->playerPosition += player1->playerVelocity;

		player1->playerFrameCounter++;
		if (player1->playerFrameCounter > player1->playerMaxFrame) {
			player1->playerFrameCounter = 0;
		}

		player1->playerAnimRect.left = (player1->playerNumber - 1) * player1->GetSpriteWidth();
		player1->playerAnimRect.top = player1->playerFrameCounter * player1->playerSpriteHeight;
		player1->playerAnimRect.right = player1->playerAnimRect.left + player1->GetSpriteWidth();
		player1->playerAnimRect.bottom = player1->playerAnimRect.top + player1->playerSpriteHeight;


		//player2
		if (aKeyPressed) {
			player2->playerDirection -= player2->playerRotationSpeed;
		}

		if (dKeyPressed) {
			player2->playerDirection += player2->playerRotationSpeed;
		}

		if (wKeyPressed) {
			/*if (counter % timer->getFPS() / player1SpriteFPS) {
				player1->playerFrameCounter++
			}*/
			player2->playerAcceleration.x = sin(player2->playerDirection) * player2->playerEngineForce / player2->GetMass();
			player2->playerAcceleration.y = -cos(player2->playerDirection) * player2->playerEngineForce / player2->GetMass();
		}
		if (sKeyPressed) {
			player2->playerAcceleration.x = -sin(player2->playerDirection) * player2->playerEngineForce / player2->GetMass();
			player2->playerAcceleration.y = cos(player2->playerDirection) * player2->playerEngineForce / player2->GetMass();
		}

		player2->playerVelocity += player2->playerAcceleration;
		//friction
		player2->playerVelocity *= 1 - friction;
		player2->playerPosition += player2->playerVelocity;

		player2->playerFrameCounter++;
		if (player2->playerFrameCounter > player2->playerMaxFrame) {
			player2->playerFrameCounter = 0;
		}

		player2->playerAnimRect.left = (player2->playerNumber - 1) * player2->GetSpriteWidth();
		player2->playerAnimRect.top = player2->playerFrameCounter * player2->playerSpriteHeight;
		player2->playerAnimRect.right = player2->playerAnimRect.left + player2->GetSpriteWidth();
		player2->playerAnimRect.bottom = player2->playerAnimRect.top + player2->playerSpriteHeight;

		//boundry
		if (player1->GetXPosition() <0 || player1->GetXPosition() > WindowWidth - player1->GetSpriteWidth()) {
			player1->playerVelocity.x *= -1.2;
		}
		if (player1->GetYPosition() <0 || player1->GetYPosition() > WindowHeight - player1->playerSpriteHeight) {
			player1->playerVelocity.y *= -1.2;
		}
		if (player2->GetXPosition() <0 || player2->GetXPosition() > WindowWidth - player2->GetSpriteWidth()) {
			player2->playerVelocity.x *= -1.2;
		}
		if (player2->GetYPosition() <0 || player2->GetYPosition() > WindowHeight - player2->playerSpriteHeight) {
			player2->playerVelocity.y *= -1.2;
		}
	}

	//player1
	leftKeyPressed = false;
	rightKeyPressed = false;
	upKeyPressed = false;
	player1->playerAcceleration = D3DXVECTOR2(0, 0);
	downKeyPressed = false;

	//palyer2
	aKeyPressed = false;
	dKeyPressed = false;
	wKeyPressed = false;
	player2->playerAcceleration = D3DXVECTOR2(0, 0);
	sKeyPressed = false;
}

void Render() {
	//	Clear the back buffer.
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//	Begin the scene
	d3dDevice->BeginScene();

	//	To Do:
	//	Drawing.
	//	Specify alpha blend will ensure that the sprite will render the background with alpha.
	spriteBrush->Begin(D3DXSPRITE_ALPHABLEND); //enable alpha value

	D3DXMATRIX mat;

	//draw player 1
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &player1->playerScaling, &player1->playerSpriteCenter, player1->playerDirection, &player1->playerPosition);
	spriteBrush->SetTransform(&mat);
	spriteBrush->Draw(playerTexture, &player1->playerAnimRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

	//draw player 2
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &player2->playerScaling, &player2->playerSpriteCenter, player2->playerDirection, &player2->playerPosition);
	spriteBrush->SetTransform(&mat);
	spriteBrush->Draw(playerTexture, &player2->playerAnimRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

	//	End sprite drawing
	spriteBrush->End();

	//	End the scene
	d3dDevice->EndScene();

	//	Present the back buffer to screen
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void CleanupMyLevel() {
	//mouseTexture->Release();
	//mouseTexture = NULL;
	playerTexture->Release();
	playerTexture = NULL;
}

void CleanUpMyDirectInput() {
	//	Release keyboard device.
	dInputKeyboardDevice->Unacquire();
	dInputKeyboardDevice->Release();
	dInputKeyboardDevice = NULL;

	//	Release DirectInput.
	dInput->Release();
	dInput = NULL;
}

void CleanupMyDx() {
	//	Release and clean up everything
	line->Release();
	line = NULL;

	//	Release and clean up everything

	font->Release();
	font = NULL;

	//	Release and clean up everything
	spriteBrush->Release();
	spriteBrush = NULL;
	
	//	Release the device when exiting.
	d3dDevice->Release();
	//	Reset pointer to NULL, a good practice.
	d3dDevice = NULL;
}

void CleanupMyWindow() {
	UnregisterClass(wndClass.lpszClassName, GetModuleHandle(NULL)); // Delete window class
}

//	use WinMain if you don't want the console
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) { // WinMain is a function in WINAPI
	/*																								
	   HINSTANCE hInstance - ID Number
	   HINSTANCE hPrevInstance - previous window
	   LPSTR lpCmdLine - Long pointer to a command line (ping.exe ->www.yahoo.com<-)
	   int nShowCmd - window mode (fullscreen, borderless)
	*/

	CreateMyWindow();
	CreateMyDX();
	CreateMyDInput();
	InitializeLevel();

	FrameTimer* timer = new FrameTimer();
	timer->Init(10);

	while (WindowIsRunning()) {	
		GetInput();
		Update(10);
		Render();
	}
	CleanupMyLevel();
	CleanUpMyDirectInput();
	CleanupMyDx();
	CleanupMyWindow();

	return 0;
}