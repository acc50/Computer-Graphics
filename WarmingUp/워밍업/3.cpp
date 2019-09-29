#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	char buf[200];				// 문자열 버퍼
	char numbuf[20];			// 빈칸이나 특수문자를 만나기전까지 숫자를 담는 버퍼
	char numbuf2[2];			// 숫자 하나하나를 담는 버퍼
	
	int numstack[100];			// 숫자스택
	int bottom = 0;				// 숫자스택의 아래
	int top = 0;				// 숫자스택의 위
	numbuf[0] = '\0';

	ifstream in("text.txt");
	int len = 0;					// 문자열 길이
	int count = 0;
	int word = 0;					// 단어 갯수
	int number;

	if (!in.is_open()) {									// 파일을 열지 못하면  종료
		std::cout << "파일을 찾을 수 없습니다" << endl;
		return 0;
	}

	while (in) {
		in.getline(buf, 200);
		len = strlen(buf);
		
		std::cout << buf << endl;

		for (int i = 0; i < len; i++) {
			if (buf[i] == ' ' || buf[i] == '	') {
				count++;
			}
		}


		word += count + 1;				// 띄어쓰기 만나면 count++,  count+1 만큼 단어개수 더함

		count = 0;

		for (int i = 0; i < len; i++) {
			if (buf[i] >= '0' && buf[i] <= '9') {		// 숫자라면
				numbuf2[0] = buf[i];						// 숫자버퍼에 숫자 대입
				numbuf2[1] = '\0';							// 숫자버퍼에 널문자 넣어줌
				strcat(numbuf, numbuf2);
			}

			else if( !(buf[i] >= 'a' && buf[i] <= 'z') && !(buf[i] >= 'A' && buf[i] <= 'Z') && numbuf[0] != '\0')			// 문자나 특수문자를 만나면
			{
				number = atoi(numbuf);				// number에 문자열을 숫자로 변환한것을 넣음
				numstack[top] = number;				// 스택의 위에 숫자를 넣음
				top = top + 1;						// 스택의 위를 한칸 올림
				
				for (int i = 0; i < 20; i++) {		// 숫자버퍼를 비움
					numbuf[i] = '\0';
				}

			}
		}

	}

	in.close();

	std::cout << endl << endl << "word count:" << word - 1 << endl;			// 마지막줄을 읽고 한번 더 빈 줄을 읽기에 word-1
	std::cout << "number: ";

	for (int i = 0; i < top; i++) {
		std::cout << numstack[i] << " ";
	}
	

	std::cout << "    total: " << top << endl;

	return 0;
}