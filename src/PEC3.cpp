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

//Audio
vector<Mix_Chunk*> audios;
int snoreChannel = 7;
int exitChannel = 6;
int playerStepsChannel = 5;
int monsterStepsChannel = 4;

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

void updateSound(int channel, Sint16 angle, Uint8 distance)
{
	Mix_SetPosition(channel, angle, distance);
	int volume = (MIX_MAX_VOLUME * (MAX_DISTANCE - distance)) / MAX_DISTANCE;
	volume = std::max(0, std::min(volume, MIX_MAX_VOLUME));
	Mix_Volume(channel, volume);
}

void updatePlayerVector()
{
	enemyPositionDegrees = static_cast<Sint16>(round(player.getAngleToEnemy(&monster)));
	exitPositionDegrees = static_cast<Sint16>(round(player.getAngleToExit(exitPositionX, exitPositionY)));
	enemyDistance = static_cast<Uint8>(round(player.getDistanceToPosition(monster.getPositionX(), monster.getPositionY())));
	exitDistance = static_cast<Uint8>(round(player.getDistanceToPosition(exitPositionX, exitPositionY)));
	// play constant sounds
	updateSound(snoreChannel, enemyPositionDegrees, enemyDistance);
	updateSound(exitChannel, exitPositionDegrees, exitDistance);
}

void playTemporalSound(int channel, int soundIndex)
{
	Mix_PlayChannel(channel, audios[soundIndex], 0);
	// Disable events while sound is playing
	SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
	SDL_EventState(SDL_KEYUP, SDL_IGNORE);

	// Wait for sound to finish
	while (Mix_Playing(channel)) {
		SDL_Delay(100);
	}

	// Enable events again
	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
	SDL_EventState(SDL_KEYUP, SDL_ENABLE);

	// Flush events
	SDL_PumpEvents();
	SDL_FlushEvent(SDL_KEYDOWN);
	SDL_FlushEvent(SDL_KEYUP);
}

void initGame()
{
	cave.setCellsCollider("./Assets/cave.txt");
	cave.setCellCollider(exitPositionX, exitPositionY, exitCollider);
	player.setPosition(playerInitialX, playerInitialY);
	player.setDirection(initialDirection);
	monster.setPosition(monsterInitialX, monsterInitialY);
	updatePlayerVector();
	cave.setCellCollider(playerInitialX, playerInitialY, player.getCollider());
	cave.setCellCollider(monsterInitialX, monsterInitialY, monster.getCollider());
	gameStatus = GameStatus::OnProgress;
	// Audio
	Mix_PlayChannel(snoreChannel, audios[0], -1);
	Mix_PlayChannel(exitChannel, audios[1], -1);
	// Testing code
	printTestingOutputs();

}

bool init()
{

	//Initialization flag
	bool success = true;

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
			}
		}
	}

	// init audio
	//Mix_Init(MIX_INIT_OGG);
	if ((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG) {
		printf("Failed Mix_Init: %s\n", Mix_GetError());
		return -1;
	}
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
		printf("Failed Mix_OpenAudio: %s\n", Mix_GetError());
	}
	Mix_Chunk* loadSound;
	loadSound = Mix_LoadWAV("Assets/sounds/snore.ogg");
	if (loadSound == NULL) {
		printf("Failed sound load: %s\n", Mix_GetError());
	}
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/waterfall.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/playerSteps.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/monsterMove.ogg");
	audios.push_back(loadSound);

	// init game
	initGame();
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
							playTemporalSound(playerStepsChannel, 2);
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
							updatePlayerVector();
							updateSound(monsterStepsChannel, enemyPositionDegrees, enemyDistance);
							Mix_HaltChannel(snoreChannel);
							playTemporalSound(monsterStepsChannel, 3);

							if (monster.getPositionX() == player.getPositionX() && monster.getPositionY() == player.getPositionY())
							{
								std::cout << "You LOSE!!! Press ENTER to exit game" << std::endl;
								gameStatus = GameStatus::Lose;
								// TODO: stop all music, start lose song
								break;
							}
							Mix_PlayChannel(snoreChannel, audios[0], -1);

							cave.setCellCollider(monster.getPositionX(), monster.getPositionY(), monster.getCollider());
							Mix_SetPosition(snoreChannel, enemyPositionDegrees, enemyDistance);
							printTestingOutputs();
						}
					}
					if (tecla == SDL_SCANCODE_RIGHT) {
						cave.setCellCollider(player.getPosition()[0], player.getPosition()[1], 0);
						player.turnRight();
						cave.setCellCollider(player.getPosition()[0], player.getPosition()[1], player.getCollider());

						updatePlayerVector();
						printTestingOutputs();
					}
					if (tecla == SDL_SCANCODE_LEFT) {
						cave.setCellCollider(player.getPosition()[0], player.getPosition()[1], 0);
						player.turnLeft();
						cave.setCellCollider(player.getPosition()[0], player.getPosition()[1], player.getCollider());

						updatePlayerVector();
						printTestingOutputs();
					}
				break;

				case SDL_QUIT:
					exit = true;
      			break;
			}
		}
		SDL_RenderPresent( gMyRenderer);
	}



	//Free loaded image
	SDL_DestroyTexture(sBG);

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