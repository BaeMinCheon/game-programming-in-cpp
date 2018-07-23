// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Laser.h"
#include "InputSystem.h"

Ship::Ship(Game* game, int conID)
	:Actor(game),
	mSpeed(400.0f),
	mLaserCooldown(0.0f),
	mControllerID(conID)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;

	// Update position based on velocity
	Vector2 pos = GetPosition();
	pos += mVelocityDir * mSpeed * deltaTime;
	SetPosition(pos);

	// Update rotation
	float angle = Math::Atan2(mRotationDir.y, mRotationDir.x);
	SetRotation(angle);
}

void Ship::ActorInput(const InputState& state)
{
	if (state.GetMappedButtonState("Fire") == ButtonState::EReleased
		&& mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (quarter second)
		mLaserCooldown = 0.25f;
	}

	if (state.Controllers[mControllerID].GetIsConnected())
	{
		mVelocityDir = state.Controllers[mControllerID].GetLeftStick();
		if (!Math::NearZero(state.Controllers[mControllerID].GetRightStick().Length()))
		{
			mRotationDir = state.Controllers[mControllerID].GetRightStick();
		}
	}
}
