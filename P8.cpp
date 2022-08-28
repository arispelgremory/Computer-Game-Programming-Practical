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

#pragma comment(lib, "d3d9.lib ")
#pragma comment(lib, "d3dx9.lib ")
#pragma comment(lib, "dinput8.lib ")
#pragma comment(lib, "dxguid.lib ")

using namespace std;

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
LPCSTR texturePath1 = "Assets/practical9.png";

int red = 0;
int green = 0;
int blue = 0;

LONG currentXpos;
LONG currentYpos;

enum movementDirection { moveDown, moveLeft, moveRight, moveUp, idle };

//Mouse Texture stuff
LPDIRECT3DTEXTURE9 mouseTexture = NULL;
RECT mouseRect;
RECT mouseColRect;
D3DXVECTOR3 mousePosition;


// Game level globals
LPDIRECT3DTEXTURE9 playerTexture = NULL;

LPD3DXSPRITE spriteBrush = NULL;
LPD3DXSPRITE spriteFontBrush = NULL;


/*
// Player 1 & 2 shares

RECT spriteRect1;
D3DXVECTOR3 position1;

int playerTextureWidth = 0;
int playerTextureHeight = 0;
int playerTextureCol = 0;
int playerTextureRow = 0;
int playerSpriteWidth = 0;
int playerSpriteHeight = 0;
int playerSpriteFPS = 0;
int maxFrame = 1;

float friction = 0.1;

// player 1
RECT player1ColRect;
RECT player1ColRect;
D3DXVECTOR2 player1.playerPosition;
D3DXVECTOR2 player1.playerVelocity;
D3DXVECTOR2 player1.playerAcceleration;
D3DXVECTOR2 player1Scaling;
D3DXVECTOR2 player1SpriteCenter;


float player1EngineForce = 0;
float player1Direction = 0;
float player1Mass = 0;
int player1FrameCounter = 0;
int player1Number = 0;
float player1RotationSpeed = 0;


// Player 2
RECT player2ColRect;
RECT player2ColRect;
D3DXVECTOR2 player2Position;
D3DXVECTOR2 player2Velocity;
D3DXVECTOR2 player2Acceleration;
D3DXVECTOR2 player2Scaling;
D3DXVECTOR2 player2SpriteCenter;

float player2EngineForce = 0;
float player2Direction = 0;
float player2Mass = 0;
int player2FrameCounter = 0;
int player2Number = 0;
float player2RotationSpeed = 0;

*/

Player player1;
Player player2;

HRESULT hr;

int counter = 0;
int delayFactor = 0;

float gravity = 0;
float friction = 0;
float playerVelocityY = 0;


LPD3DXFONT font = NULL;
LPD3DXLINE line = NULL;

boolean leftKeyPressed = false;
boolean rightKeyPressed = false;
boolean upKeyPressed = false;
boolean downKeyPressed = false;
boolean spaceKeyPressed = false;

// Clock

float currentTime;
int needleLength = 200;

D3DXVECTOR2 startpoint;
D3DXVECTOR2 endpoint;
D3DXVECTOR2 lineVertices[] = { startpoint, endpoint };


//--------------------------------------------------------------------



//	Window Procedure, for event handling
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // Get messages from the OS(mouse was move,keyboard was press)
{
	switch (message) {
		//	The message is post when we destroy the window.
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		/*
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_LEFT:
			leftKeyPressed = true;
			currentDirection = moveLeft;
			cout << "lEFT KEY WAS PRESSED" << endl;
			break;
		case VK_RIGHT:
			rightKeyPressed = true;
			currentDirection = moveRight;
			cout << "RIGHT KEY WAS PRESSED" << endl;
			break;
		case VK_UP:
			upKeyPressed = true;
			currentDirection = moveUp;
			cout << "UP KEY WAS PRESSED" << endl;
			break;
		case VK_DOWN:
			downKeyPressed = true;
			currentDirection = moveDown;
			cout << "DOWN KEY WAS PRESSED" << endl;
			break;
		case VK_SPACE:
			spaceKeyPressed = true;

			break;
		**/

		case 'R':
			red += 5;
			cout << "Red: " << red<< endl;
			break;
		case 'G':
			green += 5;
			cout << "Green: " << green << endl;
			break;
		case 'B':
			blue += 5;
			cout << "Blue: " << blue << endl;
			break;
		case '1':
			texturePath = "Assets/bg1.png";
			cout << "Shit" << endl;
			break;
		default:
			break;
		}
		break;
		//	Default handling for other messages.
	/*
	case WM_MOUSEMOVE:
		position.y = (short)HIWORD(lParam);
		position.x = (short)LOWORD(lParam);
		break;
	*/
	default:
		return DefWindowProc(hWnd, message, wParam, lParam); // Return messages back to the OS
	}
	return 0;
}
//--------------------------------------------------------------------

//	use int main if you want to have a console to print out message
//int main()

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

