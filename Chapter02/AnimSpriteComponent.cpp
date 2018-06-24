// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder),
	mCurrFrame(0.0f),
	mAnimFPS(24.0f),
	mCurrAnim("NAN")
{
	;
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	auto& currAnim = mAnimMap[mCurrAnim];

	if (mCurrAnim != "NAN" &&
		currAnim.first.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;
		
		// Wrap current frame if needed
		while (mCurrFrame >= currAnim.first.size())
		{
			if (currAnim.second)
			{
				mCurrFrame -= currAnim.first.size();
			}
			else
			{
				mCurrFrame = currAnim.first.size() - 1;
			}
		}

		// Set the current texture
		SetTexture(currAnim.first[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(std::string& key,
	const std::vector<SDL_Texture*>& textures, bool isLoop)
{
	mAnimMap[key].first = textures;
	mAnimMap[key].second = isLoop;
	if (mAnimMap[key].first.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimMap[key].first[0]);
	}
}
