
#include "SpotLightComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Actor.h"

SpotLightComponent::SpotLightComponent(class Actor* _pa)
	: Component(_pa)
{
	_pa->GetGame()->GetRenderer()->AddSpotLight(this);
}

SpotLightComponent::~SpotLightComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSpotLight(this);
}

void SpotLightComponent::Draw(class Shader* _ps, class Mesh* _pm)
{
	Matrix4 scale = Matrix4::CreateScale(mOwner->GetScale() * mOuterRadius / _pm->GetRadius());
	Matrix4 rotat = Matrix4::CreateFromQuaternion(mRotation);
	Matrix4 trans = Matrix4::CreateTranslation(mOwner->GetPosition());
	Matrix4 world = scale * rotat * trans;
	_ps->SetMatrixUniform("uWorldTransform", world);

	_ps->SetVectorUniform("uSpotLight.mWorldPos", mOwner->GetPosition());
	_ps->SetVectorUniform("uSpotLight.mDiffuseColor", mDiffuseColor);
	_ps->SetFloatUniform("uSpotLight.mInnerRadius", mInnerRadius);
	_ps->SetFloatUniform("uSpotLight.mOuterRadius", mOuterRadius);
	_ps->SetVectorUniform("uSpotLight.mSpecularColor", mSpecularColor);

	glDrawElements(GL_TRIANGLES, _pm->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}
