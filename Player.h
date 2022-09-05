#pragma once
#include <Windows.h>
//	include the D3DX9 library
#include <d3dx9.h>

class GameObject {
	protected:
		// shared?
		int textureWidth;
		int textureHeight;
		int textureRow;
		int textureCol;
		int spriteWidth;
		int spriteHeight;
		int spriteFPS;
		int maxFrame;

		// individuals
		RECT animRect;
		RECT colRect;
		D3DXVECTOR2 position;
		D3DXVECTOR2 velocity;
		D3DXVECTOR2 acceleration;
		float mass;
		int frameCounter;
		int number;
		D3DXVECTOR2 scaling;
		D3DXVECTOR2 spriteCenter;
		float direction;

		int hp;

public:
	int GetTextureWidth() const {
		return textureWidth;
	}

	void SetTextureWidth(int textureWidth) {
		this->textureWidth = textureWidth;
	}

	int GetTextureHeight() const {
		return textureHeight;
	}

	void SetTextureHeight(int textureHeight) {
		this->textureHeight = textureHeight;
	}

	int GetTextureRow() const {
		return textureRow;
	}

	void SetTextureRow(int textureRow) {
		this->textureRow = textureRow;
	}

	int GetTextureCol() const {
		return textureCol;
	}

	void SetTextureCol(int textureCol) {
		this->textureCol = textureCol;
	}

	int GetSpriteWidth() const {
		return spriteWidth;
	}

	void SetSpriteWidth(int spriteWidth) {
		this->spriteWidth = spriteWidth;
	}

	int GetSpriteHeight() const {
		return spriteHeight;
	}

	void SetSpriteHeight(int spriteHeight) {
		this->spriteHeight = spriteHeight;
	}

	int GetSpriteFPS() const {
		return spriteFPS;
	}

	void SetSpriteFPS(int spriteFPS) {
		this->spriteFPS = spriteFPS;
	}

	int GetMaxFrame() const {
		return maxFrame;
	}

	void SetMaxFrame(int maxFrame) {
		this->maxFrame = maxFrame;
	}

	RECT GetAnimRect() const {
		return animRect;
	}


	float GetLeftAnimRect() const {
		return animRect.left;
	}

	float GetRightAnimRect() const {
		return animRect.right;
	}

	float GetTopAnimRect() const {
		return animRect.top;
	}

	float GetBottomAnimRect() const {
		return animRect.bottom;
	}

	void SetLeftAnimRect(float value) {
		this->animRect.left = value;
	}

	void SetRightAnimRect(float value) {
		this->animRect.right = value;
	}

	void SetTopAnimRect(float value) {
		this->animRect.top = value;
	}

	void SetBottomAnimRect(float value) {
		this->animRect.bottom = value;
	}

	void SetAnimRect(RECT animRect) {
		this->animRect = animRect;
	}

	RECT GetColRect() const {
		return colRect;
	}

	void SetColRect(RECT colRect) {
		this->colRect = colRect;
	}

	D3DXVECTOR2 GetPosition() const {
		return position;
	}

	float GetXPosition() const {
		return position.x;
	}

	float GetYPosition() const {
		return position.y;
	}

	void SetPosition(D3DXVECTOR2 position) {
		this->position = position;
	}

	void SetXPosition(int value) {
		this->position.x = value;
	}

	void SetYPosition(int value) {
		this->position.y = value;
	}

	D3DXVECTOR2 GetVelocity() const {
		return velocity;
	}

	float GetXVelocity() const {
		return velocity.x;
	}

	float GetYVelocity() const {
		return velocity.y;
	}

	void SetVelocity(D3DXVECTOR2 velocity) {
		this->velocity = velocity;
	}

	void SetXVelocity(float value) {
		this->velocity.x = value;
	}

	void SetYVelocity(float value) {
		this->velocity.y = value;
	}

	float GetXAcceleration() const {
		return acceleration.x;
	}

	float GetYAcceleration() const {
		return acceleration.y;
	}

	D3DXVECTOR2 GetAcceleration() const {
		return acceleration;
	}

	void SetAcceleration(D3DXVECTOR2 acceleration) {
		this->acceleration = acceleration;
	}

	void SetXAcceleration(float value) {
		this->acceleration.x = value;
	}

	void SetYAcceleration(float value) {
		this->acceleration.y = value;
	}

	float GetMass() const {
		return mass;
	}

	void SetMass(float mass) {
		this->mass = mass;
	}

	int GetFrameCounter() const {
		return frameCounter;
	}

	void SetFrameCounter(int frameCounter) {
		this->frameCounter = frameCounter;
	}

	int GetNumber() const {
		return number;
	}

	void SetNumber(int number) {
		this->number = number;
	}

	D3DXVECTOR2 GetScaling() const {
		return scaling;
	}

	void SetScaling(D3DXVECTOR2 scaling) {
		this->scaling = scaling;
	}

	D3DXVECTOR2 GetSpriteCenter() const {
		return spriteCenter;
	}

	void SetSpriteCenter(D3DXVECTOR2 spriteCenter) {
		this->spriteCenter = spriteCenter;
	}

	float GetDirection() const {
		return direction;
	}

	void SetDirection(float direction) {
		this->direction = direction;
	}

	int GetHP() {
		return hp;
	}

	void SetHP(int value) {
		hp = value;
	}

	void Draw(D3DXMATRIX* mat, LPD3DXSPRITE spriteBrush, LPDIRECT3DTEXTURE9 playerTexture) {
		D3DXMatrixTransformation2D(mat, NULL, 0.0, &scaling, &spriteCenter, direction, &position);
		spriteBrush->SetTransform(mat);
		spriteBrush->Draw(playerTexture, &animRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	}


};

class Player: public GameObject {
	private:
		float playerEngineForce;
		float playerRotationSpeed;
	public:	
		// functions
		void Init(int playerNumber, int PlayerXPosition, int PlayerYPosition);
		void setPlayerAccelerationX(double X);
		void setPlayerAccelerationY(double Y);

		float GetPlayerEngineForce() const {
			return playerEngineForce;
		}

		void SetPlayerEngineForce(float playerEngineForce) {
			this->playerEngineForce = playerEngineForce;
		}

		float GetPlayerRotationSpeed() const {
			return playerRotationSpeed;
		}

		void SetPlayerRotationSpeed(float playerRotationSpeed) {
			this->playerRotationSpeed = playerRotationSpeed;
		}
};

class StaticObject: public GameObject {
	public:
		void Init(int objectNumber);
};