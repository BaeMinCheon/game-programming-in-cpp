// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <map>
#include <string>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation every frame (overriden from component)
	void Update(float deltaTime) override;
	// Set the textures used for animation
	void SetAnimTextures(std::string& key,
		const std::vector<SDL_Texture*>& textures, bool isLoop = true);
	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	void SetCurrAnim(std::string& anim)
	{
		mCurrAnim = anim;
		mCurrFrame = 0.0f;
	}
private:
	// All textures in the animation
	std::map<std::string, std::pair<std::vector<SDL_Texture*>, bool>> mAnimMap;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
	// Current anim indicator
	std::string mCurrAnim;
};