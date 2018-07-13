#pragma once

#include "Actor.h"
#include "SoundEvent.h"

class SphereActor : public Actor
{
public:
	SphereActor(class Game* game);

	void UpdateActor(float deltaTime) override;

	const static float mSpeed;

private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class MeshComponent* mMeshComp;
};