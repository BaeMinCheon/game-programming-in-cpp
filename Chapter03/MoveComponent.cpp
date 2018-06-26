// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
:Component(owner, updateOrder),
mAngularSpeed(0.0f),
mForwardSpeed(0.0f),
mMass(1.0f)
{
	mForce.x = mForce.y = mVelocity.x = mVelocity.y = 0.0f;
}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	
	{
		Vector2 pos = mOwner->GetPosition();
		Vector2 acceleration = mForce * (1.0f / mMass);
		mForce.x = mForce.y = 0.0f;
		mVelocity += acceleration * deltaTime;
		if (mVelocity.Length() > mMaxForwardSpeed)
		{
			mVelocity.Normalize();
			mVelocity *= mMaxForwardSpeed;
		}

		pos += mVelocity * deltaTime;

		// (Screen wrapping code only for asteroids)
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

		mOwner->SetPosition(pos);
	}

	if (!Math::NearZero(mForwardSpeed))
	{
		AddForce(mOwner->GetForward()*(mForwardSpeed / deltaTime * mMass));
	}
}

// input W == adding force
// ¤·¤¤¤·d