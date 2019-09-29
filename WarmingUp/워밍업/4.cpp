#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;
default_random_engine dre;
uniform_int_distribution<int> uid1( 0,400 ), uid2( 1, 30 );				// 0~400 으로 시작점  1~30*10 으로 좌표사이의 거리 

																		// 랜덤생성이 불완전함 수정 필요함

typedef struct Figure {
	int x1;
	int y1;
	int x2;
	int y2;
}Rectangle, Circle, figure;

void printShape(int type, figure *A, figure *B, figure *C)
{
	if (type == 1) {
		cout << "Shape 1: (" << A->x1 << ", " << A->y1 << ") (" << A->x2 << ", " << A->y2 << ") " << endl;
		cout << "Shape 2: (" << B->x1 << ", " << B->y1 << ") (" << B->x2 << ", " << B->y2 << ") " << endl;
	}

	else {
		cout << "Shape 1: (" << A->x1 << ", " << A->y1 << ") (" << A->x2 << ", " << A->y2 << ") " << endl;
		cout << "Shape 2: (" << C->x1 << ", " << C->y1 << ") (" << C->x2 << ", " << C->y2 << ") " << endl;
	}
}

bool IsPointInCircle(double x1, double y1, double x2, double y2, double radius)		// 점과 원의 거리 비교 2=점 1=원
{
	double X = x2 - x1;		// 점과 원 사이의 X 거리
	double Y = y2 - y1;		// 점과 원 사이의 Y 거리

	double length = (X * X) + (Y + Y);		// 점과 원 사이의 거리의 제곱

	if (length > (radius * radius))		// 점과 원사이의 거리 제곱이  반지름 제곱보다 클 경우 충돌 X
	{
		return false;
	}

	return true;

}

void CollideCrash(int type, figure *A, figure *B, figure *C)
{
	if (type == 1) {

		if (A->x1 <= B->x2 &&
			A->y1 <= B->y2 &&
			A->x2 >= B->x1 &&
			A->y2 >= B->y1)
		{
			cout << "Rectangle 1 & Rectangle 2 collide!!" << endl;
		}
	}

	else if (type == 2) {							//  원과 사각형 충돌
		double radius = (C->x2 - C->x1) / 2;
		double centerx = (C->x1 + C->x2) / 2;
		double centery = (C->y1 + C->y2) / 2;

		if (A->x1 - radius <= centerx &&			//	사각형을 원의 반지름만큼 확장한 후
			A->y1 - radius <= centery &&			//	확장한 사각형 안에 원의 중심이 있으면 충돌
			A->x2 + radius >= centerx &&
			A->y2 + radius >= centery
			) 
		{
			cout << "Rectangle  & Circle collide!!" << endl;
		}

		else {										// 사각형의 모서리와 원의 충돌 체크

			if (IsPointInCircle(centerx, centery, A->x1, A->y1, radius)) {		// 사각형의 좌상단과 원의 충돌
				cout << "Rectangle  & Circle collide!!" << endl;
			}

			else if (IsPointInCircle(centerx, centery, A->x2, A->y1, radius)) {		// 사각형의 우상단과 원의 충돌
				cout << "Rectangle  & Circle collide!!" << endl;
			}

			else if (IsPointInCircle(centerx, centery, A->x1, A->y2, radius)) {		// 사각형의 좌하단과 원의 충돌
				cout << "Rectangle  & Circle collide!!" << endl;
			}

			else if (IsPointInCircle(centerx, centery, A->x2, A->y2, radius)) {		// 사각형의 우하단과 원의 충돌
				cout << "Rectangle  & Circle collide!!" << endl;
			}
		}

	}
}

void MakeFigure(int type, figure *A, figure *B, figure *C)
{

	srand((unsigned int)time(NULL));

	int random1 = rand() % 400;
	int random2 = rand() % 30 + 1;
	int random3 = rand() % 400;
	int random4 = rand() % 30 + 1;

	if (type == 1) {				// 사각형 x 사각형

		int point = random1;			// point 에 임의의 값 넣음
		A->x1 = point;			// A.x1 = point   A.y1 = point

		point = random1;
		A->y1 = point;

		while (1) {
			point = random2 * 10;			// point 에 임의의 거리를 넣음		10~200

			A->x2 = A->x1 + point;			// A.x2 = A.x1 + 거리

			point = random2 * 10;
			A->y2 = A->y1 + point;

			if (A->x2 <= 500 && A->y2 <= 500) {
				break;
			}
		}

		point = random3;			// point 에 임의의 값 넣음
		B->x1 = point;			// A.x1 = point   A.y1 = point

		point = random3;
		B->y1 = point;

		while (1) {
			point = random4 * 10;			// point 에 임의의 거리를 넣음		10~200

			B->x2 = B->x1 + point;			// A.x2 = A.x1 + 거리

			point = random4 * 10;
			B->y2 = B->y1 + point;

			if (B->x2 <= 500 && B->y2 <= 500) {
				break;
			}
		}

		printShape(type, A, B, C);
		CollideCrash(type, A, B, C);
		/*cout << "Shape 1: (" << A->x1 << ", " << A->y1 << ") (" << A->x2 << ", " << A->y2 << ") " << endl;
		cout << "Shape 2: (" << B->x1 << ", " << B->y1 << ") (" << B->x2 << ", " << B->y2 << ") " << endl;*/

	}

	else if (type == 2) {			// 사각형 x 원

		int point = random1;			// point 에 임의의 값 넣음
		A->x1 = point;			// A.x1 = point   A.y1 = point

		point = random1;
		A->y1 = point;

		while (1) {
			point = random2 * 10;			// point 에 임의의 거리를 넣음		10~200

			A->x2 = A->x1 + point;			// A.x2 = A.x1 + 거리

			point = random2 * 10;
			A->y2 = A->y1 + point;

			if (A->x2 <= 500 && A->y2 <= 500) {
				break;
			}
		}

		point = random3;			// point 에 임의의 값 넣음
		C->x1 = point;			// A.x1 = point   A.y1 = point

		point = random3;
		C->y1 = point;

		while (1) {
			point = random4 * 10;			// point 에 임의의 거리를 넣음		10~200

			C->x2 = C->x1 + point;			// A.x2 = A.x1 + 거리
			C->y2 = C->y1 + point;

			if (C->x2 <= 500 && C->y2 <= 500) {
				break;
			}
		}

		printShape(type, A, B, C);
		CollideCrash(type, A, B, C);
		/*cout << "Shape 1: (" << A->x1 << ", " << A->y1 << ") (" << A->x2 << ", " << A->y2 << ") " << endl;
		cout << "Shape 2: (" << C->x1 << ", " << C->y1 << ") (" << C->x2 << ", " << C->y2 << ") " << endl;*/
	}
}

