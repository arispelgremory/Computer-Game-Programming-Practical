//    Ask the compiler to include minimal header files for our program.
// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
// #include <iostream>
// #include <d3d9.h>    //    include the Direct3D 9 library
// #include <d3dx9.h>    //    include the D3DX9 library
// #include <dinput.h>    //    include the Direct Input library.
// #include "FrameTimer.h"
//--------------------------------------------------------------------
// using namespace std;

// #pragma comment(lib, "d3d9.lib")
// #pragma comment(lib, "d3dx9.lib")
// #pragma comment(lib, "dinput8.lib")
// #pragma comment(lib, "dxguid.lib")
// #define WindowWidth 800
// #define WindowHeight 600

// HWND g_hWnd = NULL;   	 //    Window handle
// WNDCLASS wndClass;   	 //    Window's structure

//DX globals
// IDirect3DDevice9* d3dDevice;
// LPD3DXSPRITE spriteBrush = NULL;   		 //sprite is the thing draw in the screen (spriteBrush)
// LPD3DXFONT font = NULL;
// LPD3DXLINE line = NULL;
LPDIRECTINPUT8 dInput;   					 //    Direct Input object.
LPDIRECTINPUTDEVICE8  dInputKeyboardDevice;    //    Direct Input keyboard device.
LPDIRECTINPUTDEVICE8  dInputMouseDevice;    //    Direct Input keyboard device.
BYTE  diKeys[256];   						 //    Key input buffer
DIMOUSESTATE mouseStates;   				 // mouse input buffer

//Game level global
LPDIRECT3DTEXTURE9 playertexture = NULL;

//player 1 & player 2 shared globals
int playerTextureWidth = 0;
int playerTextureHeight = 0;
int playerTextureRow = 0;
int playerTextureCol = 0;
int playerSpriteWidth = 0;
int playerSpriteHeight = 0;
int playerSpriteFPS = 0;
int playerMaxFrame = 0;

//player 1
RECT player1AnimRect;
RECT player1ColRect;
D3DXVECTOR2 player1Position;
D3DXVECTOR2 player1Velocity;
D3DXVECTOR2 player1Acceleration;
float player1EngineForce = 0;
float player1Direction = 0;
float player1Mass = 0;
int player1FrameCounter = 0;
int player1Number = 0;
D3DXVECTOR2 player1Scaling;
D3DXVECTOR2 player1SpriteCentre;
float player1RotationSpeed = 0;

//player 2
RECT player2AnimRect;
RECT player2ColRect;
D3DXVECTOR2 player2Position;
D3DXVECTOR2 player2Velocity;
D3DXVECTOR2 player2Acceleration;
float player2EngineForce = 0;
float player2Direction = 0;
float player2Mass = 0;
int player2FrameCounter = 0;
int player2Number = 0;
D3DXVECTOR2 player2Scaling;
D3DXVECTOR2 player2SpriteCentre;
float player2RotationSpeed = 0;

//mouse texture
LPDIRECT3DTEXTURE9 mousetexture = NULL;
RECT mouseRect;
RECT mouseColRect;
D3DXVECTOR3 mouseposition;

RECT textRect;

HRESULT hr;

//enum movementDirection {moveDown, moveLeft, moveRight, moveUp};
//
//int counter = 0;
//int delayFactor = 20;

bool leftKeyPressed = false;
bool rightKeyPressed = false;
bool upKeyPressed = false;
bool downKeyPressed = false;
bool spaceKeyPressed = false;

FrameTimer* timer = new FrameTimer();

float gravity = 0;
float friction = 0;

//float currentTime;
//int needleLength = 200;
//
//D3DXVECTOR2 startpoint;
//D3DXVECTOR2 endpoint;
//D3DXVECTOR2 lineVertices[] = { startpoint, endpoint};

//--------------------------------------------------------------------

//    Window Procedure, for event handling
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        //    The message is post when we destroy the window.

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
            //    case VK_ESCAPE:
            //   	 PostQuitMessage(0);
            //   	 break;
             //case VK_SPACE:
             //    //<2 =double jump
             //    //<3 =triple jump
             //    if (position.y == WindowHeight - spriteHeight || jumpcount <2)
             //    {
             //   	 jumpcount += 1;
             //   	 spaceKeyPressed = true;
             //   	 cout << "Jump Pressed!" << endl;
             //   	 cout << "jump count:" << jumpcount << endl;
             //    }else{
             //   	 spaceKeyPressed = false;
             //    }
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
}

