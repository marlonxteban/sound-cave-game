#include "Player.h"

Player::Player() : BaseEntity(3, 0 ,0)
{
	direction = Direction::Right;
}

Player::~Player()
{
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
	int positionX = getPositionX();
	int positionY = getPositionY();
	switch (direction)
	{
	case Direction::Up:
		setPositionY(positionY-1);
		break;
	case Direction::Right:
		setPositionX(positionX+1);
		break;
	case Direction::Down:
		setPositionY(positionY+1);
		break;
	case Direction::Left:
		setPositionX(positionX-1);
		break;
	}
}

Direction Player::getDirection()
{
	return direction;
}

bool Player::canMoveForward(Cell* cell)
{
	return cell->getCollider() != 1; //1 means wall
}
