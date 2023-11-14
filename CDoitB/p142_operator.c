#include <stdio.h>

int main142()
{
	int data1 = 5, data2 = 3;

	int result1 = data1 + data2;
	int result2 = data1 * data2;
	int result3 = data1 / data2;
	int result4 = data1 % data2;

	/*
	>
	<=
	==
	!=
	>=
	<

	1 (true)
	0 (false)
	
	!1   ==> 0
	!0   ==> 1
	!3   ==> 0
	!5   ==> 0
	!ture ===> false
	!false   ==> true


	*/

	printf("result : %d, %d, %d, %d\n", result1, result2, result3, result4);

	return 0;
}
