#include <stdio.h>
#include <string.h>				// 문자열 처리 함수 모음

int main_205()
{
	/*
		strlen : 문자열 길이 반환 (null제외)
		strcpy : 첫번째에 두번째 문자열을 복사
		strncpy : 첫번째에 두번째 문자열을 원하는 갯수만큼만 복사
		strcat : 첫번째에 두번째 문자열을 이어붙이기
		strncat : 첫번째에 두번째 문자열을 원하는 갯수만큼만 이어붙이기
		strcmp : 둘이 같으면 0, 다르면 0 이외의 숫자
		strncmp : 원하는 갯수까지만 둘이 비교하고 같으면 0, 다르면 그외 숫자
	*/

	char str1[50] = "apple is good";
	char str2[50] = "berry is good";
	char str3[50];

	// 문자열 길이
	int len_str1 = strlen(str1);
	int len_str2 = strlen(str2);
	printf("str1의 길이 : %d, str2의 길이 : %d \n", len_str1, len_str2);

	// str3에 str1을 복사
	strcpy(str3, str1);

	// str2에 str1의 내용을 5칸만 복사
	strncpy(str2, str1, 5);

	printf("%s\n %s\n %s\n", str1, str2, str3);

	char str4[50] = "Michael ";
	char str5[50] = "Michael ";

	// str4에 문자열 붙이기
	strcat(str4, "Bolton");
	printf("%s\n", str4);

	// str5에 7개만 문자열 붙이기
	strncat(str5, "Jackson Five", 7);
	printf("%s\n", str5);

	// str4와 str5 비교하기 (일치하면 0, 다르면 1)
	int result = strcmp(str4, str5);
	printf("비교 결과 : %d\n", result);

	result = strncmp(str4, str5, 7);
	printf("비교 결과 : %d\n", result);

	return 0;
}