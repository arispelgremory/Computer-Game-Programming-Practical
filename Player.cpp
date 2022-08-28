#include "Player.h"

void Player::Init() {

    //shared
    playerTextureWidth = 64;
    playerTextureHeight = 64;
    playerTextureRow = 2;
    playerTextureCol = 2;
    playerSpriteWidth = playerTextureWidth / playerTextureCol;
    playerSpriteHeight = playerTextureHeight / playerTextureRow;
    playerSpriteFPS = 10;
    playerMaxFrame = 1;


    playerAnimRect.left = (playerNumber - 1) * playerSpriteWidth;
    playerAnimRect.top = 0;
    playerAnimRect.right = playerAnimRect.left + playerSpriteWidth;
    playerAnimRect.bottom = playerAnimRect.top + playerSpriteHeight;

    playerColRect.left = 0;
    playerColRect.top = 0;
    playerColRect.right = playerAnimRect.left + playerSpriteWidth;
    playerColRect.bottom = playerAnimRect.top + playerSpriteHeight;

    playerPosition = D3DXVECTOR2(100, 300);
    playerVelocity = D3DXVECTOR2(0, 0);
    playerAcceleration = D3DXVECTOR2(0, 0);

    playerMass = 1;
    playerFrameCounter = 0;
    playerRotationSpeed = 0.1;

    playerScaling = D3DXVECTOR2(1, 1);
    playerSpriteCenter = D3DXVECTOR2(playerSpriteWidth / 2, playerSpriteHeight / 2);
}