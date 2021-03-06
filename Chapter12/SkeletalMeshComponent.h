// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include <vector>
#include <string>
#include "MeshComponent.h"
#include "MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
	// Draw this mesh component
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(const class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation
	float PlayAnimation(class Animation* anim, float playRate = 1.0f, float blendTime = 0.1f);

	Vector3 GetBonePosition(std::string&);
protected:
	void ComputeMatrixPalette();

	MatrixPalette mPalette;
	const class Skeleton* mSkeleton;
	
	const class Animation* mAnimation;
	float mAnimPlayRate;
	float mAnimTime;

	std::vector<Matrix4> mCurrentPoseVector;
	
	float mMaxBlendTime;
	float mBlendTime;
	const class Animation* mNextAnimation;
	float mNextAnimPlayRate;
	float mNextAnimTime;
};
