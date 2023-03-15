#include "pch.h"
#include <vector>

void main()
{
	int a = 0, b = 2;
	auto r = (a <=> b);

	printf("%d \n", r);
}
