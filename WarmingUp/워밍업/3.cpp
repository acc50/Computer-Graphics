#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	char buf[200];				// ���ڿ� ����
	char numbuf[20];			// ��ĭ�̳� Ư�����ڸ� ������������ ���ڸ� ��� ����
	char numbuf2[2];			// ���� �ϳ��ϳ��� ��� ����
	
	int numstack[100];			// ���ڽ���
	int bottom = 0;				// ���ڽ����� �Ʒ�
	int top = 0;				// ���ڽ����� ��
	numbuf[0] = '\0';

	ifstream in("text.txt");
	int len = 0;					// ���ڿ� ����
	int count = 0;
	int word = 0;					// �ܾ� ����
	int number;

	if (!in.is_open()) {									// ������ ���� ���ϸ�  ����
		std::cout << "������ ã�� �� �����ϴ�" << endl;
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


		word += count + 1;				// ���� ������ count++,  count+1 ��ŭ �ܾ�� ����

		count = 0;

		for (int i = 0; i < len; i++) {
			if (buf[i] >= '0' && buf[i] <= '9') {		// ���ڶ��
				numbuf2[0] = buf[i];						// ���ڹ��ۿ� ���� ����
				numbuf2[1] = '\0';							// ���ڹ��ۿ� �ι��� �־���
				strcat(numbuf, numbuf2);
			}

			else if( !(buf[i] >= 'a' && buf[i] <= 'z') && !(buf[i] >= 'A' && buf[i] <= 'Z') && numbuf[0] != '\0')			// ���ڳ� Ư�����ڸ� ������
			{
				number = atoi(numbuf);				// number�� ���ڿ��� ���ڷ� ��ȯ�Ѱ��� ����
				numstack[top] = number;				// ������ ���� ���ڸ� ����
				top = top + 1;						// ������ ���� ��ĭ �ø�
				
				for (int i = 0; i < 20; i++) {		// ���ڹ��۸� ���
					numbuf[i] = '\0';
				}

			}
		}

	}

	in.close();

	std::cout << endl << endl << "word count:" << word - 1 << endl;			// ���������� �а� �ѹ� �� �� ���� �б⿡ word-1
	std::cout << "number: ";

	for (int i = 0; i < top; i++) {
		std::cout << numstack[i] << " ";
	}
	

	std::cout << "    total: " << top << endl;

	return 0;
}