// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr),
	mRenderer(nullptr),
	mTicksCount(0),
	mIsRunning(true),
	mPaddle01Dir(0),
	mPaddle02Dir(0)
{
	;
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow
	(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mPaddle01Pos.x = 10.0f;
	mPaddle01Pos.y = 768.0f / 2.0f;
	mPaddle02Pos.x = 1024.0f - thickness - 10.0f;
	mPaddle02Pos.y = 768.0f / 2.0f;
	mBallVector.push_back
	(
		Ball
		{
			1024.0f / 2.0f,
			768.0f / 2.0f,
			-200.0f,
			235.0f
		}
	);
	mBallVector.push_back
	(
		Ball
		{
			1024.0f / 2.0f,
			768.0f / 2.0f,
			200.0f,
			-235.0f
		}
	);

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle direction based on W/S keys
	mPaddle01Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddle01Dir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddle01Dir += 1;
	}
	mPaddle02Dir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddle02Dir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddle02Dir += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	{
		;
	}

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update paddle position based on direction
	if (mPaddle01Dir != 0)
	{
		mPaddle01Pos.y += mPaddle01Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddle01Pos.y < (paddleH/2.0f + thickness))
		{
			mPaddle01Pos.y = paddleH/2.0f + thickness;
		}
		else if (mPaddle01Pos.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddle01Pos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}
	if (mPaddle02Dir != 0)
	{
		mPaddle02Pos.y += mPaddle02Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddle02Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle02Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle02Pos.y >(768.0f - paddleH / 2.0f - thickness))
		{
			mPaddle02Pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}
	
	for (auto& b : mBallVector)
	{
		// Update ball position based on ball velocity
		b.pos.x += b.vel.x * deltaTime;
		b.pos.y += b.vel.y * deltaTime;
	}
	
	for (auto& b : mBallVector)
	{
		// Bounce if needed
		// Did we intersect with the paddle?
		float diff = mPaddle01Pos.y - b.pos.y;
		// Take absolute value of difference
		diff = (diff > 0.0f) ? diff : -diff;
		if (
			// Our y-difference is small enough
			diff <= (paddleH / 2.0f) &&
			// We are in the correct x-position
			10.0f <= b.pos.x && b.pos.x <= 25.0f &&
			// The ball is moving to the left
			b.vel.x < 0.0f)
		{
			b.vel.x *= -1.0f;
		}
		diff = mPaddle02Pos.y - b.pos.y;
		diff = (diff > 0.0f) ? diff : -diff;
		if (diff <= (paddleH / 2.0f) &&
			(1024.0f - 25.0f) <= b.pos.x && b.pos.x <= (1024.0f - 10.0f) &&
			b.vel.x > 0.0f)
		{
			b.vel.x *= -1.0f;
		}

		// Did the ball go off the screen? (if so, end game)
		if (b.pos.x <= 0.0f)
		{
			mIsRunning = false;
		}
		else if (b.pos.x >= 1024.0f)
		{
			mIsRunning = false;
		}

		// Did the ball collide with the top wall?
		if (b.pos.y <= thickness && b.vel.y < 0.0f)
		{
			b.vel.y *= -1;
		}
		// Did the ball collide with the bottom wall?
		else if (b.pos.y >= (768 - thickness) &&
			b.vel.y > 0.0f)
		{
			b.vel.y *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_Rect wall
	{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw paddle 01
	SDL_Rect paddle01
	{
		static_cast<int>(mPaddle01Pos.x),
		static_cast<int>(mPaddle01Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle01);
	// Draw paddle 02
	SDL_Rect paddle02
	{
		static_cast<int>(mPaddle02Pos.x),
		static_cast<int>(mPaddle02Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle02);
	
	for (auto& b : mBallVector)
	{
		// Draw ball
		SDL_Rect ball{
			static_cast<int>(b.pos.x - thickness / 2),
			static_cast<int>(b.pos.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ball);
	}
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
