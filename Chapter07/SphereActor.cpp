
#include "SphereActor.h"
#include "MoveComponent.h"
#include "SDL/SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"
#include "MeshComponent.h"

const float SphereActor::mSpeed = 300.0f;

SphereActor::SphereActor(Game* game)
	:Actor(game)
{
	SetPosition(Vector3(500.0f, -75.0f, 0.0f));
	SetScale(1.0f);

	mMoveComp = new MoveComponent(this);
	mMoveComp->SetForwardSpeed(mSpeed);
	
	mAudioComp = new AudioComponent(this);
	mAudioComp->PlayEvent("event:/FireLoop");
	
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
}

void SphereActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	auto& pos = GetPosition();

	if (pos.x > 1000.0f)
	{
		mMoveComp->SetForwardSpeed(-mSpeed);
	}
	else if (pos.x < -1000.0f)
	{
		mMoveComp->SetForwardSpeed(+mSpeed);
	}

	mAudioComp->SetForwardSpeed(mMoveComp->GetForwardSpeed());
}