bool WindowIsRunning(){
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

void CleanupMyLevel() {
	mouseTexture->Release();
	mouseTexture = NULL;
	playerTexture->Release();
	playerTexture = NULL;
}

void CleanupMyWindow(){
	UnregisterClass(wndClass.lpszClassName, GetModuleHandle(NULL)); // Delete window class
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

	hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);

	if (FAILED(hr)) {
		cout << "Create direct input mouse failed!" << endl;
	}

	
	hr = dInputMouseDevice->SetDataFormat(&c_dfDIMouse);

	// Check the return code for the SetDataFormat function
	
	if (FAILED(hr)) {
		cout << "Create direct input mouse failed" << endl;
	}



}

void InitializeLevel() {
	//	Create texture. Study the documentation.
	hr = D3DXCreateTextureFromFile(d3dDevice, texturePath, &mouseTexture);
	if (FAILED(hr)) {
		cout << "Create mouse texture failed!" << endl;
	}

	hr = D3DXCreateTextureFromFileEx(d3dDevice, texturePath1, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), NULL, NULL, &playerTexture);
	//hr = D3DXCreateTextureFromFileEx(/* Your Direct3D device */, "01.bmp", D3DX_DEFAULT, D3DX_DEFAULT, 
	//									D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
	//									D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), 
	//									NULL, NULL, &texture);

	if (FAILED(hr)) {
		cout << "Create player texture failed!" << endl;
	}

	// Mouse Initializations
	mouseRect.left = 0;
	mouseRect.right = 35;
	mouseRect.top = 0;
	mouseRect.bottom = 35;
	
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

	// Game Settings
	gravity = 0.98;
	friction = 0.01;

	// Players
	Player* player1 = new Player();
	Player* player2 = new Player();

	player1->init();
	player2->init();

	

	/*
	playerTextureWidth = 64;
	playerTextureHeight = 64;
	playerTextureCol = 2;
	playerTextureRow = 2;
	playerSpriteWidth = playerTextureWidth / playerTextureCol;
	playerSpriteHeight = playerTextureHeight / playerTextureRow;
	playerSpriteFPS = 10;
	

	// Player 1
	player1.playerPosition = D3DXVECTOR2(700, 300);
	player1.playerVelocity = D3DXVECTOR2(0, 0);
	player1.playerAcceleration = D3DXVECTOR2(0, 0);
	player1EnginerForce = 1;
	player1Direction = 0;
	player1Mass = 1;
	player1FrameCounter = 0;
	player1Number = 1;
	player1MaxFrame = 1;
	player2RotationSpeed = 2;
	

	player1AnimRect.top = 0;
	player1AnimRect.bottom = player1AnimRect.top + playerSpriteHeight;
	player1AnimRect.left = (player1Number - 1) * playerSpriteWidth;
	player1AnimRect.right = player1AnimRect.left + playerSpriteWidth;

	// Player 2
	player2Position = D3DXVECTOR2(700, 300);
	player2Velocity = D3DXVECTOR2(0, 0);
	player2Acceleration = D3DXVECTOR2(0, 0);
	player2EnginerForce = 1;
	player2Direction = 0;
	player2Mass = 1;
	player2FrameCounter = 0;
	player2Number = 2;
	player1MaxFrame = 2;
	player2RotationSpeed = 2;

	player2AnimRect.top = 0;
	player2AnimRect.bottom = player2AnimRect.top + playerSpriteHeight;
	player2AnimRect.left = (player2Number - 1) * playerSpriteWidth;;
	player2AnimRect.right = player2AnimRect.left + playerSpriteWidth;
	*/
	

	

}

void GetInput() {
	//	Acquire the device.
	dInputKeyboardDevice->Acquire();
	dInputMouseDevice->Acquire();


	//previousDirection = currentDirection;

	//	Get immediate Keyboard Data.
	dInputKeyboardDevice->GetDeviceState(256, diKeys);
	if (diKeys[DIK_UP] & 0x80) {
		std::cout << "UP" << std::endl;
		upKeyPressed = true;
		//currentDirection = moveUp;
	}
	
	if (diKeys[DIK_DOWN] & 0x80) {
		std::cout << "DOWN" << std::endl;
		//currentDirection = moveDown;
		downKeyPressed = true;
	}

	if (diKeys[DIK_LEFT] & 0x80) {
		std::cout << "LEFT" << std::endl;
		//currentDirection = moveLeft;
		leftKeyPressed = true;
	}

	if (diKeys[DIK_RIGHT] & 0x80) {
		std::cout << "RIGHT" << std::endl;
		//currentDirection = moveRight;
		rightKeyPressed = true;
	}

	if (diKeys[DIK_SPACE] & 0x80) {
		std::cout << "SPACE" << std::endl;
		if (player1.playerPosition.y == WindowHeight - player1.playerSpriteHeight) {
			spaceKeyPressed = true;
		}
	}

	dInputMouseDevice->GetDeviceState(sizeof(mouseState), (LPVOID) &mouseState);
	mousePosition.x += mouseState.lX;
	mousePosition.y += mouseState.lY;
	//cout << "Mouse Position: " << mousePosition.y << " " << mousePosition.x << endl;
	
	


}

