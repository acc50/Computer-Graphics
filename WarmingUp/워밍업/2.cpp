#include <iostream>
#include <string>
#include <cmath>

using namespace std;

typedef struct {
	int x;
	int y;
	int z;
	int dist;
	bool isempty;
}Data;

void PrintList(Data Stack[])
{
	for (int i = 9; i >= 0; i--) {
		cout << i << "  |  ";

		if (!Stack[i].isempty)
			cout << Stack[i].x << "  " << Stack[i].y << "  " << Stack[i].z;

		cout << endl;
	}
}

int main()
{
	Data Stack[10];

	for (int i = 0; i < 10; i++)
		Stack[i].isempty = true;

	int top = 0;
	int bottom = 0;
	int length = 0;
	double min = 65536;
	double max = 0;

	char str[20];
	char num[10];
	
	char command;
	int x, y, z;
	int loop = 1;
	
	while (loop) {

		cout << "명령어를 입력하시오: ";
		cin.getline(str,20);

		/*if (wscanf(L"%c %d %d %d", &command, &x, &y, &z) != 4) {
			x = 1;
			y = 1;
			z = 1;
		}*/

		if (str[1] != '\0') {
			sscanf(str, "%c %d %d %d", &command, &x, &y, &z);
		}

		else {
			command = str[0];
		}
		

		switch (command)
		{
		case '+':

			if (top == 10) {
				cout << "더이상 넣을 수 없습니다" << endl;
				break;
			}

			Stack[top].x = x;
			Stack[top].y = y;
			Stack[top].z = z;
			Stack[top].isempty = false;
			Stack[top].dist = sqrt((x * x) + (y * y) + (z * z));

			top++;

			PrintList(Stack);
			break;

		case '-':
			if (top == 0 || bottom == top) {
				cout << " 더이상 삭제할 수 없습니다 " << endl;
				break;
			}

			top--;
			Stack[top].isempty = true;

			PrintList(Stack);
			break;

		case 'e':
			
			if (bottom == 0 && !(Stack[bottom].isempty) && top == 10) {
				cout << "더이상 넣을 수 없습니다" << endl;
				break;
			}

			else if (bottom >= 0 && !(Stack[bottom].isempty) && top <10) {		// 위로 넣을공간이 있음
				for (int i = 9; i > 0; i--) {
					Stack[i].x = Stack[i - 1].x;
					Stack[i].y = Stack[i - 1].y;
					Stack[i].z = Stack[i - 1].z;
					Stack[i].dist = Stack[i - 1].dist;
					Stack[i].isempty = Stack[i - 1].isempty;
				}

				Stack[bottom].x = x;
				Stack[bottom].y = y;
				Stack[bottom].z = z;
				Stack[bottom].isempty = false;
				Stack[bottom].dist = sqrt((x * x) + (y * y) + (z * z));

				top++;
			}

			else if (bottom > 0 && top == 10) {				// 위는 가득찼지만 아래가 비어있음
				bottom--;
				Stack[bottom].x = x;
				Stack[bottom].y = y;
				Stack[bottom].z = z;
				Stack[bottom].isempty = false;
				Stack[bottom].dist = sqrt((x * x) + (y * y) + (z * z));

			}

			else if (bottom == 0 && Stack[bottom].isempty) {   // 리스트가 비었을때
				Stack[bottom].x = x;
				Stack[bottom].y = y;
				Stack[bottom].z = z;
				Stack[bottom].isempty = false;
				Stack[bottom].dist = sqrt((x * x) + (y * y) + (z * z));
				top++;
			}


			PrintList(Stack);
			break;

		case 'd':

			if (bottom == 9 || (bottom == 0 && Stack[bottom].isempty)) {
				cout << " 더이상 삭제할 수 없습니다 " << endl;
				break;
			}

			Stack[bottom].isempty = true;
			bottom++;

			PrintList(Stack);
			break;

		case 'l':

			for (int i = 0; i < 10; i++) {
				if (!Stack[i].isempty)
					length++;
			}

			cout << "length of list : "<< length << endl;
			length = 0;

			break;

		case 'c':

			for (int i = 0; i < 10; i++)
				Stack[i].isempty = true;

			top = 0;
			bottom = 0;

			PrintList(Stack);
			break;

		case 'm':

			for (int i = 0; i < 10; i++) {
				if (!Stack[i].isempty)
					if (Stack[i].dist > max)
						max = Stack[i].dist;
			}

			for (int i = 0; i < 10; i++) {
				if (!Stack[i].isempty)
					if (Stack[i].dist == max) 
						cout << "(" << Stack[i].x << ", " << Stack[i].y << ", " << Stack[i].z << ") " << endl;
			}

			max = 0;

			break;

		case 'n':

			for (int i = 0; i < 10; i++) {
				if (!Stack[i].isempty)
					if (Stack[i].dist < min)
						min = Stack[i].dist;
			}

			for (int i = 0; i < 10; i++) {
				if (!Stack[i].isempty)
					if (Stack[i].dist == min) 
						cout << "(" << Stack[i].x << ", " << Stack[i].y << ", " << Stack[i].z << ") " << endl;
			}

			min = 65536;

			break;

		case 'q':
			loop = 0;
			break;

		default:
			break;
		}

	}

	return 0;
}