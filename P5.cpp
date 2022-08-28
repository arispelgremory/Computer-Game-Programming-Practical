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

using namespace std;

#define WindowWidth 800
#define WindowHeight 600
#define BUTTONDOWN (name, key) (name.rgbButtons[key] & 0x80)


#pragma comment(lib, "d3d9.lib ")
#pragma comment(lib, "d3dx9.lib ")
#pragma comment(lib, "dinput8.lib ")
#pragma comment(lib, "dxguid.lib ")
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
LPCSTR texturePath1 = "Assets/militia.png";

int red = 0;
int green = 0;
int blue = 0;

int textureWidth = 0;
int textureHeight = 0;
int colLength = 0;
int rowLength = 0;
int testX = 0;
int testY = 0;
int loopCount = 0;
int maxCount = 1;

int counter = 0;
int delayFactor = 0;

float gravity = 0;
float playerVelocityY = 0;
float jumpForce = 0;

LONG currentXpos;
LONG currentYpos;

enum movementDirection { moveDown, moveLeft, moveRight, moveUp, idle };
int currentDirection = idle;
int previousDirection = idle;


// Game level globals
LPDIRECT3DTEXTURE9 texture = NULL;
LPDIRECT3DTEXTURE9 texture1 = NULL;

LPD3DXSPRITE spriteBrush = NULL;
LPD3DXSPRITE spriteFontBrush = NULL;
LPD3DXFONT font = NULL;
LPD3DXLINE line = NULL;

RECT spriteRect;
RECT spriteRect1;
RECT textRect;

D3DXVECTOR3 position;
D3DXVECTOR3 position1;

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
	texture->Release();
	texture = NULL;
	texture1->Release();
	texture1 = NULL;
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
	HRESULT hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);

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
	HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, texturePath, &texture);

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
	dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);

	//	Set the cooperative level.
	//	To Do:
	//	Try with different combination.
	dInputKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);

	if (FAILED(hr)) {
		cout << "Create direct input mouse failed" << endl;
	}

	hr = dInputMouseDevice->SetDataFormat(&c_dfDIMouse);

	// Check the return code for the SetDataFormat function
	
	if (FAILED(hr)) {
		cout << "Create direct input mouse failed" << endl;
	}
}

void InitializeLevel() {
	//	Create texture. Study the documentation.
	HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, texturePath, &texture);
	HRESULT hr1 = D3DXCreateTextureFromFile(d3dDevice, texturePath1, &texture1);
	//hr = D3DXCreateTextureFromFileEx(/* Your Direct3D device */, "01.bmp", D3DX_DEFAULT, D3DX_DEFAULT, 
	//									D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
	//									D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), 
	//									NULL, NULL, &texture);
	if (FAILED(hr)) {
		cout << "Create texture failed" << endl;
	}

	if (FAILED(hr1)) {
		cout << "Create Texture 1 failed" << endl;
	}


	textureWidth = 128;
	textureHeight = 192;
	colLength = 4;
	rowLength = 4;
	testX = textureWidth / colLength;
	testY = textureHeight / rowLength;

	delayFactor = 20;


	//	Specify the "	" rectangle.
	spriteRect.left = 0;
	spriteRect.right = 35;
	spriteRect.top = 0;
	spriteRect.bottom = 35;

	spriteRect1.left = 0;
	spriteRect1.right = testX;
	spriteRect1.top = 0;
	spriteRect1.bottom = testY;

	currentXpos = WindowWidth / 2;
	currentYpos = WindowHeight / 2;
	position.x = currentXpos;
	position.y = currentYpos;
	position.z = 0;

	position1.x = 0;
	position1.y = 0;
	position1.z = 0;

	maxCount = 1;

	textRect.left = 100;
	textRect.top = 100;
	textRect.right = 600;
	textRect.bottom = 600;

	gravity = 0.98;
	jumpForce = -20;
	

	startpoint = D3DXVECTOR2(WindowWidth / 2, WindowHeight / 2);
	endpoint;

	lineVertices[0] = startpoint;
	lineVertices[1] = endpoint;

}

