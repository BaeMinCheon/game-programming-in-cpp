#pragma once

#include "Math.h"
#include "Component.h"

class SpotLightComponent : public Component
{
public:
	SpotLightComponent(class Actor*);
	~SpotLightComponent();

	void Draw(class Shader*, class Mesh*);

	Vector3 mDiffuseColor;
	float mInnerRadius;
	float mOuterRadius;
	Vector3 mSpecularColor;
	Quaternion mRotation;

private:
	;
};