#pragma once
class Cell
{
	private:
		int collider;
	public:
		Cell();
		~Cell();
		bool getCollider();
		void setCollider(int newCollider);
		char getPrintChar();
};
