#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int Determinant(int n, int type, int matrix[][3])		// ���° ��, ��or��, ���
{
	int sum = 0;

	if (type == 1) {			// ��

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

	//else if (type == 2) {		// ��

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

	int ematrix[4][4] = { 0, };				// Ȯ�����

	srand((unsigned int)time(NULL));

	for (int i = 0; i < 3; i++) {			// ��� �����
		for (int j = 0; j < 3; j++) {
			matrix1[i][j] = rand() % 2;
			matrix2[i][j] = rand() % 2;
		}
	}


	for (int i = 0; i < 3; i++) {					// ��� ���
		for (int j = 0; j < 3; j++) {
			cout << matrix1[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;

	for (int i = 0; i < 3; i++) {				// ��� ���
		for (int j = 0; j < 3; j++) {
			cout << matrix2[i][j] << " ";
		}
		cout << endl;
	}

	char command;
	int loop = 1;
	int count = 0;

	for (int i = 0; i < 3; i++) {					// 3x3 ��� ��
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				mul_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}

	

	while (loop) {					// while ����

		cout << "��ɾ �Է��Ͻÿ�: ";
		cin >> command;

		switch (command) {		// ����ġ ����
		case 'M':		// ��
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

		case 'A':		// ����
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

		case 'D':		// ����
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

		case 'R':		// ��Ľ��� ��
		case 'r':

			cout << "��Ľ��ǰ�: " << Determinant(1, 1, matrix1) << endl;
			break;

		case 'H':		// 4x4 ��ķ� ��ȯ �� ��Ľ� �� ���
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

			cout << "��Ľ��ǰ�: " << Determinant(1, 1, matrix1) << endl;

			break;

		case 'Q':		// ����
		case 'q':
			loop = 0;
			break;

		default:
			cout << "���� ��ɾ��Դϴ�" << endl;
			break;
		}						// ����ġ ��

	}			// while ��

	return 0;
}