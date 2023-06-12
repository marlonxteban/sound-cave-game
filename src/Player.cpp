#include "Player.h"

Player::Player() : positionX(0), positionY(0), direction(Direction::Right), collider(3)
{
}

Player::~Player()
{
}

void Player::setPosition(int x, int y)
{
	positionX = x;
	positionY = y;
}

void Player::setDirection(Direction dir)
{
	direction = dir;
}

void Player::turnRight()
{
	switch (direction)
	{
	case Direction::Up:
		direction = Direction::Right;
		break;
	case Direction::Right:
		direction = Direction::Down;
		break;
	case Direction::Down:
		direction = Direction::Left;
		break;
	case Direction::Left:
		direction = Direction::Up;
		break;
	}
}

void Player::turnLeft()
{
	switch (direction)
	{
	case Direction::Up:
		direction = Direction::Left;
		break;
	case Direction::Right:
		direction = Direction::Up;
		break;
	case Direction::Down:
		direction = Direction::Right;
		break;
	case Direction::Left:
		direction = Direction::Down;
		break;
	}
}

void Player::moveForward()
{
	switch (direction)
	{
	case Direction::Up:
		positionY--;
		break;
	case Direction::Right:
		positionX++;
		break;
	case Direction::Down:
		positionY++;
		break;
	case Direction::Left:
		positionX--;
		break;
	}
}

int* Player::getPosition()
{
	int* position = new int[2];
	position[0] = positionX;
	position[1] = positionY;
	return position;
}

Direction Player::getDirection()
{
	return direction;
}

int Player::getCollider()
{
	return collider;
}

bool Player::canMoveForward(Cell* cell)
{
	return cell->getCollider() == 0; //0 means no collider
}