void CreateMyWindow()
{
    //    Set all members in wndClass to 0.
    ZeroMemory(&wndClass, sizeof(wndClass));

    //    Filling wndClass. You are to refer to MSDN for each of the members details.
    //    These are the fundamental structure members to be specify, in order to create your window.
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpfnWndProc = WindowProcedure;
    wndClass.lpszClassName = "My Window";
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    //    Register the window.
    RegisterClass(&wndClass);

    //    You are to refer to MSDN for each of the parameters details.
    g_hWnd = CreateWindowEx(0, wndClass.lpszClassName, "My Window's Name", WS_OVERLAPPEDWINDOW, 0, 100, WindowWidth, WindowHeight, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(g_hWnd, 1);
}

void CreateMyDX() {
    //    Define Direct3D 9.
    IDirect3D9* direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);

    //    Define how the screen presents.
    D3DPRESENT_PARAMETERS d3dPP;
    ZeroMemory(&d3dPP, sizeof(d3dPP));

    //    Refer to Direct3D 9 documentation for the meaning of the members.
    d3dPP.Windowed = true;
    d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dPP.BackBufferCount = 1;
    d3dPP.BackBufferWidth = WindowWidth;
    d3dPP.BackBufferHeight = WindowHeight;
    d3dPP.hDeviceWindow = g_hWnd;

    //    Create a Direct3D 9 device.
    hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL/*D3DDEVTYPE_REF*/, g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPP, &d3dDevice);
    //    To Do: Cout out the message to indicate the failure.
    if (FAILED(hr))
        cout << "Create DirectX Failed" << endl;

    //    Create sprite. Study the documentation.
    hr = D3DXCreateSprite(d3dDevice, &spriteBrush);

    if (FAILED(hr))
    {
        cout << "sprite fail" << endl;
    }

    // Create text.
    hr = D3DXCreateFont(d3dDevice, 25, 0, 0, 1, false,
        DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);

    // Create Line
    hr = D3DXCreateLine(d3dDevice, &line);
    //return 0;
}

void CreateMyDInput() {
    //    Create the Direct Input object.
    hr = DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (void**)&dInput, NULL);

    if (FAILED(hr)) {
        cout << "input object fail" << endl;
    }

    //    Create the keyboard device.
    hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);

    if (FAILED(hr)) {
        cout << "keyboard fail" << endl;
    }

    //    Set the input data format.
    hr = dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);

    if (FAILED(hr)) {
        cout << "keyboard input data fail" << endl;
    }

    //    Set the cooperative level.
    //    To Do:
    //    Try with different combination.
    hr = dInputKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    if (FAILED(hr)) {
        cout << "cooperative level fail" << endl;
    }

    //mouse//
    //    Create the mouse device.
    hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);

    if (FAILED(hr)) {
        cout << "mouse fail" << endl;
    }

    //    Set the input data format.
    hr = dInputMouseDevice->SetDataFormat(&c_dfDIMouse);

    if (FAILED(hr)) {
        cout << "mouse input data fail" << endl;
    }

    //    Set the cooperative level.
    //    To Do:
    //    Try with different combination.
    hr = dInputKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    mouseposition.x = WindowWidth / 2;
    mouseposition.y = WindowHeight / 2;

    if (FAILED(hr)) {
        cout << "cooperative level fail" << endl;
    }
}

