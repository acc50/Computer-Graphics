#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int Determinant(int n, int type, int matrix[][3])		// 몇번째 줄, 행or열, 행렬
{
	int sum = 0;

	if (type == 1) {			// 행

		switch (n) {
		case 1:
			sum = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) +
				matrix[0][1] * (-1) * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
				matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
			break;

		case 2:
			sum = matrix[1][0] * (-1) * (matrix[0][1] * matrix[2][2] - matrix[0][2] * matrix[2][1]) +
				matrix[1][1] * (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) +
				matrix[1][2] * (-1) * (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
			break;

		case 3:
			sum = matrix[2][0] * (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) +
				matrix[2][1] * (-1) * (matrix[0][0] * matrix[1][2] - matrix[0][2] * matrix[1][0]) +
				matrix[2][2] * (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
			break;

		}
	}

	//else if (type == 2) {		// 열

	//	switch (n) {
	//	case 1:

	//		break;

	//	case 2:

	//		break;

	//	case 3:

	//		break;

	//	}

	//}
	//

	return sum;
}

int main()
{
	int matrix1[3][3] = { 0, };
	int matrix2[3][3] = { 0, };
	int mul_matrix[3][3] = { 0, };

	int ematrix[4][4] = { 0, };				// 확장행렬

	srand((unsigned int)time(NULL));

	for (int i = 0; i < 3; i++) {			// 행렬 만들기
		for (int j = 0; j < 3; j++) {
			matrix1[i][j] = rand() % 2;
			matrix2[i][j] = rand() % 2;
		}
	}


	for (int i = 0; i < 3; i++) {					// 행렬 출력
		for (int j = 0; j < 3; j++) {
			cout << matrix1[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;

	for (int i = 0; i < 3; i++) {				// 행렬 출력
		for (int j = 0; j < 3; j++) {
			cout << matrix2[i][j] << " ";
		}
		cout << endl;
	}

	char command;
	int loop = 1;
	int count = 0;

	for (int i = 0; i < 3; i++) {					// 3x3 행렬 곱
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				mul_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}

	

	while (loop) {					// while 시작

		cout << "명령어를 입력하시오: ";
		cin >> command;

		switch (command) {		// 스위치 시작
		case 'M':		// 곱
		case 'm':

			for (int i = 0; i < 3; i++) {

				for (int j = 0; j < 3; j++) {

					if (count == 0) {
						cout << matrix1[i][j] << " ";
					}

					else if (count == 1) {
						cout << matrix2[i][j] << " ";
					}

					else if (count == 2) {
						cout << mul_matrix[i][j] << " ";
					}

					if (j == 2 && count == 2) {
						count = 0;
					}

					else if (j == 2) {
						cout << "       ";
						count++;
						j = -1;
					}


				}

				cout << endl;
			}

			break;

		case 'A':		// 덧셈
		case 'a':

			for (int i = 0; i < 3; i++) {

				for (int j = 0; j < 3; j++) {

					if (count == 0) {
						cout << matrix1[i][j] << " ";
					}

					else if (count == 1) {
						cout << matrix2[i][j] << " ";
					}

					else if (count == 2) {
						cout << matrix1[i][j] + matrix2[i][j] << " ";
					}

					if (j == 2 && count == 2) {
						count = 0;
					}

					else if (j == 2) {
						cout << "       ";
						count++;
						j = -1;
					}


				}

				cout << endl;
			}

			break;

		case 'D':		// 뺄셈
		case 'd':

			for (int i = 0; i < 3; i++) {

				for (int j = 0; j < 3; j++) {

					if (count == 0) {
						cout << matrix1[i][j] << " ";
					}

					else if (count == 1) {
						cout << matrix2[i][j] << " ";
					}

					else if (count == 2) {
						cout << matrix1[i][j] - matrix2[i][j] << " ";
					}

					if (j == 2 && count == 2) {
						count = 0;
					}

					else if (j == 2) {
						cout << "       ";
						count++;
						j = -1;
					}


				}

				cout << endl;
			}

			break;

		case 'R':		// 행렬식의 값
		case 'r':

			cout << "행렬식의값: " << Determinant(1, 1, matrix1) << endl;
			break;

		case 'H':		// 4x4 행렬로 변환 후 행렬식 값 출력
		case 'h':

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					ematrix[i][j] = matrix1[i][j];
				}
			}
			ematrix[3][3] = 1;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					cout << ematrix[i][j] << " ";
				}
				cout << endl;
			}

			cout << "행렬식의값: " << Determinant(1, 1, matrix1) << endl;

			break;

		case 'Q':		// 종료
		case 'q':
			loop = 0;
			break;

		default:
			cout << "없는 명령어입니다" << endl;
			break;
		}						// 스위치 끝

	}			// while 끝

	return 0;
}