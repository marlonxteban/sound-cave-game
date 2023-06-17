#pragma once
#include "BaseEntity.h"
#include "Cell.h"
#include "Utils.h"

class Player : public BaseEntity
{
#define MAX_DISTANCE 255
#define MIN_DISTANCE 9
private:
	Direction direction;
public:
	Player();
	~Player();

	void setDirection(Direction dir);
	void turnRight();
	void turnLeft();
	void moveForward();

	Direction getDirection();
	bool canMoveForward(Cell* cell);
	double getAngleToEnemy(BaseEntity* enemy);
	double getAngleToExit(int exitX, int exitY);
	double getRelativeAngle(double angle);
	double getDistanceToPosition(int positionX, int positionY);
};