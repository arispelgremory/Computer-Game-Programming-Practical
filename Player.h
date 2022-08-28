#pragma once
#include <Windows.h>
//	include the D3DX9 library
#include <d3dx9.h>

class Player {
	public:
		LPCSTR texturePath;

		// shared?
		int playerTextureWidth;
		int playerTextureHeight;
		int playerTextureRow;
		int playerTextureCol;
		int playerSpriteWidth;
		int playerSpriteHeight;
		int playerSpriteFPS;
		int playerMaxFrame;

		// individuals
		RECT playerAnimRect;
		RECT playerColRect;
		D3DXVECTOR2 playerPosition;
		D3DXVECTOR2 playerVelocity;
		D3DXVECTOR2 playerAcceleration;
		float playerEngineForce;
		float playerDirection;
		float playerMass;
		int playerFrameCounter;
		int playerNumber;
		D3DXVECTOR2 playerScaling;
		D3DXVECTOR2 playerSpriteCenter;
		float playerRotationSpeed;

		// functions
		void Init();

};