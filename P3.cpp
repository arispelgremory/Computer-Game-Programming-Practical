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

using namespace std;

#define WindowWidth 900
#define WindowHeight 900

#pragma comment(lib, "d3d9.lib ")
#pragma comment(lib, "d3dx9.lib ")
//--------------------------------------------------------------------

//	Window handle
HWND g_hWnd = NULL;
// Window's Structure  /DESIGN PATTERN SINGLETON
WNDCLASS wndClass;

// DX globals
IDirect3DDevice9* d3dDevice;

LPCSTR texturePath = "Assets/pointer.png";

int red = 0;
int green = 0;
int blue = 0;

// Game level globals
LPDIRECT3DTEXTURE9 texture = NULL;
LPD3DXSPRITE spriteBrush = NULL;
RECT spriteRect;
D3DXVECTOR3 position;

boolean leftKeyPressed = false;
boolean rightKeyPressed = false;
boolean upKeyPressed = false;
boolean downKeyPressed = false;
//--------------------------------------------------------------------

//	Window Procedure, for event handling
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // Get messages from the OS(mouse was move,keyboard was press)
{
	switch (message)
	{
		//	The message is post when we destroy the window.
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_LEFT:
			leftKeyPressed = true;
			cout << "lEFT KEY WAS PRESSED" << endl;
			break;
		case VK_RIGHT:
			rightKeyPressed = true;
			cout << "RIGHT KEY WAS PRESSED" << endl;
			break;
		case VK_UP:
			upKeyPressed = true;
			cout << "UP KEY WAS PRESSED" << endl;
			break;
		case VK_DOWN:
			downKeyPressed = true;
			cout << "DOWN KEY WAS PRESSED" << endl;
			break;
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
	case WM_MOUSEMOVE:
		position.y = (short)HIWORD(lParam);
		position.x = (short)LOWORD(lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam); // Return messages back to the OS
	}
	return 0;
}
//--------------------------------------------------------------------

//	use int main if you want to have a console to print out message
//int main()

void createMyWindow(){
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

bool windowIsRunning(){
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

void cleanupMyLevel() {
	texture->Release();
	texture = NULL;
}

void cleanupMyWindow(){
	UnregisterClass(wndClass.lpszClassName, GetModuleHandle(NULL)); // Delete window class
}

//	use int main if you want to have a console to print out message
//int main()

void createMyDX() {
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

	
}

void intializeLevel() {
	//	Create texture. Study the documentation.
	HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, texturePath, &texture);
	//hr = D3DXCreateTextureFromFileEx(/* Your Direct3D device */, "01.bmp", D3DX_DEFAULT, D3DX_DEFAULT, 
	//									D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
	//									D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), 
	//									NULL, NULL, &texture);
	if (FAILED(hr)) {
		cout << "Create texture failed" << endl;
	}
	//	Specify the "	" rectangle.
	spriteRect.left = 0;
	spriteRect.right = 35;
	spriteRect.top = 0;
	spriteRect.bottom = 35;

	position.x = 0;
	position.y = 0;
	position.z = 0;
}

void Update() {
	if (leftKeyPressed) {
		position.x -= 5;
		leftKeyPressed = false;
	}
	if (rightKeyPressed) {
		position.x += 5;
		leftKeyPressed = false;
	}
	if (upKeyPressed) {
		position.y -= 5;
		upKeyPressed = false;
	}
	if (downKeyPressed) {
		position.y += 5;
		downKeyPressed = false;
	}
}

void render() {
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

	//	End sprite drawing
	spriteBrush->End();
	//PlaySound()

	//	End the scene
	d3dDevice->EndScene();

	//	Present the back buffer to screen
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void cleanupMyDx() {
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

	createMyWindow();
	createMyDX();
	intializeLevel();

	while (windowIsRunning())
	{	
		//GetInput()
		Update();
		render();
	}
	cleanupMyLevel();
	cleanupMyDx();
	cleanupMyWindow();

	return 0;
}
//--------------------------------------------------------------------