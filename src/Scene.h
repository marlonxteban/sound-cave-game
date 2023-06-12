#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Cell.h"
#include "Utils.h"

class Scene
{
	private:
		int width;
		int height;
		Cell** cells;
	public:
		Scene(int width, int height);
		~Scene();
		int getWidth();
		int getHeight();
		Cell** getCells();
		void setCellsCollider(const char* filenName);
		void setCellCollider(int x, int y, int value);
		Cell getCell(int x, int y);
		Cell getForwardCell(int x, int y, Direction direction);
		Cell* getAdjacentCells(int x, int y);
		void printScene();
};

