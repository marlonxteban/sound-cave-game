#include "Cell.h"

Cell::Cell() : collider(false), positionX(0), positionY(0)
{
}

Cell::Cell(int collider, int x, int y): collider(collider), positionX(x), positionY(y)
{
}

Cell::~Cell()
{
}
bool Cell::getCollider()
{
	return collider;
}

int* Cell::getPosition()
{
	int* position = new int[2];
	position[0] = positionX;
	position[1] = positionY;
	return position;
}

void Cell::setPosition(int x, int y)
{
	this->positionX = x;
	this->positionY = y;
}

void Cell::setCollider(int newCollider)
{
	this->collider = newCollider;
}

char Cell::getPrintChar()
{
	if (collider == 1)
	{
		return 'X';
	}
	if (collider == 2)// exit
	{
		return 'O';
	}
	if (collider == 3)// player
	{
		return 'P';
	}
	return '_';
}

