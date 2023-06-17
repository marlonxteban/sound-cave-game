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

double Player::getAngleToEnemy(BaseEntity* enemy)
{
	double angle = atan2(enemy->getPositionY() - getPositionY(), enemy->getPositionX() - getPositionX());
	return getRelativeAngle(angle);
}

double Player::getAngleToExit(int exitX, int exitY)
{
	double angle = atan2(exitY - getPositionY(), exitX - getPositionX());
	return getRelativeAngle(angle);
}

double Player::getRelativeAngle(double angle)
{
	switch (direction)
	{
	case Direction::Right:
		break;
	case Direction::Up:
		angle += PI / 2;
		if (angle > PI)
			angle -= 2 * PI;
		break;
	case Direction::Left:
		angle += PI;
		if (angle > PI)
			angle -= 2 * PI;
		break;
	case Direction::Down:
		angle -= PI / 2;
		if (angle > PI)
			angle -= 2 * PI;
		break;
	}
	return angle * 180 / PI;
}