void GetInput() {
	//	Acquire the device.
	dInputKeyboardDevice->Acquire();
	dInputMouseDevice->Acquire();


	previousDirection = currentDirection;

	//	Get immediate Keyboard Data.
	dInputKeyboardDevice->GetDeviceState(256, diKeys);
	if (diKeys[DIK_UP] & 0x80) {
		std::cout << "UP" << std::endl;
		upKeyPressed = true;
		currentDirection = moveUp;
	}

	if (diKeys[DIK_DOWN] & 0x80) {
		std::cout << "DOWN" << std::endl;
		currentDirection = moveDown;
		downKeyPressed = true;
	}

	if (diKeys[DIK_LEFT] & 0x80) {
		std::cout << "LEFT" << std::endl;
		currentDirection = moveLeft;
		leftKeyPressed = true;
	}

	if (diKeys[DIK_RIGHT] & 0x80) {
		std::cout << "RIGHT" << std::endl;
		currentDirection = moveRight;
		rightKeyPressed = true;
	}

	if (diKeys[DIK_SPACE] & 0x80) {
		std::cout << "SPACE" << std::endl;
		if (position1.y == WindowHeight - testY) {
			spaceKeyPressed = true;
		}
	}

	dInputMouseDevice->GetDeviceState(sizeof(mouseState), (LPVOID) &mouseState);
	position.x += mouseState.lX;
	position.y += mouseState.lY;
	// cout << "Mouse Position: " << position.y << " " << position.x << endl;;


}

void Update() {	

	if (loopCount > maxCount || loopCount >= rowLength * colLength) {
		loopCount = (currentDirection * rowLength);
		if (currentDirection == idle) {
			loopCount = 0;
		}
			
	}

	if (spaceKeyPressed) {
		playerVelocityY += jumpForce;
		position1.y += playerVelocityY;
		
		// cout << "Sprite Position left & top: " << spriteRect1.left << " " << spriteRect1.top << endl;
		spaceKeyPressed = false;
	}

	counter++;
	if (counter % delayFactor == 0) {
		 cout << "LoopCount: " << loopCount << " MaxCount: " << maxCount << endl;
		// cout << "Current Direction: " << currentDirection << " Previous Direction: " << previousDirection << endl;


		if (loopCount > maxCount) {
			loopCount = (currentDirection * rowLength);
		}
		else if (loopCount < maxCount - colLength) {
			loopCount = maxCount - colLength;
			// cout << "Reset LoopCount: " << loopCount << " MaxCount: " << maxCount << endl;
		}


		loopCount++;
		maxCount = (currentDirection + 1) * colLength - 1;
		if ((currentDirection == previousDirection) && (!leftKeyPressed || !rightKeyPressed || !upKeyPressed || !downKeyPressed)) {
			currentDirection = idle;
			cout << "current idle" << endl;
		}
	}


	if (leftKeyPressed && position1.x > 0) {
		position1.x -= 5;
		//previousDirection = moveLeft;
	}
	if (rightKeyPressed && position1.x <= WindowWidth - testX) {
		position1.x += 5;
		//previousDirection = moveRight;
	}
	if (upKeyPressed && position1.y > 0) {
		position1.y -= 5;
		//previousDirection = moveUp;
	}
	if (downKeyPressed && position1.y <= WindowHeight - testY) {
		position1.y += 5;
		//previousDirection = moveDown;
	}


	leftKeyPressed = false;
	rightKeyPressed = false;
	upKeyPressed = false;
	downKeyPressed = false;

	
	spriteRect1.left = (loopCount % colLength) * testX;
	spriteRect1.right = spriteRect1.left + testX;
	spriteRect1.top = (loopCount / colLength) * testY;
	spriteRect1.bottom = spriteRect1.top + testY;

	//cout << "Sprite Position left & top: " << spriteRect1.left << " " << spriteRect1.top << endl;
	//cout << "Sprite width & height: " << testX << " " << testY << endl;


	if (currentDirection == idle) {
		maxCount = 0;
	}

	playerVelocityY += gravity;
	position1.y += playerVelocityY;

	if (position1.y > WindowHeight - testY) {
		position1.y = WindowHeight - testY;
		playerVelocityY = 0;
	}

	currentTime += 0.02;

	endpoint.x = sin(currentTime) * needleLength + startpoint.x;
	endpoint.y = -cos(currentTime) * needleLength + startpoint.y;

	lineVertices[1] = endpoint;


	
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
	//spriteBrush->Draw(texture, &spriteRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	spriteBrush->Draw(texture, &spriteRect, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	spriteBrush->Draw(texture1, &spriteRect1, NULL, &position1, D3DCOLOR_XRGB(255, 255, 255));

	font->DrawText(spriteBrush, "Hello World!", -1, &textRect, 0, D3DCOLOR_XRGB(255, 255, 255));

	//	End sprite drawing
	spriteBrush->End();
	//PlaySound()

	//	Begin to draw the lines.
	line->Begin();
	line->Draw(lineVertices, 2, D3DCOLOR_XRGB(255, 255, 255));
	line->End();


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
	while (WindowIsRunning()) {	
		GetInput();
		Update();
		Render();
	}
	CleanupMyLevel();
	CleanUpMyDirectInput();
	CleanupMyDx();
	CleanupMyWindow();

	return 0;
}
//--------------------------------------------------------------------