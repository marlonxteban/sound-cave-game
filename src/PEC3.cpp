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

//GLOBAL
//SDL_Renderer* gMyRenderer = NULL;
SDL_Window* gWindow = NULL;
GameStatus gameStatus;
bool consoleDebug = false;

//GAME
Scene cave(SCENE_WIDTH, SCENE_HEIGHT);
Player player;
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
int monsterAttackChannel = 3;
int wallHitChannel = 2;
int endGameChannel = 1;


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
	if (!consoleDebug)
		return;
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
	std::cout << "Press SPACE to enter console visual mode" << std::endl;
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
		}
	}

	// init audio
	if ((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG) {
		printf("Failed Mix_Init: %s\n", Mix_GetError());
		return -1;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
		printf("Failed Mix_OpenAudio: %s\n", Mix_GetError());
	}
	Mix_Chunk* loadSound;
	loadSound = Mix_LoadWAV("Assets/sounds/snore.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/waterfall.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/playerSteps.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/monsterMove.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/monsterAttack.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/wallHit.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/win.ogg");
	audios.push_back(loadSound);
	loadSound = Mix_LoadWAV("Assets/sounds/lose.ogg");
	audios.push_back(loadSound);
	if (loadSound == NULL) {
		printf("Failed sound load: %s\n", Mix_GetError());
	}

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
					if (tecla == SDL_SCANCODE_SPACE)
					{
						consoleDebug = !consoleDebug;
						if (consoleDebug)
						{
							printTestingOutputs();
						}
						else
						{
							system("cls");
						}
					}
					if (tecla == SDL_SCANCODE_RETURN && gameStatus != GameStatus::OnProgress) return 0;
					if (gameStatus != GameStatus::OnProgress) break;
					if (tecla == SDL_SCANCODE_UP) {
						nextCell = cave.getForwardCell(player.getPositionX(), player.getPositionY(), player.getDirection());
						if (player.canMoveForward(&nextCell))
						{
							if (nextCell.getCollider() == exitCollider)
							{
								std::cout << "You win!!! Press ENTER to exit" << std::endl;
								gameStatus = GameStatus::Win;
								Mix_HaltChannel(-1);
								Mix_PlayChannel(endGameChannel, audios[6], 0);
								break;
							}
							if (nextCell.getCollider() == monster.getCollider())
							{
								Mix_HaltChannel(snoreChannel);
								playTemporalSound(monsterAttackChannel, 4);
								std::cout << "You LOSE!!! Press ENTER to exit" << std::endl;
								gameStatus = GameStatus::Lose;
								Mix_HaltChannel(-1);
								Mix_PlayChannel(endGameChannel, audios[7], 0);
								break;
							}
							cave.setCellCollider(player.getPositionX(), player.getPositionY(), 0);
							player.moveForward();
							playTemporalSound(playerStepsChannel, 2);
							cave.setCellCollider(player.getPositionX(), player.getPositionY(), player.getCollider());
							updatePlayerVector();
							printTestingOutputs();
						}
						else
						{
							playTemporalSound(wallHitChannel, 5);
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
								playTemporalSound(monsterAttackChannel, 4);
								std::cout << "You LOSE!!! Press ENTER to exit" << std::endl;
								gameStatus = GameStatus::Lose;
								Mix_HaltChannel(-1);
								Mix_PlayChannel(endGameChannel, audios[7], 0);
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
	}

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	Mix_CloseAudio();
	SDL_Quit();

	return 0;
}