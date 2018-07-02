// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Board.h"
#include "Random.h"

BoardState::BoardState()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			mBoard[i][j] = Empty;
		}
	}
}

std::vector<BoardState*> BoardState::GetPossibleMoves(SquareState player) const
{
	std::vector<BoardState*> retVal;

	// For each column, find if a move is possible
	for (int col = 0; col < 7; col++)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (mBoard[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(new BoardState(*this));
				retVal.back()->mBoard[row][col] = player;
				break;
			}
		}
	}

	return retVal;
}

std::vector<BoardState*> BoardState::GetPossibleMovesAll(SquareState player) const
{
	std::vector<BoardState*> retVal;

	// For each column, find if a move is possible
	for (int col = 0; col < 7; ++col)
	{
		if (mBoard[5][col] == BoardState::Empty)
		{
			retVal.emplace_back(new BoardState(*this));
			retVal.back()->mBoard[5][col] = player;
		}
	}
	for (int row = 4; row >= 0; --row)
	{
		for (int col = 0; col < 7; ++col)
		{
			if (mBoard[row + 1][col] != BoardState::Empty &&
				mBoard[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(new BoardState(*this));
				retVal.back()->mBoard[row][col] = player;
			}
		}
	}

	return retVal;
}

std::vector<std::pair<int, int>> BoardState::GetPossiblePosAll() const
{
	std::vector<std::pair<int, int>> retVal;

	for (int col = 0; col < 7; ++col)
	{
		if (mBoard[5][col] == BoardState::Empty)
		{
			retVal.emplace_back(std::make_pair(5, col));
		}
	}
	for (int row = 4; row >= 0; --row)
	{
		for (int col = 0; col < 7; ++col)
		{
			if (mBoard[row + 1][col] != BoardState::Empty &&
				mBoard[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(std::make_pair(row, col));
			}
		}
	}

	return retVal;
}

bool BoardState::IsTerminal() const
{
	// Is the board full?
	if (IsFull())
	{
		return true;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return true;
	}

	return false;
}

float BoardState::GetScore() const
{
	// If the board is full, the score is 0
	if (IsFull())
	{
		return 0.0f;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return static_cast<float>(fourInRow);
	}

	return CalculateHeuristic();
}

bool BoardState::IsFull() const
{
	bool isFull = true;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (mBoard[i][j] == Empty)
			{
				isFull = false;
			}
		}
	}

	return isFull;
}