void InitializeLevel() {
    //    Create texture. Study the documentation.
    hr = D3DXCreateTextureFromFile(d3dDevice, "Assets/pointer.png", &mousetexture);
    hr = D3DXCreateTextureFromFileEx(d3dDevice, "Assets/space.png", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
        D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), NULL, NULL, &playertexture);

    if (FAILED(hr)) {
        cout << "texture fail" << endl;
    }

    //mouse
    mouseRect.left = 0;
    mouseRect.top = 0;
    mouseRect.right = 32;
    mouseRect.bottom = 32;
    //mouse collision
    mouseColRect.left = 0;
    mouseColRect.top = 0;
    mouseColRect.right = 32;
    mouseColRect.bottom = 32;
    //text
    textRect.left = 100;
    textRect.top = 100;
    textRect.right = 450;
    textRect.bottom = 125;

    gravity = 2;
    friction = 0.01;

    //shared
    playerTextureWidth = 64;
    playerTextureHeight = 64;
    playerTextureRow = 2;
    playerTextureCol = 2;
    playerSpriteWidth = playerTextureWidth / playerTextureCol;
    playerSpriteHeight = playerTextureHeight / playerTextureRow;
    playerSpriteFPS = 10;
    playerMaxFrame = 1;

    //player1
    player1Number = 1;

    player1AnimRect.left = (player1Number - 1) * playerSpriteWidth;
    player1AnimRect.top = 0;
    player1AnimRect.right = player1AnimRect.left + playerSpriteWidth;
    player1AnimRect.bottom = player1AnimRect.top + playerSpriteHeight;

    player1ColRect.left = 0;
    player1ColRect.top = 0;
    player1ColRect.right = player1AnimRect.left + playerSpriteWidth;
    player1ColRect.bottom = player1AnimRect.top + playerSpriteHeight;

    player1Position = D3DXVECTOR2(100, 300);
    player1Velocity = D3DXVECTOR2(0, 0);
    player1Acceleration = D3DXVECTOR2(0, 0);
    player1EngineForce = 1;
    player1Direction = 0;
    player1Mass = 1;
    player1FrameCounter = 0;
    player1RotationSpeed = 0.1;

    player1Scaling = D3DXVECTOR2(1, 1);
    player1SpriteCentre = D3DXVECTOR2(playerSpriteWidth / 2, playerSpriteHeight / 2);

    //player2
    player1Number = 2;

    player2AnimRect.left = (player2Number - 1) * playerSpriteWidth;
    player2AnimRect.top = 0;
    player2AnimRect.right = player2AnimRect.left + playerSpriteWidth;
    player2AnimRect.bottom = player2AnimRect.top + playerSpriteHeight;

    player2ColRect.left = 0;
    player2ColRect.top = 0;
    player2ColRect.right = player1AnimRect.left + playerSpriteWidth;
    player2ColRect.bottom = player1AnimRect.top + playerSpriteHeight;

    player2Position = D3DXVECTOR2(700, 300);
    player2Velocity = D3DXVECTOR2(0, 0);
    player2Acceleration = D3DXVECTOR2(0, 0);
    player2EngineForce = 1;
    player2Direction = 0;
    player2Mass = 1;
    player2FrameCounter = 0;
    player2RotationSpeed = 0.1;

    player2Scaling = D3DXVECTOR2(1, 1);
    player2SpriteCentre = D3DXVECTOR2(playerSpriteWidth / 2, playerSpriteHeight / 2);
}

void GetInput() {
    //    Get immediate Keyboard Data.
    dInputKeyboardDevice->GetDeviceState(256, diKeys);
    //    Acquire the device.
    dInputKeyboardDevice->Acquire();
    dInputMouseDevice->Acquire();

    if (diKeys[DIK_UP] & 0x80) {
        if (player1Position.y == 0) {
            upKeyPressed = false;
        }
        else {
            upKeyPressed = true;
            cout << "Up Pressed!" << endl;
        }
    }

    if (diKeys[DIK_DOWN] & 0x80) {
        if (player1Position.y > WindowHeight - playerSpriteWidth) {
            downKeyPressed = false;
        }
        else {
            downKeyPressed = true;
            cout << "Down Pressed!" << endl;
        }
    }

    if (diKeys[DIK_LEFT] & 0x80) {
        if (player1Position.x == 0) {
            leftKeyPressed = false;
        }
        else {
            leftKeyPressed = true;
            cout << "Left Pressed!" << endl;
        }
    }

    if (diKeys[DIK_RIGHT] & 0x80) {
        if (player1Position.x > WindowWidth - playerSpriteWidth) {
            rightKeyPressed = false;
        }
        else {
            rightKeyPressed = true;
            cout << "Right Pressed!" << endl;
        }
    }

    //if (diKeys[DIK_SPACE] & 0x80){
    //    if (player1Position.y == WindowHeight - playerSpriteHeight /*|| jumpcount <2*/)
    //    {
    //   	 //jumpcount += 1;
    //   	 spaceKeyPressed = true;
    //   	 cout << "Jump Pressed!" << endl;
    //   	 //cout << "jump count:" << jumpcount << endl;
    //    }
    //    else {
    //   	 spaceKeyPressed = false;
    //    }
    //}

    dInputMouseDevice->GetDeviceState(sizeof(mouseStates), &mouseStates);
}

