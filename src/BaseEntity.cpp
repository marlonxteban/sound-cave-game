#include "BaseEntity.h"

BaseEntity::BaseEntity() : collider(0), positionX(0), positionY(0)
{
}

BaseEntity::BaseEntity(int collider, int x, int y) : collider(collider), positionX(x), positionY(y)
{
}

BaseEntity::~BaseEntity()
{
}

int BaseEntity::getCollider()
{
	return collider;
}

int* BaseEntity::getPosition()
{
	int* position = new int[2];
	position[0] = positionX;
	position[1] = positionY;
	return position;
}

int BaseEntity::getPositionX()
{
	return positionX;
}

int BaseEntity::getPositionY()
{
	return positionY;
}

void BaseEntity::setPosition(int x, int y)
{
	this->positionX = x;
	this->positionY = y;
}

void BaseEntity::setPositionX(int x)
{
	this->positionX = x;
}

void BaseEntity::setPositionY(int y)
{
	this->positionY = y;
}

void BaseEntity::setCollider(int newCollider)
{
	this->collider = newCollider;
}

char BaseEntity::getPrintChar()
{
	if (collider == 3)
	{
		return 'P';
	}
	if (collider == 4)
	{
		return 'E';
	}
	return '?';
}