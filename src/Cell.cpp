#include "Cell.h"

Cell::Cell() : collider(false)
{
}
Cell::~Cell()
{
}
bool Cell::getCollider()
{
	return collider;
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
	return '_';
}

