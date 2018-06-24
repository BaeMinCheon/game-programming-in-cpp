#include <fstream>
#include "TileMapComponent.h"

TileMapComponent::TileMapComponent(class Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
{
	;
}

void TileMapComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	;
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	for (auto map : mMapVector)
	{
		for (int row = 0; row < MAP_HEIGHT; ++row)
		{
			for (int col = 0; col < MAP_WIDTH; ++col)
			{
				int& tileIndex = map.at(row * MAP_WIDTH + col);

				if (tileIndex != -1)
				{
					SDL_Rect src;
					src.w = TILE_WIDTH;
					src.h = TILE_HEIGHT;
					src.x = (tileIndex % 8) * TILE_WIDTH;
					src.y = (tileIndex / 8) * TILE_HEIGHT;

					SDL_Rect dst;
					dst.w = TILE_WIDTH;
					dst.h = TILE_HEIGHT;
					dst.x = col * TILE_WIDTH;
					dst.y = row * TILE_HEIGHT;

					SDL_RenderCopyEx(renderer,
						mTexture,
						&src,
						&dst,
						0.0,
						nullptr,
						SDL_FLIP_NONE);
				}
			}
		}
	}
}

void TileMapComponent::SetMap(const std::string& fileName)
{
	std::vector<int> map;
	std::ifstream fs(fileName);
	std::string cell;
	
	while (!fs.eof())
	{
		std::getline(fs, cell, ',');
		int index = cell.find('\n');
		if (index != std::string::npos)
		{
			cell.erase(index, 1);
			std::string former = cell.substr(0, index);
			std::string later = cell.substr(index, cell.size() - index);

			map.push_back(std::stoi(former));
			if (!later.empty())
			{
				map.push_back(std::stoi(cell));
			}
		}
		else
		{
			map.push_back(std::stoi(cell));
		}
	}

	mMapVector.push_back(map);

	fs.close();
}