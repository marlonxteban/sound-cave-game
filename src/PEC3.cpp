#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;
const int SCENE_WIDTH = 10;
const int SCENE_HEIGHT = 10;

#ifndef PI
#define PI 3.14159265
#endif

#define MAX_TIME_SHIP_CROSS 5000 // in milisecons

//GLOBAL
SDL_Renderer* gMyRenderer = NULL;
SDL_Window* gWindow = NULL;
GameStatus gameStatus;
//Background
SDL_Texture* sBG = NULL;
SDL_Rect	BG_rect = { 0,0,512,512 };
int	BG_x = 0;
int BG_y = 0;

//GAME
Scene cave(SCENE_WIDTH, SCENE_HEIGHT);
Player player;
//int playerInitialX = 6;
//int playerInitialY = 4;
int playerInitialX = 2;
int playerInitialY = 7;
Direction initialDirection = Direction::Right;
Enemy monster;
int monsterInitialX = 5;
int monsterInitialY = 4;
int exitPositionX = 8;
int exitPositionY = 8;
int exitCollider = 2;

Sint16 enemyPositionDegrees;
Sint16 exitPositionDegrees;
Uint8 enemyDistance;
Uint8 exitDistance;

void renderTexture(SDL_Texture* origin, SDL_Rect* _rect, int X, int Y, double angle = 0){
		SDL_Rect source,target;
		source.x = _rect->x;
		source.y = _rect->y;
		source.w = _rect->w;
		source.h = _rect->h;
		target.x = X;
		target.y = Y;
		target.w = _rect->w;
		target.h = _rect->h;
		SDL_RenderCopyEx(gMyRenderer,origin,&source, &target,angle,NULL,SDL_FLIP_NONE); 
}

SDL_Texture* loadTexture( std::string path)
{
	//The final texture image
	SDL_Texture*  finalTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		cout << "Unable to load image " << path.c_str() <<"! SDL_image Error: " << IMG_GetError() << endl;
	}
	else
	{
		//Convert surface to renderer format
		finalTexture = SDL_CreateTextureFromSurface(gMyRenderer,loadedSurface);
		if( finalTexture == NULL )
		{
			cout << "Unable to optimize image " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return finalTexture;
}

void printAngleToEnemy()
{
	std::cout << "************************************************" << std::endl;
	std::cout << "Angulo Enemigo: "<< enemyPositionDegrees << std::endl;
	std::cout << "************************************************" << std::endl;
}

void printAngleToExit()
{
	std::cout << "************************************************" << std::endl;
	std::cout << "Angulo Salida: "<< exitPositionDegrees << std::endl;
	std::cout << "************************************************" << std::endl;
}

void printPlayerDirection()
{
	std::cout << "************************************************" << std::endl;
	std::cout << "Direccion Jugador: "<< DirectionToString(player.getDirection()) << std::endl;
	std::cout << "************************************************" << std::endl;
}

void printDistanceToEnemy()
{
	std::cout << "************************************************" << std::endl;
	std::cout << "Distancia Enemigo: "<< static_cast<int>(enemyDistance) << std::endl;
	std::cout << "************************************************" << std::endl;
}

void printDistanceToExit()
{
	std::cout << "************************************************" << std::endl;
	std::cout << "Distancia Salida: "<< static_cast<int>(exitDistance) << std::endl;
	std::cout << "************************************************" << std::endl;
}

void printTestingOutputs()
{
	cave.printScene();
	printPlayerDirection();
	printAngleToEnemy();
	printAngleToExit();
	printDistanceToEnemy();
	printDistanceToExit();
}

void updatePlayerVector()
{
	enemyPositionDegrees = static_cast<Sint16>(round(player.getAngleToEnemy(&monster)));
	exitPositionDegrees = static_cast<Sint16>(round(player.getAngleToExit(exitPositionX, exitPositionY)));
	enemyDistance = static_cast<Uint8>(round(player.getDistanceToPosition(monster.getPositionX(), monster.getPositionY())));
	exitDistance = static_cast<Uint8>(round(player.getDistanceToPosition(exitPositionX, exitPositionY)));
}

bool init()
{
	//Initialization flag
	bool success = true;

	cave.setCellsCollider("./Assets/cave.txt");
	cave.setCellCollider(exitPositionX, exitPositionY, exitCollider);
	player.setPosition(playerInitialX, playerInitialY);
	player.setDirection(initialDirection);
	monster.setPosition(monsterInitialX, monsterInitialY);
	updatePlayerVector();
	cave.setCellCollider(playerInitialX, playerInitialY, player.getCollider());
	cave.setCellCollider(monsterInitialX, monsterInitialY, monster.getCollider());
	// Testing code
	printTestingOutputs();

	srand(time(0));

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("PEC 3 - Cueva de los condenados", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Initialize PNG loading
			IMG_Init(IMG_INIT_PNG);
			//Get window renderer
			gMyRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gMyRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetHint("SDL_HINT_RENDER_VSYNC", "1");
				//Sound audio active
				Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

				//Load PNG surface
				sBG = loadTexture("Assets/space/background.png");
			}
		}
	}
	gameStatus = GameStatus::OnProgress;
	return success;
}

