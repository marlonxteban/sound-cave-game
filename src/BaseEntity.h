#pragma once
class BaseEntity
{
	private:
		int collider;
		int positionX;
		int positionY;
	public:
		BaseEntity();
		BaseEntity(int collider, int x, int y);
		~BaseEntity();
		bool getCollider();
		int* getPosition();
		void setPosition(int x, int y);
		void setCollider(int newCollider);
		char getPrintChar();
};

