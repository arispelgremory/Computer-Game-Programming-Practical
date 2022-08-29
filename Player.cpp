#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
#include "Player.h"

void Player::Init(int playerNumber, int PlayerXPosition, int PlayerYPosition) {

    //shared
    this->playerNumber = playerNumber;
    playerTextureWidth = 64;
    playerTextureHeight = 64;
    playerTextureRow = 2;
    playerTextureCol = 2;
    playerSpriteWidth = playerTextureWidth / playerTextureCol;
    playerSpriteHeight = playerTextureHeight / playerTextureRow;
    playerSpriteFPS = 10;
    playerMaxFrame = 1;


    playerAnimRect.left = (this->playerNumber - 1) * playerSpriteWidth;
    playerAnimRect.top = 0;
    playerAnimRect.right = playerAnimRect.left + playerSpriteWidth;
    playerAnimRect.bottom = playerAnimRect.top + playerSpriteHeight;

    playerColRect.left = 0;
    playerColRect.top = 0;
    playerColRect.right = playerAnimRect.left + playerSpriteWidth;
    playerColRect.bottom = playerAnimRect.top + playerSpriteHeight;

    playerPosition = D3DXVECTOR2(PlayerXPosition, PlayerYPosition);
    playerVelocity = D3DXVECTOR2(0, 0);
    playerAcceleration = D3DXVECTOR2(0, 0);

    playerMass = 1;
    playerFrameCounter = 0;
    playerRotationSpeed = 0.1;

    playerScaling = D3DXVECTOR2(1, 1);
    playerSpriteCenter = D3DXVECTOR2(playerSpriteWidth / 2, playerSpriteHeight / 2);
}

void Player::setPlayerAccelerationX(double X) {
    playerAcceleration.x = X;
    cout << "PLAYER.CPP PLAYER ACCELERATION X: " << playerAcceleration.x << " X: " << X << endl;
}

void Player::setPlayerAccelerationY(double Y) {
    playerAcceleration.y = Y;
}