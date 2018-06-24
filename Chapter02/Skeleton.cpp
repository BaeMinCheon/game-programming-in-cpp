#include "Skeleton.h"
#include "Game.h"

Skeleton::Skeleton(Game* game)
	: Actor(game),
	mRightSpeed(0.0f),
	mDownSpeed(0.0f)
{
	mpASC = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> walkingAnims =
	{
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png")
	};
	mpASC->SetAnimTextures(std::string("walking"), walkingAnims, false);
	std::vector<SDL_Texture*> jumpingAnims =
	{
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png")
	};
	mpASC->SetAnimTextures(std::string("jumping"), jumpingAnims, false);
	std::vector<SDL_Texture*> punchingAnims =
	{
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png")
	};
	mpASC->SetAnimTextures(std::string("punching"), punchingAnims, false);
	
	mpASC->SetCurrAnim(std::string("NAN"));
}

void Skeleton::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	
	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	}
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);
}

void Skeleton::ProcessKeyboard(const uint8_t* state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;

	if (state[SDL_SCANCODE_W])
	{
		mpASC->SetCurrAnim(std::string("walking"));
	}
	if (state[SDL_SCANCODE_SPACE])
	{
		mpASC->SetCurrAnim(std::string("jumping"));
	}
	if (state[SDL_SCANCODE_P])
	{
		mpASC->SetCurrAnim(std::string("punching"));
	}
}
