// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <cstddef>
#include <string>
#include "Mesh.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, class Mesh* mesh);
	~MeshComponent();
	// Draw this mesh component
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

	const std::string& GetShaderName() const { return mMesh->GetShaderName(); }
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
};
