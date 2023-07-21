#include <cstdio>
#include <cstring>

int main()
{
    // 1. Json형태 문자열 만들기
    char json[] = "{\"name\":\"jackson\",\"age\":\"19\",\"hobbies\":[\"reading\",\"gaming\",\"hiking\"]}";

    printf("%lld\n", strlen(json));                     // NULL문자 제외
    printf("%s\n", json);
    printf("%c\n", json[0]);
    printf("%c\n", json[1]);
    printf("%c\n", json[2]);
    printf("\n\n");

    // 2. Json에서 \" 의 위치를 모두 찾기
    char* p = json;
    int str_len = strlen(json);
    int target_index[100] = { 0, };
    int* p_target_index = target_index;
    int num_of_target = 0;

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


    // 3. Json형태 문자열 파싱하기
    // 키를 찾아 값을 가져온다 (첫번째는 name이 오는것으로 정해져있고, 두번째는 age로 오는 것이 정해져있다.)
    // {\"name\":\"jackson\",\"age\":\"19\",\"hobbies\":[\"reading\",\"gaming\",\"hiking\"]}
    // 1, 6, 8, 16, 18, 22, 24, 27, 29, 37, 40, 48, 50, 57, 59, 66,
    char name[5] = { 0, };
    char jackson[8] = { 0, };
    char age[4] = { 0, };
    char _19[3] = { 0, };
    char hobby[8] = { 0, };
    char hobbies[3][64] = { 0, };

    // name
    char* target_position = json;
    target_position += (target_index[0] - 0 + 1);          // 첫 시작
    for (int i = 0; i < (target_index[1] - target_index[0] - 1); i++, target_position++) {
        name[i] = *target_position;
    }

    // jackson
    target_position += (target_index[2] - target_index[1] + 1);             // 징검다리
    for (int i = 0; i < (target_index[3] - target_index[2] - 1); i++, target_position++) {
        jackson[i] = *target_position;
    }

    // age
    target_position += (target_index[4] - target_index[3] + 1);             // 징검다리
    for (int i = 0; i < (target_index[5] - target_index[4] - 1); i++, target_position++){
        age[i] = *target_position;
    }

    // 19
    target_position += (target_index[6] - target_index[5] + 1);             // 징검다리
    for (int i = 0; i < (target_index[7] - target_index[6] - 1); i++, target_position++) {
        _19[i] = *target_position;
    }

    // hobbies
    target_position += (target_index[8] - target_index[7] + 1);             // 징검다리
    for (int i = 0; i < (target_index[9] - target_index[8] - 1); i++, target_position++) {
        hobby[i] = *target_position;
    }

    // [3] {reading, gaming, hiking}
    target_position += (target_index[10] - target_index[9] + 1);                // 징검다리
    for (int i = 0; i < (target_index[11] - target_index[10] - 1); i++, target_position++) {
        hobbies[0][i] = *target_position;
    }

    target_position += (target_index[12] - target_index[11] + 1);                // 징검다리
    for (int i = 0; i < (target_index[13] - target_index[12] - 1); i++, target_position++) {
        hobbies[1][i] = *target_position;
    }

    target_position += (target_index[14] - target_index[13] + 1);                // 징검다리
    for (int i = 0; i < (target_index[15] - target_index[14] - 1); i++, target_position++) {
        hobbies[2][i] = *target_position;
    }

    
    printf("%s\n", name);
    printf("%s\n", jackson);
    printf("%s\n", age);
    printf("%s\n", _19);
    printf("%s\n", hobby);
    printf("%s\n", hobbies[0]);
    printf("%s\n", hobbies[1]);
    printf("%s\n", hobbies[2]);

    return 0;
}
