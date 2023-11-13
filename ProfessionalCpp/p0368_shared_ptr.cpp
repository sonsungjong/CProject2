import "HeaderUnits.h";

// shared_ptr : 포인터 하나를 여러 객체나 코드에서 복제해서 사용할 때 (공유 소유권)
// unique_ptr : 복제가 안됨

void fileClose(FILE* a_file_ptr)
{
	if (a_file_ptr == nullptr) {
		return;
	}
	fclose(a_file_ptr);
	printf("File Closed.\n");
}

int main0368()
{
	auto my_shared_ptr = make_shared<int>();
	*my_shared_ptr = 2;
	printf("%d\n", *my_shared_ptr);

	FILE* file_ptr = fopen("data.txt", "w");
	shared_ptr<FILE> shared_file_ptr{ file_ptr, fileClose };
	if (shared_file_ptr == nullptr) {
		printf("Error opening file\n");
	}
	else {
		printf("File Opend\n");
		printf("shared_file_ptr : %x\n", shared_file_ptr);
	}


	return 0;
}