#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Cell.h"

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
		void printScene();
};