void Update(int framesToUpdate) {
    for (int i = 0; i < framesToUpdate; i++) {
        /*counter++;*/
        if (leftKeyPressed) {
            player1Direction -= player1RotationSpeed;
        }

        if (rightKeyPressed) {
            player1Direction += player1RotationSpeed;
        }

        if (upKeyPressed) {
            /*if (counter % timer->getFPS() / player1SpriteFPS) {
                player1FrameCounter++
            }*/
            player1Acceleration.x = sin(player1Direction) * player1EngineForce / player1Mass;
            player1Acceleration.y = -cos(player1Direction) * player1EngineForce / player1Mass;
        }

        if (downKeyPressed) {
            player1Acceleration.x = -sin(player1Direction) * player1EngineForce / player1Mass;
            player1Acceleration.y = cos(player1Direction) * player1EngineForce / player1Mass;
        }

        player1Velocity += player1Acceleration;
        //friction
        player1Velocity *= 1 - friction;
        player1Position += player1Velocity;

        player1FrameCounter++;
        if (player1FrameCounter > playerMaxFrame) {
            player1FrameCounter = 0;
        }

        player1AnimRect.left = (player1Number - 1) * playerSpriteWidth;
        player1AnimRect.top = player1FrameCounter * playerSpriteHeight;
        player1AnimRect.right = player1AnimRect.left + playerSpriteWidth;
        player1AnimRect.bottom = player1AnimRect.top + playerSpriteHeight;

        player2FrameCounter++;
        if (player2FrameCounter > playerMaxFrame) {
            player2FrameCounter = 0;
        }

        player2AnimRect.left = (player2Number - 1) * playerSpriteWidth;
        player2AnimRect.top = player2FrameCounter * playerSpriteHeight;
        player2AnimRect.right = player2AnimRect.left + playerSpriteWidth;
        player2AnimRect.bottom = player2AnimRect.top + playerSpriteHeight;



        //boundry
        if (player1Position.x <0 || player1Position.x > WindowWidth - playerSpriteWidth) {
            player1Velocity.x *= -1;
        }
        if (player1Position.y <0 || player1Position.y > WindowHeight - playerSpriteHeight) {
            player1Velocity.y *= -1;
        }
        if (player2Position.x <0 || player2Position.x > WindowWidth - playerSpriteWidth) {
            player2Velocity.x *= -1;
        }
        if (player2Position.y <0 || player2Position.y > WindowHeight - playerSpriteHeight) {
            player2Velocity.y *= -1;
        }

    }
    leftKeyPressed = false;
    rightKeyPressed = false;
    upKeyPressed = false;
    player1Acceleration = D3DXVECTOR2(0, 0);
    player2Acceleration = D3DXVECTOR2(0, 0);
    downKeyPressed = false;
}

void Render() {
    //    Clear the back buffer.
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    //    Begin the scene
    d3dDevice->BeginScene();

    //    To Do:
    //    Drawing.
    //    Specify alpha blend will ensure that the sprite will render the background with alpha.
    spriteBrush->Begin(D3DXSPRITE_ALPHABLEND); //enable alpha value

    D3DXMATRIX mat;

    //draw player 1
    D3DXMatrixTransformation2D(&mat, NULL, 0.5f, &player1Scaling, &player1SpriteCentre, player1Direction, &player1Position);
    //tell the sprite about the matrix
    spriteBrush->SetTransform(&mat);
    spriteBrush->Draw(playertexture, &player1AnimRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

    //draw player 2
    D3DXMatrixTransformation2D(&mat, NULL, 0.5f, &player2Scaling, &player2SpriteCentre, player2Direction, &player2Position);
    //tell the sprite about the matrix
    spriteBrush->SetTransform(&mat);
    spriteBrush->Draw(playertexture, &player2AnimRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

    //spriteBrush->Draw(playertexture, &spriteRect, NULL, &player1Position, D3DCOLOR_XRGB(255, 255, 255));
    //spriteBrush->Draw(mousetexture, &mouseRect, NULL, &mouseposition, D3DCOLOR_XRGB(255, 255, 255));

    //    End sprite drawing
    spriteBrush->End();

    //    End the scene
    d3dDevice->EndScene();

    //    Present the back buffer to screen
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}

bool WindowIsRunning()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        //    Receive a quit message
        if (msg.message == WM_QUIT)
            return false;
        //    Translate the message
        TranslateMessage(&msg);
        //    Send message to your window procedure
        DispatchMessage(&msg);
    }
    return true;
}

void CleanupMyLevel() {
    spriteBrush->Release();
    spriteBrush = NULL;

    playertexture->Release();
    playertexture = NULL;

    mousetexture->Release();
    mousetexture = NULL;
}

void CleanupMyWindow()
{
    //    Free up the memory.
    UnregisterClass(wndClass.lpszClassName, GetModuleHandle(NULL));
}

void CleanupMyDX() {
    //    Release the device when exiting.
    d3dDevice->Release();
    //    Reset pointer to NULL, a good practice.
    d3dDevice = NULL;

    font->Release();
    font = NULL;

    line->Release();
    line = NULL;

}

void CleanupMyDInput() {
    //    Release keyboard device.
    dInputKeyboardDevice->Acquire();
    dInputKeyboardDevice->Release();
    dInputKeyboardDevice = NULL;

    //    Release DirectInput.
    dInput->Release();
    dInput = NULL;
}

//    use WinMain if you don't want the console
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    CreateMyWindow();
    CreateMyDX();
    CreateMyDInput();
    InitializeLevel();



    timer->Init(20);

    while (WindowIsRunning())
    {
        GetInput();
        Update(timer->framesToUpdate());
        Render();
        //PlaySound;
    }

    CleanupMyLevel();
    CleanupMyDInput();
    CleanupMyDX();
    CleanupMyWindow();


    return 0;
}



