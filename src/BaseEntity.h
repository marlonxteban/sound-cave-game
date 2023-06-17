#pragma once
#include <cmath>

class BaseEntity
{
#define PI 3.14159265

	private:
		int collider;
		int positionX;
		int positionY;
	public:
		BaseEntity();
		BaseEntity(int collider, int x, int y);
		~BaseEntity();
		int getCollider();
		int* getPosition();
		int getPositionX();
		int getPositionY();
		void setPosition(int x, int y);
		void setPositionX(int x);
		void setPositionY(int y);
		void setCollider(int newCollider);
		char getPrintChar();
};

