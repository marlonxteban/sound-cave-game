#pragma once
#include <random>
#include "BaseEntity.h"
#include "Cell.h"

class Enemy: public BaseEntity
{
private:
public:
	Enemy();
	~Enemy();

	void move(Cell* adjacentCells);
	void attack();
};

