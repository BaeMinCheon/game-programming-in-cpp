// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen
{
public:
	// (Lower draw order corresponds with further back)
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	
	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);
protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);
	
	class Texture* mHealthBar;
	class Texture* mRadar;
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mBlipDownTex;
	class Texture* mBlipUpTex;
	class Texture* mRadarArrow;
	class Texture* mTargetArrow;
	
	// All the target components in the game
	std::vector<class TargetComponent*> mTargetComps;
	// 2D offsets of blips relative to radar
	std::vector<Vector2> mBlips;
	std::vector<Vector2> mBlipDowns;
	std::vector<Vector2> mBlipUps;
	// Adjust range of radar and radius
	float mRadarRange;
	float mRadarRadius;
	// Whether the crosshair targets an enemy
	bool mTargetEnemy;
	// angle for arrow target
	float mAngle;
};
