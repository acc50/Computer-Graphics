#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;
default_random_engine dre;
uniform_int_distribution<int> uid1( 0,400 ), uid2( 1, 30 );				// 0~400 ���� ������  1~30*10 ���� ��ǥ������ �Ÿ� 

																		// ���������� �ҿ����� ���� �ʿ���

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

bool IsPointInCircle(double x1, double y1, double x2, double y2, double radius)		// ���� ���� �Ÿ� �� 2=�� 1=��
{
	double X = x2 - x1;		// ���� �� ������ X �Ÿ�
	double Y = y2 - y1;		// ���� �� ������ Y �Ÿ�

	double length = (X * X) + (Y + Y);		// ���� �� ������ �Ÿ��� ����

	if (length > (radius * radius))		// ���� �������� �Ÿ� ������  ������ �������� Ŭ ��� �浹 X
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

	else if (type == 2) {							//  ���� �簢�� �浹
		double radius = (C->x2 - C->x1) / 2;
		double centerx = (C->x1 + C->x2) / 2;
		double centery = (C->y1 + C->y2) / 2;

		if (A->x1 - radius <= centerx &&			//	�簢���� ���� ��������ŭ Ȯ���� ��
			A->y1 - radius <= centery &&			//	Ȯ���� �簢�� �ȿ� ���� �߽��� ������ �浹
			A->x2 + radius >= centerx &&
			A->y2 + radius >= centery
			) 
		{
			cout << "Rectangle  & Circle collide!!" << endl;
		}

		else {										// �簢���� �𼭸��� ���� �浹 üũ

			if (IsPointInCircle(centerx, centery, A->x1, A->y1, radius)) {		// �簢���� �»�ܰ� ���� �浹
				cout << "Rectangle  & Circle collide!!" << endl;
			}

			else if (IsPointInCircle(centerx, centery, A->x2, A->y1, radius)) {		// �簢���� ���ܰ� ���� �浹
				cout << "Rectangle  & Circle collide!!" << endl;
			}

			else if (IsPointInCircle(centerx, centery, A->x1, A->y2, radius)) {		// �簢���� ���ϴܰ� ���� �浹
				cout << "Rectangle  & Circle collide!!" << endl;
			}

			else if (IsPointInCircle(centerx, centery, A->x2, A->y2, radius)) {		// �簢���� ���ϴܰ� ���� �浹
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

	if (type == 1) {				// �簢�� x �簢��

		int point = random1;			// point �� ������ �� ����
		A->x1 = point;			// A.x1 = point   A.y1 = point

		point = random1;
		A->y1 = point;

		while (1) {
			point = random2 * 10;			// point �� ������ �Ÿ��� ����		10~200

			A->x2 = A->x1 + point;			// A.x2 = A.x1 + �Ÿ�

			point = random2 * 10;
			A->y2 = A->y1 + point;

			if (A->x2 <= 500 && A->y2 <= 500) {
				break;
			}
		}

		point = random3;			// point �� ������ �� ����
		B->x1 = point;			// A.x1 = point   A.y1 = point

		point = random3;
		B->y1 = point;

		while (1) {
			point = random4 * 10;			// point �� ������ �Ÿ��� ����		10~200

			B->x2 = B->x1 + point;			// A.x2 = A.x1 + �Ÿ�

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

	else if (type == 2) {			// �簢�� x ��

		int point = random1;			// point �� ������ �� ����
		A->x1 = point;			// A.x1 = point   A.y1 = point

		point = random1;
		A->y1 = point;

		while (1) {
			point = random2 * 10;			// point �� ������ �Ÿ��� ����		10~200

			A->x2 = A->x1 + point;			// A.x2 = A.x1 + �Ÿ�

			point = random2 * 10;
			A->y2 = A->y1 + point;

			if (A->x2 <= 500 && A->y2 <= 500) {
				break;
			}
		}

		point = random3;			// point �� ������ �� ����
		C->x1 = point;			// A.x1 = point   A.y1 = point

		point = random3;
		C->y1 = point;

		while (1) {
			point = random4 * 10;			// point �� ������ �Ÿ��� ����		10~200

			C->x2 = C->x1 + point;			// A.x2 = A.x1 + �Ÿ�
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

		switch (command) {		// ����ġ ����

		case 'a':
		case 'A':

			if (type == 1) {
				if (B->x1 < 40) {
					cout << "�������� ������ �� �����ϴ�" << endl;
					break;
				}

				B->x1 -= 40;
				B->x2 -= 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			else {
				if (C->x1 < 40) {
					cout << "�������� ������ �� �����ϴ�" << endl;
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
					cout << "���������� ������ �� �����ϴ�" << endl;
					break;
				}

				B->x1 += 40;
				B->x2 += 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			else {
				if (C->x2 > 460) {
					cout << "���������� ������ �� �����ϴ�" << endl;
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
					cout << "�Ʒ������� ������ �� �����ϴ�" << endl;
					break;
				}

				B->y1 += 40;
				B->y2 += 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			else {
				if (C->y2 > 460) {
					cout << "�Ʒ������� ������ �� �����ϴ�" << endl;
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
					cout << "�������� ������ �� �����ϴ�" << endl;
					break;
				}

				B->y1 -= 40;
				B->y2 -= 40;

				printShape(type, A, B, C);
				CollideCrash(type, A, B, C);
			}

			else {
				if (C->y1 < 40) {
					cout << "�������� ������ �� �����ϴ�" << endl;
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
			cout << "���� ��ɾ��Դϴ�" << endl;
			break;

		}				// ����ġ ��

	}
}

int main()
{
	int loop = 1;													// ���� ���� �ݺ���

	int type = 0;

	while (loop) {													// 1 �Ǵ� 2 ��ɾ� �Է� ���ϸ� ��� �ݺ�
																	// 1 �Ǵ� 2 �Է½� ��������
		cout << "Input Shape Combination: ";

		cin >> type;

		if (type == 1 || type == 2) {
			loop = 0;
		}

		else {
			cout << "1 - ( rectangle x rectangle ) " << "2 - ( rectangle x circle ) " << endl;
		}
	}

	figure A, B, C;					// ����ü A B C ����
	A.x1 = 0, A.y1 = 0;
	B.x1 = 0, B.y1 = 0;
	C.x1 = 0, C.y1 = 0;



	//int point = uid1(dre);			// point �� ������ �� ����
	//A.x1 = point;			// A.x1 = point   A.y1 = point

	//point = uid1(dre);
	//A.y1 = point;

	//while (1) {
	//	point = uid2(dre) * 10;			// point �� ������ �Ÿ��� ����		10~200

	//	A.x2 = A.x1 + point;			// A.x2 = A.x1 + �Ÿ�

	//	point = uid2(dre) * 10;
	//	A.y2 = A.y1 + point;

	//	if (A.x2 <= 500 && A.y2 <= 500) {
	//		break;
	//	}
	//}

	// cout << A.x1 << A.y1 << A.x2 << A.y2;		 Ȯ�ο�


	if (type == 1) {					// rectangle x rectangle

		MakeFigure(type, &A, &B, &C);

		Command(type, &A, &B, &C);
	}

	else {								// rectangle x circle

		MakeFigure(type, &A, &B, &C);

		Command(type, &A, &B, &C);

		/*cout << "Shape 1: (" << A.x1 << ", " << A.y1 << ") (" << A.x2 << ", " << A.y2 << ") " << endl;		// Ȯ�ο�
		cout << "Shape 2: (" << C.x1 << ", " << C.y1 << ") (" << C.x2 << ", " << C.y2 << ") " << endl;*/
	}

}