#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>
#include <string.h>

#define MAX_LINE_CHARS	1024
#define MAX_LINES		1024

char* utf8_to_ansii(char* utf8_char); // 함수 프로토타입

// flie_cvt 에서 쓰이는 배열인데 안에다 쓰면 지역변수 주제에 16k 이상 쓴다고 싫어함
char str_tmp[MAX_LINES][MAX_LINE_CHARS]; // 1MB 

void file_cvt(const char* file_path)
{
	FILE* fp = fopen(file_path, "r");

	if (fp == NULL) { // 해당 파일을 찾을 수 없음 -> csv파일이 있는곳에서 실행을 안한것임.
		puts("아 csv파일이 있는곳에 실행파일 옮겨서 실행하시라구요!!!");
		printf("아니면... 혹시 [%s] 라는 파일이 없진 않던가요..?\n\n", file_path);
		system("pause");
		exit(1);
	}

	int line = 0; // 파일이 몇 줄로 되어 있는지 카운트 하는용도
	memset(str_tmp, 0, sizeof(str_tmp)); // 배열 클리어

	// 파일의 끝까지 한 Line 씩 cvt하여 str_temp에 저장
	while (!feof(fp)) {
		
		fgets((str_tmp[line]), MAX_LINE_CHARS, fp); // 1024 는 1라인 최대 문자 수
		memcpy((str_tmp[line]), utf8_to_ansii((str_tmp[line])), MAX_LINE_CHARS);
		line++;
	}

	fclose(fp); // utf8로 인코딩 된 파일의 파일 포인터 닫음

	unlink(file_path); /* utf8로 인코딩 된 파일을 '삭제' */

	// 원본과 같은이름으로 파일을 만들어 ANSI로 인코딩된 파일로 저장함
	FILE* new_fp = fopen(file_path, "w");
	for (int i = 0; i < line; i++)
		fputs(str_tmp[i], new_fp);
	fclose(new_fp);

	if (!strcmp(file_path, "7호선.csv"))
		for (int x = 0; x < line; x++)
			printf("%s\n", str_tmp[x]);
}

// utf8을 ansi로 "한 줄" 인코딩 해줌
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

	// cvt한거 retrun
	return utf8_char;
}

int main()
{
	// 객기부리기
	system("mode con:cols=60 lines=5");
	system("color 0A");

	// 파일 목록 ( 파일과 실행파일이 같은 디렉토리에 있어야만 동작함. 안그럼 exit(1)만남)
	char* f_list[] = { "1지선.csv", "1호선.csv", "2지선.csv", "2호선.csv"
		, "3호선.csv", "4호선.csv", "5지선.csv", "5호선.csv", "6호선.csv"
		, "7호선.csv", "8호선.csv", "9호선.csv", "경의선.csv", "경춘선.csv"
		, "공항철도.csv", "분당선.csv", "인천1선.csv", "중앙선.csv", "환승정보.csv" };

	// 파일 갯수. 차피19개지만 그냥 간지나게 써봄
	int f_len = sizeof(f_list) / sizeof(f_list[0]); 

	// 모든파일을 수정함
	for(int i=0; i < f_len; i++)
		file_cvt(f_list[i]);

	printf("변환이.. 잘.. 됐겠죠..? 그럼.. data버전 업 될 때 만나요..\n\n");
	system("pause");
}