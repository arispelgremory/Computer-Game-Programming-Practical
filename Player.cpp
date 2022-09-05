#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
#include "Player.h"
#define WindowWidth 800
#define WindowHeight 600

void Player::Init(int playerNumber, int playerXPosition, int playerYPosition) {

    //shared
    number = playerNumber;
    textureWidth = 64;
    textureHeight = 64;
    textureRow = 2;
    textureCol = 2;
    spriteWidth = textureWidth / textureCol;
    spriteHeight = textureHeight / textureRow;
    spriteFPS = 10;
    maxFrame = 1;


    animRect.left = (number - 1) * spriteWidth;
    animRect.top = 0;
    animRect.right = animRect.left + spriteWidth;
    animRect.bottom = animRect.top + spriteHeight;

    colRect.left = 0;
    colRect.top = 0;
    colRect.right = animRect.left + spriteWidth;
    colRect.bottom = animRect.top + spriteHeight;

    position = D3DXVECTOR2(playerXPosition, playerYPosition);
    acceleration = D3DXVECTOR2(0, 0);
    frameCounter = 0;
    mass = 1;

    // Player Object
    velocity = D3DXVECTOR2(0, 0);
    //this->engineForce = 0.005;
    playerEngineForce = 0.005;
    this->SetPlayerEngineForce(0.005);
    //this->rotationSpeed = 0.005;    
    this->SetPlayerRotationSpeed(0.005);

    scaling = D3DXVECTOR2(1, 1);
    spriteCenter = D3DXVECTOR2(spriteWidth / 2, spriteHeight / 2);
}

void Player::setPlayerAccelerationX(double X) {
    acceleration.x = X;
}

void Player::setPlayerAccelerationY(double Y) {
    acceleration.y = Y;
}

void StaticObject::Init(int objectNumber) {
    //shared
    number = objectNumber;
    textureWidth = 64;
    textureHeight = 64;
    textureRow = 2;
    textureCol = 2;
    spriteWidth = textureWidth / textureCol;
    spriteHeight = textureHeight / textureRow;
    spriteFPS = 10;
    maxFrame = 1;


    animRect.left = (number - 1) * spriteWidth;
    animRect.top = 0;
    animRect.right = animRect.left + spriteWidth;
    animRect.bottom = animRect.top + spriteHeight;

    colRect.left = 0;
    colRect.top = 0;
    colRect.right = animRect.left + spriteWidth;
    colRect.bottom = animRect.top + spriteHeight;

    position = D3DXVECTOR2(rand() % (WindowWidth - spriteWidth), rand() % (WindowHeight - spriteHeight));
    mass = 1;
    frameCounter = 0;
    scaling = D3DXVECTOR2(1, 1);
    spriteCenter = D3DXVECTOR2(spriteWidth / 2, spriteHeight / 2);
}