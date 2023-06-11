#include "Scene.h"

Scene::Scene(int width, int height) : width(width), height(height)
{
	// with x height matrix of cells
	cells = new Cell*[width];
	for (int i = 0; i < width; i++)
	{
		cells[i] = new Cell[height];
		//create a cell for each position
		for (int j = 0; j < height; j++)
		{
			cells[i][j] = Cell(false, i, j);
		}
	}
}

Scene::~Scene()
{
	for (int i = 0; i < width; i++)
	{
		delete[] cells[i];
	}
	delete[] cells;
}

int Scene::getWidth()
{
	return width;
}

int Scene::getHeight()
{
	return height;
}

Cell** Scene::getCells()
{
	return cells;
}

/// <summary>
/// Set colliders to cells from a file
/// </summary>
/// <param name="fileName">Path to file of colliders</param>
void Scene::setCellsCollider(const char* fileName)
{
	// read file
	std::ifstream file(fileName);
	if (!file) {
		std::cerr << "Can't open the file, check the file name.\n";
		return;
	}

	std::string line;
	// read each line
	for (int j = 0; j < height; ++j) {
		if (std::getline(file, line)) {
			std::istringstream iss(line);
			int collider;
			// read each collider 0 no collider, 1 collider, 2 exit
			for (int i = 0; i < width; ++i) {
				if (iss >> collider) {
					// set collider to cell
					setCellCollider(i, j, collider);
				}
				else {
					std::cerr << "Error at (" << i << ", " << j << ") line: " << line << "\n";
					return;
				}
			}
		}
		else {
			std::cerr << "Error at line: " << (j + 1) << ".\n";
			return;
		}
	}
}

void Scene::setCellCollider(int x, int y, int value)
{
	// set a cell in a position
	cells[x][y].setCollider(value);
}

Cell Scene::getCell(int x, int y)
{
	return cells[x][y];
}

void Scene::printScene()
{
	// print the matrix of cells
	for (int j = 0; j < height; j++)
	{
		std::cout << "|";
		for (int i = 0; i < width; i++)
		{
			std::cout << cells[i][j].getPosition()[0] << "," << cells[i][j].getPosition()[1] << " ";
			std::cout << cells[i][j].getPrintChar() << "|";
		}
		std::cout << std::endl;
	}
}
