#define _CRT_SECURE_NO_WARNINGS

#include "TetrisHelper.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstring>


#define GRID_WIDTH 7
#define GRID_HEIGHT 10
#define USERBLOCK_SIZE 3

int displayData[GRID_HEIGHT][GRID_WIDTH] = { 0, };

class Display {
public:
	void draw() {
		for (int i = 0; i < GRID_HEIGHT; i++) {
			for (int k = 0; k < GRID_WIDTH; k++) {
				if (displayData[i][k] == 0) {
					drawPosition(k, i, false);
				}
				else {
					drawPosition(k, i, TRUE);
				}
			}
		}
	}
};

class GameEngine {
public:

	enum class GameState {
		PLAYING, GAMEOVER
	};

	GameState state = GameState::PLAYING;

	int gameGridData[GRID_HEIGHT][GRID_WIDTH] = {0,};
	int userBlock[USERBLOCK_SIZE][USERBLOCK_SIZE] = {0,	};

	int userBlockVarious[3][USERBLOCK_SIZE][USERBLOCK_SIZE] = {
		{
			{0,1,0},
			{0,1,0},
			{0,1,0}
		},
		{
			{0,0,0},
			{0,1,1},
			{0,1,1}
		},
		{
			{0,0,0},
			{0,1,0},
			{1,1,0}
		}
	};

	int blockX = 0;
	int blockY = 0;
	float elapsed = 0.0f;
	float controlCheck = 0.0f;


	void init() {
		//���ʿ� ���� ������ �ʱ�ȭ �ϴ� ������ �þ� �մϴ�.
		makeUserBlock();
	}

	//next�� while �������� �Ź� �ҷ����� �Լ��� �� �� �Դϴ�.
	void next(float dt, char keyboardInput) {
		if (state == GameState::GAMEOVER) return;
		elapsed += dt;
		if (elapsed > 0.5f) {
			if (canGoDown()) {
				blockY++;
			}
			else {
				//userBlock�� gameGridData�� �����ϸ� �ȴ�.
				trans();
				if (gameOverDicision()) state = GameState::GAMEOVER;
			}
			elapsed -= 0.5f;
		}

		controlCheck += dt;

		if (keyboardInput == 'a' && canGoLeft() && controlCheck >0.2) {
			blockX--;
			controlCheck = 0.0f;
		}
		if (keyboardInput == 'd' && canGoRight() && controlCheck > 0.2) {
			blockX++;
			controlCheck = 0.0f;
		}
		if (keyboardInput == 's' && canGoDown() && controlCheck > 0.2) {
			blockY++;
			controlCheck = 0.0f;
		}
		if (keyboardInput == 'w' && canGo() && controlCheck > 0.2) {
			//ȸ��
			controlCheck = 0.0f;
		}
	}
	
	bool canGoDown() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (userBlock[i][k] == 1 && i + blockY + 1 >= GRID_HEIGHT) {
					return false;
				}
				if (userBlock[i][k] == 1 && gameGridData[i + blockY + 1][k + blockX] == 1) {
					return false;
				}
			}
		}
		return true;
	}

	bool canGoLeft() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (userBlock[i][k] == 1 && k + blockX - 1 < 0)
					return false; //����¥�� �ڵ�� ����� ��������
				if (userBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX - 1] == 1)
					return false;
			}
		}
		return true;
	}
	bool canGoRight() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (userBlock[i][k] == 1 && k + blockX + 1 > GRID_WIDTH -1 )
					return false; //����¥�� �ڵ�� ����� ��������
				if (userBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX + 1] == 1)
					return false;
			}
		}
		return true;
	}

	//������ ����á���� Ȯ���ϴ� �Լ�
	bool isLineFilled(int y) {
		for (int i = 0; i < GRID_WIDTH; i++) {
			if (gameGridData [y][i] == 0) return false;
		}
		return true;
	}
	
	void eraseLine(int y) {
		for (int i = 0; i < GRID_WIDTH; i++) {
			gameGridData[y][i] = 0;
		}
	}

	//y��ǥ�� �������� ��ĭ �� �Ʒ��� ����
	void drop(int y) {
		for (int i = y; i >= 0; i--) {
			for (int k = 0; k < GRID_WIDTH; k++) {
				gameGridData[i][k] = i-1 < 0 ? 0 :gameGridData[i - 1][k];
			}
		}
	}

	//�����ϴ� �Լ�
	void trans() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				//����ȭ����
				gameGridData[i + blockY][k + blockX] = userBlock[i][k] == 1 ? userBlock[i][k] : gameGridData[i+blockY][k+blockX];
			}
		}

		//�� ���� ���� �� �ִ��� Ȯ��
		for (int i = 0; i < GRID_HEIGHT; i++) {
			if (isLineFilled(i)) {
				eraseLine(i);
				drop(i);
			}
		}


		//���ο� ���� ����
		makeUserBlock();
	}

	bool gameOverDicision() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (userBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX] == 1)
					return true;
			}
		}
		return false;
	}



	void makeUserBlock() {
		blockX = GRID_WIDTH /2 - USERBLOCK_SIZE /2;
		blockY = 0;
		
		//������ ���ؼ� ���ο� ������ �����.

		int various = rand() % 3;
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				userBlock[i][k] = userBlockVarious[various][i][k];
			}
		}
	}

	void rotate() {
		//ȸ��

	}


	//���� ���� �����͸� ȭ�鿡 ����� �� �ִ� �����ͷ� �ٲ��ݴϴ�.
	void makeDisplayData() {
		for (int i = 0; i < GRID_HEIGHT; i++) {
			for (int k = 0; k < GRID_WIDTH; k++) {
				displayData[i][k] = gameGridData[i][k];
			}
		}

		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (i + blockY < 0 || i + blockY > GRID_HEIGHT) {
					//DO NOTHING
				}
				else if (k + blockX < 0 || k + blockX > GRID_WIDTH) {
					//DO NOTHING
				}
				else {
					//TODO
					//displayData[i + blockY][k + blockX] = userBlock[i][k] == 1 ? userBlock[i][k] : displayData[i+blockY][k+blockX];
					
					//���� ���� �ڵ� ������ �� �ڵ尡 ������ �� ����
					int _x = k + blockX;
					int _y = i + blockY;
					displayData[_y][_x] = userBlock[i][k] | displayData[_y][_x];
				}
			}
		}

	}
};