void Update(int framesToUpdate) {	

	for (int i = 0; i < framesToUpdate; i++) {

		if (leftKeyPressed) {
			player1.playerDirection -= player1.playerRotationSpeed;
		}

		if (rightKeyPressed) {
			player1.playerDirection += player1.playerRotationSpeed;
		}

		if (upKeyPressed) {
			player1.playerAcceleration.x = sin(player1.playerDirection * player1.playerEngineForce) / player1.playerMass;
			player1.playerAcceleration.y = -cos(player1.playerDirection * player1.playerEngineForce) / player1.playerMass;
		}

		if (downKeyPressed) {
			player1.playerAcceleration.x = -sin(player1.playerDirection * player1.playerEngineForce) / player1.playerMass;
			player1.playerAcceleration.y = cos(player1.playerDirection * player1.playerEngineForce) / player1.playerMass;
		}

		player1.playerVelocity += player1.playerAcceleration;
		player1.playerPosition += player1.playerVelocity;

		player1.playerFrameCounter++;
		if (player1.playerFrameCounter > player1.playerMaxFrame) {
			player1.playerFrameCounter = 0;
		}

		player1.playerAnimRect.top = player1.playerFrameCounter * player1.playerSpriteHeight;
		player1.playerAnimRect.bottom = player1.playerAnimRect.top + player1.playerSpriteHeight;
		player1.playerAnimRect.left = (player1.playerNumber - 1) * player1.playerSpriteWidth;
		player1.playerAnimRect.right = player1.playerAnimRect.left + player1.playerSpriteWidth;
		
		player2.playerFrameCounter++;
		if (player2.playerFrameCounter > player2.playerMaxFrame) {
			player2.playerFrameCounter = 0;
		}

		player2.playerAnimRect.top = player2.playerFrameCounter * player2.playerSpriteHeight;
		player2.playerAnimRect.bottom = player2.playerAnimRect.top + player2.playerSpriteHeight;
		player2.playerAnimRect.left = (player2.playerNumber - 1) * player2.playerSpriteWidth;
		player2.playerAnimRect.right = player2.playerAnimRect.left + player2.playerSpriteWidth;

		// friction
		player1.playerVelocity *= 1 - friction;

		//boundary
		if (player1.playerPosition.x < 0 || player1.playerPosition.x > player1.playerPosition.x > WindowWidth - player1.playerSpriteWidth) {
			player1.playerVelocity.x *= -1;
		}

		if (player1.playerPosition.y < 0 || player1.playerPosition.y > player1.playerPosition.y > WindowHeight - player1.playerSpriteWidth) {
			player1.playerVelocity.y *= -1;
		}

	}

	leftKeyPressed = false;
	rightKeyPressed = false;
	upKeyPressed = false;
	downKeyPressed = false;

	player1.playerAcceleration = D3DXVECTOR2(0, 0);
	player2.playerAcceleration = D3DXVECTOR2(0, 0);

}

void Render(){
	//	To Do:
	//	Update.


	//	Clear the back buffer.
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0);

	//	Begin the scene
	d3dDevice->BeginScene();

	//	Drawing.
	//	Specify alpha blend will ensure that the sprite will render the background with alpha.
	spriteBrush->Begin(D3DXSPRITE_ALPHABLEND);

	//	Sprite rendering. Study the documentation.
	// D3DCOLOR_XRGB() - COLOR FILTER
	//spriteBrush->Draw(texture, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	//spriteBrush->Draw(texture, &mouseRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	spriteBrush->Draw(mouseTexture, &mouseRect, NULL, &mousePosition, D3DCOLOR_XRGB(255, 255, 255));
	//spriteBrush->Draw(texture, &spriteRect1, NULL, &mousePosition1, D3DCOLOR_XRGB(255, 255, 255));

	//spriteBrush->Draw(playerTexture, &player1.playerAnimRect, NULL, &player1.playerPosition, D3DCOLOR_XRGB(255, 255, 255));
	
	D3DXMATRIX mat;

	// Draw Player 1
	D3DXMatrixTransformation2D(&mat, NULL, 0.5f, &player1.playerScaling, &player1.playerSpriteCenter, player1.playerDirection, &player1.playerPosition);
	//tell the sprite about the matrix
	spriteBrush->SetTransform(&mat);
	spriteBrush->Draw(playerTexture, &player1.playerAnimRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));



	//font->DrawText(spriteBrush, "Hello World!", -1, &player1ColRect, 0, D3DCOLOR_XRGB(255, 255, 255));

	//	End sprite drawing
	spriteBrush->End();
	//PlaySound()

	//	Begin to draw the lines.
	//line->Begin();
	//line->Draw(lineVertices, 2, D3DCOLOR_XRGB(255, 255, 255));
	//line->End();


	//	End and present scene

	//	End the scene
	d3dDevice->EndScene();

	//	Present the back buffer to screen
	d3dDevice->Present(NULL, NULL, NULL, NULL);
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

bool CollisionDetection() {
	/*
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
	*/
	return false;
}

//	use WinMain if you don't want the console
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) // WinMain is a function in WINAPI
{
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
//--------------------------------------------------------------------