#pragma once
#include "BaseEntity.h"

class Player : public BaseEntity
{
	enum facingDirection { UP, DOWN, LEFT, RIGHT };
private:
	int positionX;
	int positionY;
	int collider;
	facingDirection direction;
public:
	Player();
	~Player();

	void setPosition(int x, int y);
	void setDirection(facingDirection dir);
	void turnRight();
	void turnLeft();
	void moveForward();

	int* getPosition();
	facingDirection getDirection();
	int getCollider();
};