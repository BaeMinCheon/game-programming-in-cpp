// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>

class BoardState
{
public:
	enum SquareState { Empty, Red, Yellow };
	BoardState();
	std::vector<BoardState*> GetPossibleMoves(SquareState) const;
	std::vector<BoardState*> GetPossibleMovesAll(SquareState) const;
	std::vector<std::pair<int, int>> BoardState::GetPossiblePosAll() const;
	bool IsTerminal() const;
	float GetScore() const;

	SquareState mBoard[6][7];
protected:
	bool IsFull() const;
	int GetFourInARow() const;
	float CalculateHeuristic() const;
};

// Try to place the player's piece
bool TryPlayerMove(class BoardState* state, int column);

// Make the next CPU move
void CPUMove(class BoardState* state);

struct GTNode
{
	std::vector<GTNode*> mChildren;
	BoardState mState;
};

void GenStates(struct GTNode*, bool, int);
BoardState AlphaBetaDecide(struct GTNode*, int);
float AlphaBetaMax(struct GTNode*, int, float, float);
float AlphaBetaMin(struct GTNode*, int, float, float);