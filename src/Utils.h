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

std::string DirectionToString(Direction direction);