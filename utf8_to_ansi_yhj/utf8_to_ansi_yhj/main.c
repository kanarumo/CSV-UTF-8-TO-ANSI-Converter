#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>
#include <string.h>

#define MAX_LINE_CHARS	1024
#define MAX_LINES		1024

char* utf8_to_ansii(char* utf8_char); // �Լ� ������Ÿ��

// flie_cvt ���� ���̴� �迭�ε� �ȿ��� ���� �������� ������ 16k �̻� ���ٰ� �Ⱦ���
char str_tmp[MAX_LINES][MAX_LINE_CHARS]; // 1MB 

void file_cvt(const char* file_path)
{
	FILE* fp = fopen(file_path, "r");

	if (fp == NULL) { // �ش� ������ ã�� �� ���� -> csv������ �ִ°����� ������ ���Ѱ���.
		puts("�� csv������ �ִ°��� �������� �Űܼ� �����Ͻö󱸿�!!!");
		printf("�ƴϸ�... Ȥ�� [%s] ��� ������ ���� �ʴ�����..?\n\n", file_path);
		system("pause");
		exit(1);
	}

	int line = 0; // ������ �� �ٷ� �Ǿ� �ִ��� ī��Ʈ �ϴ¿뵵
	memset(str_tmp, 0, sizeof(str_tmp)); // �迭 Ŭ����

	// ������ ������ �� Line �� cvt�Ͽ� str_temp�� ����
	while (!feof(fp)) {
		
		fgets((str_tmp[line]), MAX_LINE_CHARS, fp); // 1024 �� 1���� �ִ� ���� ��
		memcpy((str_tmp[line]), utf8_to_ansii((str_tmp[line])), MAX_LINE_CHARS);
		line++;
	}

	fclose(fp); // utf8�� ���ڵ� �� ������ ���� ������ ����

	unlink(file_path); /* utf8�� ���ڵ� �� ������ '����' */

	// ������ �����̸����� ������ ����� ANSI�� ���ڵ��� ���Ϸ� ������
	FILE* new_fp = fopen(file_path, "w");
	for (int i = 0; i < line; i++)
		fputs(str_tmp[i], new_fp);
	fclose(new_fp);

	if (!strcmp(file_path, "7ȣ��.csv"))
		for (int x = 0; x < line; x++)
			printf("%s\n", str_tmp[x]);
}

// utf8�� ansi�� "�� ��" ���ڵ� ����
char* utf8_to_ansii(char* utf8_char)
{
	int		len = 0;
	WCHAR	temp[MAX_LINE_CHARS];

	//utf8 to unicode
	len = MultiByteToWideChar(CP_UTF8, 0, utf8_char, -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8_char, -1, temp, len);

	//unicode to ansi
	len = WideCharToMultiByte(CP_ACP, 0, temp, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, temp, -1, utf8_char, len, NULL, NULL);

	// cvt�Ѱ� retrun
	return utf8_char;
}

int main()
{
	// ����θ���
	system("mode con:cols=60 lines=5");
	system("color 0A");

	// ���� ��� ( ���ϰ� ���������� ���� ���丮�� �־�߸� ������. �ȱ׷� exit(1)����)
	char* f_list[] = { "1����.csv", "1ȣ��.csv", "2����.csv", "2ȣ��.csv"
		, "3ȣ��.csv", "4ȣ��.csv", "5����.csv", "5ȣ��.csv", "6ȣ��.csv"
		, "7ȣ��.csv", "8ȣ��.csv", "9ȣ��.csv", "���Ǽ�.csv", "���ἱ.csv"
		, "����ö��.csv", "�д缱.csv", "��õ1��.csv", "�߾Ӽ�.csv", "ȯ������.csv" };

	// ���� ����. ����19������ �׳� �������� �ẽ
	int f_len = sizeof(f_list) / sizeof(f_list[0]); 

	// ��������� ������
	for(int i=0; i < f_len; i++)
		file_cvt(f_list[i]);

	printf("��ȯ��.. ��.. �ư���..? �׷�.. data���� �� �� �� ������..\n\n");
	system("pause");
}