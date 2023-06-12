#include "Enemy.h"

Enemy::Enemy(): BaseEntity(4, 0, 0)
{

}

Enemy::~Enemy()
{

}

void Enemy::move(Cell* adjacentCells)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 3);

    int randomIndex = distrib(gen);
    while (adjacentCells[randomIndex].getCollider() == 1)
    {
	    randomIndex = distrib(gen);
    }
    Cell nextCell = adjacentCells[randomIndex];
    setPosition(nextCell.getPosition()[0], nextCell.getPosition()[1]);
}