int BoardState::GetFourInARow() const
{
	// Returns -1 if yellow wins, 1 if red wins, 0 otherwise

	// Check if there's a row with four in a row
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (mBoard[row][col] == mBoard[row][col + 1] &&
				mBoard[row][col] == mBoard[row][col + 2] &&
				mBoard[row][col] == mBoard[row][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a column with four in a row
	for (int col = 0; col < 7; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col] &&
				mBoard[row][col] == mBoard[row + 2][col] &&
				mBoard[row][col] == mBoard[row + 3][col])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a right diagonal four in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
				mBoard[row][col] == mBoard[row + 2][col + 2] &&
				mBoard[row][col] == mBoard[row + 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a left diagonal for in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 3; row < 6; row++)
		{
			if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
				mBoard[row][col] == mBoard[row - 2][col + 2] &&
				mBoard[row][col] == mBoard[row - 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

float BoardState::CalculateHeuristic() const
{
	float retVal = 0.0f;
	std::vector<std::pair<int, int>> moves = this->GetPossiblePosAll();

	for (std::pair<int, int> pos : moves)
	{
		// horizonal
		if (pos.second > 0 &&
			mBoard[pos.first][pos.second - 1] == BoardState::Yellow)
		{
			retVal += 0.1f;
		}
		if (pos.second < 6 &&
			mBoard[pos.first][pos.second + 1] == BoardState::Yellow)
		{
			retVal += 0.1f;
		}

		// vertical
		if (pos.first > 0 &&
			mBoard[pos.first - 1][pos.second] == BoardState::Yellow)
		{
			retVal += 0.1f;
		}
		if (pos.first < 6 &&
			mBoard[pos.first + 1][pos.second] == BoardState::Yellow)
		{
			retVal += 0.1f;
		}

		// diagonal : left top 2 right bottom
		if (pos.first > 0 && pos.second > 0 &&
			mBoard[pos.first - 1][pos.second - 1] == BoardState::Yellow)
		{
			retVal += 0.1f;
		}
		if (pos.first < 6 && pos.second < 6 &&
			mBoard[pos.first + 1][pos.second + 1] == BoardState::Yellow)
		{
			retVal += 0.1f;
		}

		// diagonal : left bottom 2 right top
		if (pos.first < 6 && pos.second > 0 &&
			mBoard[pos.first + 1][pos.second - 1] == BoardState::Yellow)
		{
			retVal += 0.1f;
		}
		if (pos.first > 0 && pos.second < 6 &&
			mBoard[pos.first - 1][pos.second + 1] == BoardState::Yellow)
		{
			retVal += 0.1f;
		}
	}

	return retVal;
}

bool TryPlayerMove(BoardState* state, int column)
{
	// Find the first row in that column that's available
	// (if any)
	for (int row = 5; row >= 0; row--)
	{
		if (state->mBoard[row][column] == BoardState::Empty)
		{
			state->mBoard[row][column] = BoardState::Yellow;
			return true;
		}
	}

	return false;
}

void CPUMove(BoardState* state)
{
	// For now, this just randomly picks one of the possible moves
	std::vector<BoardState*> moves = state->GetPossibleMoves(BoardState::Red);

	int index = Random::GetIntRange(0, moves.size() - 1);

	*state = *moves[index];

	// Clear up memory from possible moves
	for (auto state : moves)
	{
		delete state;
	}
}

void GenStates(struct GTNode* root, bool isPlayer, int maxDepth)
{
	if (maxDepth <= 0)
	{
		return;
	}

	std::vector<BoardState*> moves =
		root->mState.GetPossibleMovesAll(isPlayer ? BoardState::Yellow : BoardState::Red);

	for (std::size_t i = 0; i < moves.size(); ++i)
	{
		GTNode* node = new GTNode;
		root->mChildren.emplace_back(node);
		node->mState = *moves[i];
		GenStates(node, !isPlayer, maxDepth - 1);
	}
}

BoardState AlphaBetaDecide(GTNode* node, int maxDepth)
{
	GTNode* choice = nullptr;
	float alpha = -std::numeric_limits<float>::infinity();
	float beta = +std::numeric_limits<float>::infinity();
	for (auto child : node->mChildren)
	{
		float v = AlphaBetaMin(child, maxDepth - 1, alpha, beta);
		if (v > alpha)
		{
			alpha = v;
			choice = child;
		}
	}

	return choice->mState;
}

float AlphaBetaMax(GTNode* node, int maxDepth, float alpha, float beta)
{
	if (maxDepth == 0 || node->mChildren.empty())
	{
		return node->mState.GetScore();
	}

	float maxValue = -std::numeric_limits<float>::infinity();

	for (auto child : node->mChildren)
	{
		maxValue = std::max(maxValue, AlphaBetaMin(child, maxDepth - 1, alpha, beta));
		if (maxValue >= beta)
		{
			return maxValue;
		}
		alpha = std::max(maxValue, alpha);
	}
	
	return maxValue;
}

float AlphaBetaMin(GTNode* node, int maxDepth, float alpha, float beta)
{
	if (maxDepth == 0 || node->mChildren.empty())
	{
		return node->mState.GetScore();
	}

	float minValue = std::numeric_limits<float>::infinity();

	for (auto child : node->mChildren)
	{
		minValue = std::max(minValue, AlphaBetaMin(child, maxDepth - 1, alpha, beta));
		if (minValue <= beta)
		{
			return minValue;
		}
		alpha = std::max(minValue, alpha);
	}

	return minValue;
}