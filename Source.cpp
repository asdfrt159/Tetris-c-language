#include "Header.h"

#define INTERVAL 1.0/60.0

int main() {
	srand(time(0));

	float prev = (float)clock() / CLOCKS_PER_SEC;
	// clock �̶� �Լ��� ���α׷� ���ۺ��� ���α׷��� Ŭ���� ��� ���ϰ� �ִ�.
	// CLOCKS_PER_SEC �� ���� 1000
	Display* display = new Display(); //���÷��� �Լ� ȣ��
	showConsoleCursor(false); //�ֿܼ� Ŀ�� ���ִ� ��
	GameEngine* gameEngine = new GameEngine();
	gameEngine->init();



	while (true) {
		float curr = (float)clock() / CLOCKS_PER_SEC;
		float dt = curr - prev;

		if (dt < INTERVAL) continue;
		prev = curr;

		bool left = keyState('a');
		bool right = keyState('d');
		bool down = keyState('s');
		bool rotate = keyState('w');

		if (left) {
			//�������� ��� �̵�
			gameEngine->next(dt, 'a');
		}
		else if (right) {
			gameEngine->next(dt, 'd');
		}
		else if (down) {
			gameEngine->next(dt, 's');
		}
		else if (rotate) {
			gameEngine->next(dt, 'w');
		}
		else {
			//�׳� ��� ��������
			gameEngine->next(dt, 0);
		}
		

		//ȭ�� ���
		gameEngine->makeDisplayData();
		display->draw();
		//���� ���� �Ǻ�
		if (gameEngine->state == GameEngine::GameState::GAMEOVER) {
			printf("\n������ ����Ǿ����ϴ�.\n");
			break;
		}
	}

	return 0;
}