#include "Player.h"

Player::Player() : positionX(0), positionY(0), direction(RIGHT), collider(3)
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

void Player::setDirection(facingDirection dir)
{
	direction = dir;
}

void Player::turnRight()
{
	switch (direction)
	{
	case UP:
		direction = RIGHT;
		break;
	case RIGHT:
		direction = DOWN;
		break;
	case DOWN:
		direction = LEFT;
		break;
	case LEFT:
		direction = UP;
		break;
	}
}

void Player::turnLeft()
{
	switch (direction)
	{
	case UP:
		direction = LEFT;
		break;
	case RIGHT:
		direction = UP;
		break;
	case DOWN:
		direction = RIGHT;
		break;
	case LEFT:
		direction = DOWN;
		break;
	}
}

void Player::moveForward()
{
	switch (direction)
	{
	case UP:
		positionY--;
		break;
	case RIGHT:
		positionX++;
		break;
	case DOWN:
		positionY++;
		break;
	case LEFT:
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

Player::facingDirection Player::getDirection()
{
	return direction;
}

int Player::getCollider()
{
	return collider;
}
