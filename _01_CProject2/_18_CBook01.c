#include <stdio.h>

struct Person {
	char* name;
	unsigned int age;
	float height;
	float weight;
};

int main1801() {
	char* name1, *name2, *name3, *name4, *name5;
	int age1, age2, age3, age4, age5;
	float height1, height2, height3, height4, height5;
	float weight1, weight2, weight3, weight4, weight5;
	
	int age[5];
	float height[5];
	float weight[5];

	struct Person ��¯��;
	struct Person ��ö��;
	struct Person �迵��;
	
	��¯��.age = 21;
	��¯��.name = "��¯��";
	��¯��.weight = 77.7;
	��¯��.height = 175.7;

	��ö��.age = 21;
	��ö��.name = "��ö��";
	��ö��.weight = 80.0;
	��ö��.height = 180.1;
	
	�迵��.age = 21;
	�迵��.name = "�迵��";
	�迵��.weight = 60.0;
	�迵��.height = 167.6;

	return 0;
}
