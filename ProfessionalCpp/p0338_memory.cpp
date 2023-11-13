import "HeaderUnits.h";

int main0338()
{
	int* my_arr_ptr = new int[] {1, 2, 3, 4, 5};
	int* my_arr_zero = new int[5] {0};

	printf("%d ", *my_arr_ptr);
	printf("%d ", *(my_arr_ptr + 1));
	printf("%d ", *(my_arr_ptr + 2));
	printf("%d ", *(my_arr_ptr + 3));
	printf("%d ", *(my_arr_ptr + 4));

	printf("\n%d ", *my_arr_zero);
	printf("%d ", *(my_arr_zero + 1));
	printf("%d ", *(my_arr_zero + 2));
	printf("%d ", *(my_arr_zero + 3));
	printf("%d ", *(my_arr_zero + 4));

	delete[] my_arr_ptr;
	my_arr_ptr = NULL;
	delete[] my_arr_zero;
	my_arr_zero = NULL;

	printf("\nprogram finished\n");

	return 0;
}