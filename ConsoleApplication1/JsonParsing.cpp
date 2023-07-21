#include <cstdio>
#include <cstring>

int main()
{
    // 1. Json���� ���ڿ� �����
    //char json[] = "{\"name\":\"jackson\",\"age\":\"19\",\"hobbies\":[\"reading\",\"gaming\",\"hiking\"]}";
    //char json[] = "{\r\n    \"AI\":\"res\"\r\n}";
    //char json[] = "{\r\n    \"AI\": \"res\",\r\n    \"UI\": \"req\"\r\n}";
    char json[] = "{\r\n    \"AI\": \"res\",\r\n    \"UI\": \"req\",\r\n    \"list\": [\r\n        \"reading\",\r\n        \"hiking\",\r\n        \"gaming\"\r\n    ]\r\n}";

    printf("%lld\n", strlen(json));                     // NULL���ڸ� ������ ���ڰ���
    printf("%s\n", json);
    printf("%c\n", json[0]);
    printf("\n\n");

    // 2. Json���� \" �� ��ġ�� ��� ã��
    int str_len = strlen(json);
    int target_index[100] = { 0, };
    int* p_target_index = target_index;
    int num_of_target = 0;

    char* p = json;
    for (int i = 0; i < str_len; i++, p++) {
        if (*p == '\"') {
            *p_target_index = i;
            p_target_index++;
            num_of_target++;
        }
    }

    for (int i = 0; i < num_of_target; i++) {
        printf("%d, ", target_index[i]);
    }
    printf("\ntotal number = %d\n", num_of_target);


    // 3. Json���� ���ڿ� �Ľ��ϱ� (������ �޽����� ����� �Ǿ��ִ� ����)
    // Ű�� ã�� ���� �����´� (json�� ��űԾ��� �����Ǿ������� Ȱ��)
    // {\r\n    \"AI\": \"res\",\r\n    \"UI\": \"req\",\r\n    \"list\": [\r\n        \"reading\",\r\n        \"hiking\",\r\n        \"gaming\"\r\n    ]\r\n}
    // \"�� ��ġ(16��) : 7, 10, 13, 17, 25, 28, 31, 35, 43, 48, 62, 70, 82, 89, 101, 108,
    char AI[3] = { 0, };
    char res[4] = { 0, };
    char UI[3] = { 0, };
    char req[4] = { 0, };
    char list[5] = { 0, };
    char list_content[3][64] = { 0, };           // ���� �����Ҵ����� ����

    // AI
    char* target_position = json;
    target_position += (target_index[0] - 0 + 1);          // ù ����
    for (int i = 0; i < (target_index[1] - target_index[0] - 1); i++, target_position++) {
        AI[i] = *target_position;
    }

    // res
    target_position += (target_index[2] - target_index[1] + 1);             // ¡�˴ٸ�
    for (int i = 0; i < (target_index[3] - target_index[2] - 1); i++, target_position++) {
        res[i] = *target_position;
    }

    // UI
    target_position += (target_index[4] - target_index[3] + 1);             // ¡�˴ٸ�
    for (int i = 0; i < (target_index[5] - target_index[4] - 1); i++, target_position++) {
        UI[i] = *target_position;
    }

    // req
    target_position += (target_index[6] - target_index[5] + 1);             // ¡�˴ٸ�
    for (int i = 0; i < (target_index[7] - target_index[6] - 1); i++, target_position++) {
        req[i] = *target_position;
    }

    // list
    target_position += (target_index[8] - target_index[7] + 1);             // ¡�˴ٸ�
    for (int i = 0; i < (target_index[9] - target_index[8] - 1); i++, target_position++) {
        list[i] = *target_position;
    }

    // ��� �迭�� ������ �� ��� ���� �ʿ�
    // [ �� ] �� ��ġ�� ��´�
    p = json;
    int index_square_bracket[2] = { 0, };             // ���ȣ�� ���۰� �� ��ġ(�ε���)

    int k = 0;
    for (; k < str_len; k++, p++) {
        if (*p == '[') {
            index_square_bracket[0] = k;            // [ ��ġ
            break;
        }
    }

    for (; k < str_len; k++, p++) {
        if (*p == ']') {
            index_square_bracket[1] = k;            // ] ��ġ
            break;
        }
    }
    
    // ������ ���ڿ��� �Ľ��Ͽ� ���� �����Ѵ�
    char bracket_content[256] = { 0, };                 // ���ȣ�� �������� �����Ͽ� �� ��� content�� �ִ��� Ȯ���Ѵ�

    // ���ȣ ������ target_index�� target_position�� �̵���Ų��

    printf("%d, %d\n", index_square_bracket[0], index_square_bracket[1]);
    
    printf("%s\n", AI);
    printf("%s\n", res);
    printf("%s\n", UI);
    printf("%s\n", req);
    printf("%s\n", list);
    printf("%s\n", list_content[0]);
    printf("%s\n", list_content[1]);
    printf("%s\n", list_content[2]);


    return 0;
}
