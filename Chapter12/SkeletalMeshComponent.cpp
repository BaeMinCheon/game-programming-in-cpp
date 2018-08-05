// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	:MeshComponent(owner, true)
	,mSkeleton(nullptr)
	,mBlendTime(0.0f)
{
}

void SkeletalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		// Set the matrix palette
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], 
			MAX_SKELETON_BONES);
		// Set specular power
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;
		// Wrap around anim time if past duration
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}
		
		if (mNextAnimation)
		{
			mNextAnimTime += deltaTime * mNextAnimPlayRate;
			while (mNextAnimTime > mNextAnimation->GetDuration())
			{
				mNextAnimTime -= mNextAnimation->GetDuration();
			}

			if (mBlendTime > 0.0f)
			{
				mBlendTime -= deltaTime;
			}
			else
			{
				mBlendTime = 0.0f;

				mAnimation = mNextAnimation;
				mAnimTime = mNextAnimTime;
				mAnimPlayRate = mNextAnimPlayRate;

				mNextAnimation = nullptr;
			}
		}

		// Recompute matrix palette
		ComputeMatrixPalette();

		// for debugging
		std::string boneName("pelvis");
		auto pos = GetBonePosition(boneName);
		SDL_Log("object space position of [ %s ] : ( %f %f %f )",
			boneName.c_str(), pos.x, pos.y, pos.z);
	}
}

float SkeletalMeshComponent::PlayAnimation(Animation* anim, float playRate, float blendTime)
{
	if (!mAnimation)
	{
		mAnimation = anim;
		mAnimTime = 0.0f;
		mAnimPlayRate = playRate;
		
		mBlendTime = 0.0f;

		if (!mAnimation) { return 0.0f; }

		ComputeMatrixPalette();

		return mAnimation->GetDuration();
	}
	else
	{
		mNextAnimation = anim;
		mNextAnimTime = 0.0f;
		mNextAnimPlayRate = playRate;

		mBlendTime = mMaxBlendTime = blendTime;

		if (!mNextAnimation) { return 0.0f; }

		ComputeMatrixPalette();

		return mNextAnimation->GetDuration();
	}
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();
	
	if (mBlendTime > 0.0f)
	{
		std::vector<Matrix4> firstAnim;
		std::vector<Matrix4> secondAnim;

		mAnimation->GetGlobalPoseAtTime(firstAnim, mSkeleton, mAnimTime);
		mNextAnimation->GetGlobalPoseAtTime(secondAnim, mSkeleton, mNextAnimTime);

		auto num = mAnimation->GetNumBones();
		if (mCurrentPoseVector.size() < num)
		{
			mCurrentPoseVector.resize(num);
		}

		auto scale = mBlendTime / mMaxBlendTime;
		for (int i = 0; i < num; ++i)
		{
			auto interp = BoneTransform::Interpolate(BoneTransform::ToBoneTransform(firstAnim[i]),
				BoneTransform::ToBoneTransform(secondAnim[i]), scale);
			auto mat = interp.ToMatrix();

			mCurrentPoseVector[i] = mat;
		}
	}
	else
	{
		mAnimation->GetGlobalPoseAtTime(mCurrentPoseVector, mSkeleton, mAnimTime);
	}

	// Setup the palette for each bone
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// Global inverse bind pose matrix times current pose matrix
		mPalette.mEntry[i] = globalInvBindPoses[i] * mCurrentPoseVector[i];
	}
}

Vector3 SkeletalMeshComponent::GetBonePosition(std::string& _n)
{
	Vector3 retVal(1.0f, 1.0f, 1.0f);

	auto& bv = mSkeleton->GetBones();
	for (int i = 0; i < bv.size(); ++i)
	{
		if (bv[i].mName == _n)
		{
			//retVal = mCurrentPoseVector[i].GetTranslation();
			retVal = Vector3::Transform(Vector3::Zero, mCurrentPoseVector[i]);

			break;
		}
	}

	return retVal;
}
