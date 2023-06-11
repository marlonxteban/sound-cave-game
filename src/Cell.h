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
		bool getCollider();
		int* getPosition();
		void setPosition(int x, int y);
		void setCollider(int newCollider);
		char getPrintChar();
};