void Command(int type, figure *A, figure *B, figure *C)
{
	char command;
	int loop2 = 1;

	while (loop2) {

		cout << "Command: ";
		cin >> command;

		switch (command) {		// 스위치 시작

		case 'a':
		case 'A':

			if (type == 1) {
				if (B->x1 < 40) {
					cout << "왼쪽으로 움직일 수 없습니다" << endl;
					break;
				}

				B->x1 -= 40;
				B->x2 -= 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			else {
				if (C->x1 < 40) {
					cout << "왼쪽으로 움직일 수 없습니다" << endl;
					break;
				}

				C->x1 -= 40;
				C->x2 -= 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			break;

		case 'd':
		case 'D':

			if (type == 1) {
				if (B->x2 > 460) {
					cout << "오른쪽으로 움직일 수 없습니다" << endl;
					break;
				}

				B->x1 += 40;
				B->x2 += 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			else {
				if (C->x2 > 460) {
					cout << "오른쪽으로 움직일 수 없습니다" << endl;
					break;
				}

				C->x1 += 40;
				C->x2 += 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			break;

		case 's':
		case 'S':

			if (type == 1) {
				if (B->y2 > 460) {
					cout << "아래쪽으로 움직일 수 없습니다" << endl;
					break;
				}

				B->y1 += 40;
				B->y2 += 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			else {
				if (C->y2 > 460) {
					cout << "아래쪽으로 움직일 수 없습니다" << endl;
					break;
				}

				C->y1 += 40;
				C->y2 += 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			break;

		case 'w':
		case 'W':

			if (type == 1) {
				if (B->y1 < 40) {
					cout << "위쪽으로 움직일 수 없습니다" << endl;
					break;
				}

				B->y1 -= 40;
				B->y2 -= 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			else {
				if (C->y1 < 40) {
					cout << "위쪽으로 움직일 수 없습니다" << endl;
					break;
				}

				C->y1 -= 40;
				C->y2 -= 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			break;

		case 'Q':
			loop2 = 0;
			break;

		default:
			cout << "없는 명령어입니다" << endl;
			break;

		}				// 스위치 끝

	}
}

int main()
{
	int loop = 1;													// 도형 선택 반복문

	int type = 0;

	while (loop) {													// 1 또는 2 명령어 입력 안하면 계속 반복
																	// 1 또는 2 입력시 빠져나옴
		cout << "Input Shape Combination: ";

		cin >> type;

		if (type == 1 || type == 2) {
			loop = 0;
		}

		else {
			cout << "1 - ( rectangle x rectangle ) " << "2 - ( rectangle x circle ) " << endl;
		}
	}

	figure A, B, C;					// 구조체 A B C 선언
	A.x1 = 0, A.y1 = 0;
	B.x1 = 0, B.y1 = 0;
	C.x1 = 0, C.y1 = 0;



	//int point = uid1(dre);			// point 에 임의의 값 넣음
	//A.x1 = point;			// A.x1 = point   A.y1 = point

	//point = uid1(dre);
	//A.y1 = point;

	//while (1) {
	//	point = uid2(dre) * 10;			// point 에 임의의 거리를 넣음		10~200

	//	A.x2 = A.x1 + point;			// A.x2 = A.x1 + 거리

	//	point = uid2(dre) * 10;
	//	A.y2 = A.y1 + point;

	//	if (A.x2 <= 500 && A.y2 <= 500) {
	//		break;
	//	}
	//}

	// cout << A.x1 << A.y1 << A.x2 << A.y2;		 확인용


	if (type == 1) {					// rectangle x rectangle

		MakeFigure(type, &A, &B, &C);

		Command(type, &A, &B, &C);
	}

	else {								// rectangle x circle

		MakeFigure(type, &A, &B, &C);

		Command(type, &A, &B, &C);

		/*cout << "Shape 1: (" << A.x1 << ", " << A.y1 << ") (" << A.x2 << ", " << A.y2 << ") " << endl;		// 확인용
		cout << "Shape 2: (" << C.x1 << ", " << C.y1 << ") (" << C.x2 << ", " << C.y2 << ") " << endl;*/
	}

}