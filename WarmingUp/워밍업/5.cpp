#include <iostream>
#include <ctime>
#include <cstdlib>
#include <random>

using namespace std;

default_random_engine dre;
uniform_int_distribution<int> uid(0, 5);

typedef struct {
	int x;
	int y;
}Object;

void GameStart(int maze[][50], Object *Player)
{
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			maze[i][j] = 1;
		}
	}


	Player->x = 0;
	Player->y = 0;

	maze[Player->y][Player->x] = 0;
}

bool IsWall(Object* P, int x, int y)
{
	if (P->x + x < 0 || P->x + x >= 50 || P->y + y < 0 || P->y + y >= 50) {		// 가려는곳이 벽이면 true리턴
		return true;
	}

	return false;
}

bool IsReadyLoad(int maze[][50], Object* P, int x, int y)
{
	if (maze[P->y + y][P->x + x] == 0) {				// 가려는곳이 이미 길이면 true 리턴
		return true;
	}

	return false;
}

void Move1(int maze[][50], Object* P, int Ds[], int& top)
{
	int direction = uid(dre);

	switch (direction) {
	case 0:
		if (IsWall(P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;
		

		P->y -= 1;
		maze[P->y][P->x] = 0;

		break;

	case 1:
		if (IsWall(P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		P->y += 1;
		maze[P->y][P->x] = 0;
		break;

	case 2:
		if (IsWall(P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;

		P->x -= 1;
		maze[P->y][P->x] = 0;
		break;

	case 3:
		if (IsWall(P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		P->x += 1;
		maze[P->y][P->x] = 0;
		break;

	case 4:
		if (IsWall(P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		P->y += 1;
		maze[P->y][P->x] = 0;
		break;

	case 5:
		if (IsWall(P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		P->x += 1;
		maze[P->y][P->x] = 0;
		break;

	}
}

void Move2(int maze[][50], Object* P, int Ds[], int& top)
{
	int direction = uid(dre);

	switch (direction) {
	case 0:
		if (IsWall(P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;


		P->y -= 1;
		maze[P->y][P->x] = 0;

		break;

	case 1:
		if (IsWall(P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		P->y += 1;
		maze[P->y][P->x] = 0;
		break;

	case 2:
		if (IsWall(P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;

		P->x -= 1;
		maze[P->y][P->x] = 0;
		break;

	case 3:
		if (IsWall(P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		P->x += 1;
		maze[P->y][P->x] = 0;
		break;

	case 4:
		if (IsWall(P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		P->y += 1;
		maze[P->y][P->x] = 0;
		break;

	case 5:
		if (IsWall(P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;

		P->x -= 1;
		maze[P->y][P->x] = 0;
		break;
	}
}

void Move3(int maze[][50], Object* P, int Ds[], int& top)
{
	int direction = uid(dre);

	switch (direction) {
	case 0:
		if (IsWall(P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;


		P->y -= 1;
		maze[P->y][P->x] = 0;

		break;

	case 1:
		if (IsWall(P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		P->y += 1;
		maze[P->y][P->x] = 0;
		break;

	case 2:
		if (IsWall(P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;

		P->x -= 1;
		maze[P->y][P->x] = 0;
		break;

	case 3:
		if (IsWall(P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		P->x += 1;
		maze[P->y][P->x] = 0;
		break;

	case 4:
		if (IsWall(P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;


		P->y -= 1;
		maze[P->y][P->x] = 0;

		break;

	case 5:
		if (IsWall(P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		P->x += 1;
		maze[P->y][P->x] = 0;
		break;
	}
}

void Move4(int maze[][50], Object* P, int Ds[], int& top)
{
	int direction = uid(dre);

	switch (direction) {
	case 0:
		if (IsWall(P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 0, -1))
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;


		P->y -= 1;
		maze[P->y][P->x] = 0;

		break;

	case 1:
		if (IsWall(P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		P->y += 1;
		maze[P->y][P->x] = 0;
		break;

	case 2:
		if (IsWall(P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 0))
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, -1))			// 대각선 확인
			break;

		P->x -= 1;
		maze[P->y][P->x] = 0;
		break;

	case 3:
		if (IsWall(P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		P->x += 1;
		maze[P->y][P->x] = 0;
		break;

	case 4:
		if (IsWall(P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 0, 1))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, -1, 1))			// 대각선 확인
			break;

		P->y += 1;
		maze[P->y][P->x] = 0;
		break;

	case 5:
		if (IsWall(P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 0))
			break;

		if (IsReadyLoad(maze, P, 1, 1))			// 대각선 확인
			break;

		if (IsReadyLoad(maze, P, 1, -1))			// 대각선 확인
			break;

		P->x += 1;
		maze[P->y][P->x] = 0;
		break;
	}
}

int main()
{

	int maze[50][50];

	int Dstack[5];
	int top = 0;
	Object Player;

	char command;
	int loop = 1;

	int count = 0;

	while (loop) {
		command = cin.get();

		if (command == '\n') {
			GameStart(maze, &Player);

			while (1) {
				
				if (Player.x < 25 && Player.y < 25) {
					Move1(maze, &Player, Dstack, top);
				}

				else if (Player.x >= 25 && Player.y < 25) {
					Move2(maze, &Player, Dstack, top);
				}

				else if (Player.x < 25 && Player.y >= 25) {
					Move3(maze, &Player, Dstack, top);
				}

				else if (Player.x >= 25 && Player.y >= 25) {
					Move4(maze, &Player, Dstack, top);
				}

				if (Player.x == 49 && Player.y == 49) {
					break;
				}
				/*for (int i = 0; i < 50; i++) {
					for (int j = 0; j < 50; j++) {
						cout << maze[i][j] << " ";
					}
					cout << endl;
				}

				system("cls");*/

				count++;

				if (count % 30 == 0) {
					for (int i = 0; i < 50; i++) {
						for (int j = 0; j < 50; j++) {
							cout << maze[i][j] << " ";
						}
						cout << endl;
					}
				}
				

			}		// 엔터 반복 끝

			for (int i = 0; i < 50; i++) {
				for (int j = 0; j < 50; j++) {
					cout << maze[i][j] << " ";
				}
				cout << endl;
			}

			

		}

		else if (command == 'q') {
			loop = 0;
		}

	}

	return 0;
}