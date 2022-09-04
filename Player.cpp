#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
#include "Player.h"

void Player::Init(int playerNumber, int PlayerXPosition, int PlayerYPosition) {

    //shared
    this->playerNumber = playerNumber;
    this->playerTextureWidth = 64;
    this->playerTextureHeight = 64;
    this->playerTextureRow = 2;
    this->playerTextureCol = 2;
    this->playerSpriteWidth = playerTextureWidth / playerTextureCol;
    this->playerSpriteHeight = playerTextureHeight / playerTextureRow;
    this->playerSpriteFPS = 10;
    this->playerMaxFrame = 1;


    this->playerAnimRect.left = (this->playerNumber - 1) * playerSpriteWidth;
    this->playerAnimRect.top = 0;
    this->playerAnimRect.right = playerAnimRect.left + playerSpriteWidth;
    this->playerAnimRect.bottom = playerAnimRect.top + playerSpriteHeight;

    this->playerColRect.left = 0;
    this->playerColRect.top = 0;
    this->playerColRect.right = playerAnimRect.left + playerSpriteWidth;
    this->playerColRect.bottom = playerAnimRect.top + playerSpriteHeight;

    this->playerPosition = D3DXVECTOR2(PlayerXPosition, PlayerYPosition);
    this->playerVelocity = D3DXVECTOR2(0, 0);
    this->playerAcceleration = D3DXVECTOR2(0, 0);

    this->playerMass = 1;
    this->playerEngineForce = 0.005;
    this->playerFrameCounter = 0;
    this->playerRotationSpeed = 0.005;

    this->playerScaling = D3DXVECTOR2(1, 1);
    this->playerSpriteCenter = D3DXVECTOR2(playerSpriteWidth / 2, playerSpriteHeight / 2);
}

void Player::setPlayerAccelerationX(double X) {
    this->playerAcceleration.x = X;
    cout << "PLAYER.CPP PLAYER ACCELERATION X: " << this->playerAcceleration.x << " X: " << X << endl;
}

void Player::setPlayerAccelerationY(double Y) {
    this->playerAcceleration.y = Y;
}