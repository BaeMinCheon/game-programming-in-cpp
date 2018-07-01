// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include "AIComponent.h"
#include "AIState.h"

Tower::Tower(class Game* game)
	: Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));
	
	mMove = new MoveComponent(this);
	//mMove->SetAngularSpeed(Math::Pi);

	mAI = new AIComponent(this);
	mAI->RegisterState(new AIAttack(mAI));
	mAI->RegisterState(new AIPatrol(mAI));
	mAI->ChangeState("Patrol");

	mNextAttack = AttackTime;
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	
	mNextAttack -= deltaTime;
	if (mNextAttack <= 0.0f)
	{
		Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
		if (e)
		{
			// Vector from me to enemy
			Vector2 dir = e->GetPosition() - GetPosition();
			float dist = dir.Length();
			if (dist < AttackRange)
			{
				if (mAI->GetState() != "Attack")
				{
					mAI->ChangeState("Attack");
				}

				// Rotate to face enemy
				SetRotation(Math::Atan2(-dir.y, dir.x));
				// Spawn bullet at tower position facing enemy
				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
			else if(mAI->GetState() != "Patrol")
			{
				mAI->ChangeState("Patrol");
			}
		}
		mNextAttack += AttackTime;
	}
}
