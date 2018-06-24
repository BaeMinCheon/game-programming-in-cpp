#pragma once

#include <string>
#include <vector>
#include "SpriteComponent.h"

const static int TILE_WIDTH = 32;
const static int TILE_HEIGHT = 32;
const static int TILE_ROWS = 24;
const static int TILE_COLS = 8;
const static int MAP_WIDTH = 32;
const static int MAP_HEIGHT = 24;

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(class Actor*, int = 10);

	void Update(float) override;
	void Draw(SDL_Renderer*) override;

	void SetMap(const std::string&);

private:
	std::vector<std::vector<int>> mMapVector;
};