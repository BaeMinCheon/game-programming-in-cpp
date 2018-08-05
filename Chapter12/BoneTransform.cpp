// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const
{
	Matrix4 rot = Matrix4::CreateFromQuaternion(mRotation);
	Matrix4 trans = Matrix4::CreateTranslation(mTranslation);

	return rot * trans;
}

BoneTransform BoneTransform::ToBoneTransform(const Matrix4& _m)
{
	BoneTransform retVal;

	Quaternion q;
	q.w = Math::Sqrt(Math::Max(0.0f, 1.0f + _m.mat[0][0] + _m.mat[1][1] + _m.mat[2][2])) / 2.0f;
	q.x = Math::Sqrt(Math::Max(0.0f, 1.0f + _m.mat[0][0] - _m.mat[1][1] - _m.mat[2][2])) / 2.0f;
	q.y = Math::Sqrt(Math::Max(0.0f, 1.0f - _m.mat[0][0] + _m.mat[1][1] - _m.mat[2][2])) / 2.0f;
	q.z = Math::Sqrt(Math::Max(0.0f, 1.0f - _m.mat[0][0] - _m.mat[1][1] + _m.mat[2][2])) / 2.0f;
	q.x *= (q.x * (_m.mat[1][2] - _m.mat[2][1]) >= 0) ? 1.0f : -1.0f;
	q.y *= (q.y * (_m.mat[2][0] - _m.mat[0][2]) >= 0) ? 1.0f : -1.0f;
	q.z *= (q.z * (_m.mat[0][1] - _m.mat[1][0]) >= 0) ? 1.0f : -1.0f;
	retVal.mRotation = q;
	
	Vector3 v = _m.GetTranslation();
	retVal.mTranslation = v;

	return retVal;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
	retVal.mTranslation = Vector3::Lerp(a.mTranslation, b.mTranslation, f);
	return retVal;
}
