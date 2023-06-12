#pragma once
#include "BaseEntity.h"
#include "Cell.h"
#include "Utils.h"

class Player : public BaseEntity
{
private:
	int positionX;
	int positionY;
	int collider;
	Direction direction;
public:
	Player();
	~Player();

	void setPosition(int x, int y);
	void setDirection(Direction dir);
	void turnRight();
	void turnLeft();
	void moveForward();

	int* getPosition();
	Direction getDirection();
	int getCollider();
	bool canMoveForward(Cell* cell);
};