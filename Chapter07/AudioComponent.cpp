// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AudioComponent.h"
#include "Actor.h"
#include "Game.h"
#include "AudioSystem.h"
#include "CameraActor.h"

AudioComponent::AudioComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder),
	mFowardSpeed(0.0f)
{
}

AudioComponent::~AudioComponent()
{
	StopAllEvents();
}

void AudioComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	// Remove invalid 2D events
	auto iter = mEvents2D.begin();
	while (iter != mEvents2D.end())
	{
		if (!iter->IsValid())
		{
			iter = mEvents2D.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// Remove invalid 3D events
	iter = mEvents3D.begin();
	while (iter != mEvents3D.end())
	{
		if (!iter->IsValid())
		{
			iter = mEvents3D.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void AudioComponent::OnUpdateWorldTransform()
{
	// Update 3D events' world transforms
	auto cameraPos = mOwner->GetGame()->GetCameraActor()->GetCameraPosition();
	auto playerPos = mOwner->GetGame()->GetCameraActor()->GetPlayerPosition();
	auto virtualPos = (mOwner->GetPosition() - cameraPos) *
		((mOwner->GetPosition() - playerPos).Length() / (mOwner->GetPosition() - cameraPos).Length());
	Matrix4 world = Matrix4::CreateScale(mOwner->GetScale());
	world *= Matrix4::CreateFromQuaternion(mOwner->GetRotation());
	world *= Matrix4::CreateTranslation(virtualPos);
	for (auto& event : mEvents3D)
	{
		if (event.IsValid())
		{
			event.Set3DAttributes(world, mOwner->GetForward() * mFowardSpeed);
		}
	}
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
	SoundEvent e = mOwner->GetGame()->GetAudioSystem()->PlayEvent(name);
	// Is this 2D or 3D?
	if (e.Is3D())
	{
		mEvents3D.emplace_back(e);
		// Set initial 3D attributes
		e.Set3DAttributes(mOwner->GetWorldTransform(), mOwner->GetForward() * mFowardSpeed);
	}
	else
	{
		mEvents2D.emplace_back(e);
	}
	return e;
}

void AudioComponent::StopAllEvents()
{
	// Stop all sounds
	for (auto& e : mEvents2D)
	{
		e.Stop();
	}
	for (auto& e : mEvents3D)
	{
		e.Stop();
	}
	// Clear events
	mEvents2D.clear();
	mEvents3D.clear();
}
