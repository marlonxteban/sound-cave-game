#pragma once
class Cell
{
	private:
		int collider;
		int positionX;
		int positionY;
	public:
		Cell();
		Cell(int collider, int x, int y);
		~Cell();
		int getCollider();
		int* getPosition();
		void setPosition(int x, int y);
		void setCollider(int newCollider);
		char getPrintChar();
};
