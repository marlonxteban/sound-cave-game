#pragma once
#include <iostream>
#include <string>

enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

enum class GameStatus
{
	OnProgress,
	Win,
	Lose,
	Finished
};

std::string DirectionToString(Direction direction);