int main( int argc, char* args[] )
{

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	//Mouse
	int	mouseX=0,mouseY=0;

	bool	exit = false;

	while(!exit){

		SDL_Event test_event;
		SDL_Scancode tecla;
		Cell nextCell;
		while (SDL_PollEvent(&test_event)) {
			if (gameStatus == GameStatus::Finished) return 0;
			switch (test_event.type) {
				case SDL_KEYDOWN:
					tecla = test_event.key.keysym.scancode;
					if (tecla == SDL_SCANCODE_ESCAPE){
						exit = true;
					}
					if (tecla == SDL_SCANCODE_RETURN && gameStatus != GameStatus::OnProgress) return 0;
					if (gameStatus != GameStatus::OnProgress) break;
					if (tecla == SDL_SCANCODE_UP) {
						nextCell = cave.getForwardCell(player.getPositionX(), player.getPositionY(), player.getDirection());
						if (player.canMoveForward(&nextCell))
						{
							if (nextCell.getCollider() == exitCollider)
							{
								std::cout << "You win!!! Press ENTER to exit game" << std::endl;
								gameStatus = GameStatus::Win;
								// TODO: stop all music, start win song
								break;
							}
							if (nextCell.getCollider() == monster.getCollider())
							{
								std::cout << "You LOSE!!! Press ENTER to exit game" << std::endl;
								gameStatus = GameStatus::Lose;
								// TODO: stop all music, start lose song
								break;
							}
							cave.setCellCollider(player.getPositionX(), player.getPositionY(), 0);
							player.moveForward();
							cave.setCellCollider(player.getPositionX(), player.getPositionY(), player.getCollider());
							updatePlayerVector();
							// Testing code
							printTestingOutputs();
						}
						else
						{
							// TODO implement enemy movement and collision sound, diferentiate wall and exit
							std::cout << "Yayay!!!";
							std::cout << std::endl << std::endl;
							Cell* adjacentCells;
							adjacentCells = cave.getAdjacentCells(monster.getPositionX(), monster.getPositionY());
							cave.setCellCollider(monster.getPositionX(), monster.getPositionY(), 0);
							monster.move(adjacentCells);
							enemyPositionDegrees = static_cast<Sint16>(round(player.getAngleToEnemy(&monster)));
							enemyDistance = static_cast<Uint8>(round(player.getDistanceToPosition(monster.getPositionX(), monster.getPositionY())));

							if (monster.getPositionX() == player.getPositionX() && monster.getPositionY() == player.getPositionY())
							{
								std::cout << "You LOSE!!! Press ENTER to exit game" << std::endl;
								gameStatus = GameStatus::Lose;
								// TODO: stop all music, start lose song
								break;
							}

							cave.setCellCollider(monster.getPositionX(), monster.getPositionY(), monster.getCollider());
							printTestingOutputs();
						}
					}
					if (tecla == SDL_SCANCODE_RIGHT) {
						cave.setCellCollider(player.getPosition()[0], player.getPosition()[1], 0);
						player.turnRight();
						cave.setCellCollider(player.getPosition()[0], player.getPosition()[1], player.getCollider());
						enemyPositionDegrees = static_cast<Sint16>(round(player.getAngleToEnemy(&monster)));
						exitPositionDegrees = static_cast<Sint16>(round(player.getAngleToExit(exitPositionX, exitPositionY)));
						printTestingOutputs();
					}
					if (tecla == SDL_SCANCODE_LEFT) {
						cave.setCellCollider(player.getPosition()[0], player.getPosition()[1], 0);
						player.turnLeft();
						cave.setCellCollider(player.getPosition()[0], player.getPosition()[1], player.getCollider());
						enemyPositionDegrees = static_cast<Sint16>(round(player.getAngleToEnemy(&monster)));
						exitPositionDegrees = static_cast<Sint16>(round(player.getAngleToExit(exitPositionX, exitPositionY)));
						printTestingOutputs();
					}
				break;

				case SDL_QUIT:
					exit = true;
      			break;
			}
		}
// UPDATE SOUNDS
		//timeNextExplosion -= global_deltaTime;
		//timeNextLaser -= global_deltaTime;
		//if (timeNextExplosion < 0) {
		//	timeNextExplosion = 2000 + (rand() % 5) * 1000;
		//	posExplosion = rand() % 360;
		//	distanceExplosion = rand() % 256;
		//	int Channel = Mix_PlayChannel(-1, audios[rand() % 2], 0);
		//	Mix_SetPosition(Channel, posExplosion, distanceExplosion);
		//	Boom_x = distanceExplosion * sin((posExplosion*PI) / 180.0) + 256;
		//	Boom_y = distanceExplosion * cos((posExplosion*PI) / 180.0) + 256;
		//	//cout << "Explosion Angle:" << posExplosion << " distance:" << distanceExplosion <<" Next in:" << timeNextExplosion << endl;
		//}
		//if (timeNextLaser < 0) {
		//	timeNextLaser = 100 + (rand() % 10) * 100;
		//	posLaser = rand() % 360;
		//	distanceLaser = rand() % 256;
		//	int Channel = Mix_PlayChannel(-1, audios[4 + rand() % 2], 0);
		//	Mix_SetPosition(Channel, posLaser, distanceLaser);
		//	Laser_x = distanceLaser * sin((posLaser*PI) / 180.0) + 256;
		//	Laser_y = distanceLaser * cos((posLaser*PI) / 180.0) + 256;
		//	//cout << "Laser Angle:" << posLaser << " distance:" << distanceLaser << " Next in:" << timeNextLaser << endl;
		//}

		//if (timeNextShip > 0) {
		//	timeNextShip -= global_deltaTime;
		//	if (timeNextShip < 0) {
		//		timeNextShip = 0;
		//		timeCrossing = MAX_TIME_SHIP_CROSS;
		//		shipChannel = Mix_PlayChannel(-1, audios[2+rand() % 2], 0);
		//	}
		//}
		//if (timeNextShip == 0) { // Starship is crossing
		//	timeCrossing -= global_deltaTime;
		//	if (timeCrossing > 0) {
		//		posShip = (255 * timeCrossing) / MAX_TIME_SHIP_CROSS;
		//		Uint8 leftShip;
		//		Uint8 rightShip;
		//		if (shipGoRight) {
		//			rightShip = 255 - posShip;
		//			leftShip = posShip;
		//		}
		//		else {
		//			rightShip = posShip;
		//			leftShip = 255 - posShip;
		//		}
		//		Mix_SetPanning(shipChannel, leftShip, rightShip);
		//		Ship_X = rightShip * 2;
		//		//cout << "Ship Pass:" << timeCrossing << " Left:" << (int)leftShip << " Right:" << (int)rightShip << endl;
		//	}
		//	if (timeCrossing < 0) {
		//		timeCrossing = 0;
		//		timeNextShip = 1000 + (rand() % 5) * 1000;
		//		shipGoRight = rand() % 2;
		//	}
		//}
//RENDER
		// Clear Background
		//Apply the PNG image
		/*renderTexture(sBG,&BG_rect,BG_x,BG_y);
		renderTexture(sBoom,&Boom_rect,Boom_x,Boom_y);
		renderTexture(sLaser, &Laser_rect, Laser_x, Laser_y);
		renderTexture(sShip, &Ship_rect, Ship_X, Ship_Y);*/
		//Update the surface
		SDL_RenderPresent( gMyRenderer);
	}



	//Free loaded image
	SDL_DestroyTexture(sBG);
	/*SDL_DestroyTexture(sBoom);
	SDL_DestroyTexture(sLaser);
	SDL_DestroyTexture(sShip);*/

	//Destroy window
	SDL_DestroyRenderer(gMyRenderer);
	gMyRenderer = NULL;
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();

	return 0;
}