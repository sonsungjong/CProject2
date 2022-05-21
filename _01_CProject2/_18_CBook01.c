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

	struct Person ½ÅÂ¯±¸;
	struct Person ±èÃ¶¼ö;
	struct Person ±è¿µÈñ;
	
	½ÅÂ¯±¸.age = 21;
	½ÅÂ¯±¸.name = "½ÅÂ¯±¸";
	½ÅÂ¯±¸.weight = 77.7;
	½ÅÂ¯±¸.height = 175.7;

	±èÃ¶¼ö.age = 21;
	±èÃ¶¼ö.name = "±èÃ¶¼ö";
	±èÃ¶¼ö.weight = 80.0;
	±èÃ¶¼ö.height = 180.1;
	
	±è¿µÈñ.age = 21;
	±è¿µÈñ.name = "±è¿µÈñ";
	±è¿µÈñ.weight = 60.0;
	±è¿µÈñ.height = 167.6;

	return 0;
}
