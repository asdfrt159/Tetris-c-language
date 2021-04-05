#include "Header.h"

#define INTERVAL 1.0/60.0

int main() {
	srand(time(0));

	float prev = (float)clock() / CLOCKS_PER_SEC;
	// clock 이란 함수가 프로그램 시작부터 프로그램의 클럭을 계속 구하고 있다.
	// CLOCKS_PER_SEC 은 보통 1000
	Display* display = new Display(); //디스플레이 함수 호출
	showConsoleCursor(false); //콘솔에 커서 없애는 것
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
			//왼쪽으로 블록 이동
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
			//그냥 블록 떨어지게
			gameEngine->next(dt, 0);
		}
		

		//화면 출력
		gameEngine->makeDisplayData();
		display->draw();
		//게임 상태 판별
		if (gameEngine->state == GameEngine::GameState::GAMEOVER) {
			printf("\n게임이 종료되었습니다.\n");
			break;
		}
	}

	return 0;
}