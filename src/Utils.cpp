#include "Utils.h"

std::string DirectionToString(Direction direction)
{
switch (direction)
	{
	case Direction::Up:
		return "forward direction: Up";
	case Direction::Down:
		return "forward direction: Down";
	case Direction::Left:
		return "forward direction: Left";
	case Direction::Right:
		return "forward direction: Right";
	default:
		return "forward direction: Unknown";
	}
}
