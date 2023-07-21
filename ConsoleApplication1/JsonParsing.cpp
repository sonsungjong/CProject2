#include <cstdio>
#include <cstring>

int main()
{
    // 1. Json형태 문자열 만들기
    //char json[] = "{\"name\":\"jackson\",\"age\":\"19\",\"hobbies\":[\"reading\",\"gaming\",\"hiking\"]}";
    //char json[] = "{\r\n    \"AI\":\"res\"\r\n}";
    //char json[] = "{\r\n    \"AI\": \"res\",\r\n    \"UI\": \"req\"\r\n}";
    char json[] = "{\r\n    \"AI\": \"res\",\r\n    \"UI\": \"req\",\r\n    \"list\": [\r\n        \"reading\",\r\n        \"hiking\",\r\n        \"gaming\"\r\n    ]\r\n}";

    printf("%lld\n", strlen(json));                     // NULL문자를 제외한 문자개수
    printf("%s\n", json);
    printf("%c\n", json[0]);
    printf("\n\n");

    // 2. Json에서 \" 의 위치를 모두 찾기
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


    // 3. Json형태 문자열 파싱하기 (사전에 메시지가 약속이 되어있다 가정)
    // 키를 찾아 값을 가져온다 (json은 통신규약이 고정되어있음을 활용)
    // {\r\n    \"AI\": \"res\",\r\n    \"UI\": \"req\",\r\n    \"list\": [\r\n        \"reading\",\r\n        \"hiking\",\r\n        \"gaming\"\r\n    ]\r\n}
    // \"의 위치(16개) : 7, 10, 13, 17, 25, 28, 31, 35, 43, 48, 62, 70, 82, 89, 101, 108,
    char AI[3] = { 0, };
    char res[4] = { 0, };
    char UI[3] = { 0, };
    char req[4] = { 0, };
    char list[5] = { 0, };
    char list_content[3][64] = { 0, };           // 추후 동적할당으로 변경

    // AI
    char* target_position = json;
    target_position += (target_index[0] - 0 + 1);          // 첫 시작
    for (int i = 0; i < (target_index[1] - target_index[0] - 1); i++, target_position++) {
        AI[i] = *target_position;
    }

    // res
    target_position += (target_index[2] - target_index[1] + 1);             // 징검다리
    for (int i = 0; i < (target_index[3] - target_index[2] - 1); i++, target_position++) {
        res[i] = *target_position;
    }

    // UI
    target_position += (target_index[4] - target_index[3] + 1);             // 징검다리
    for (int i = 0; i < (target_index[5] - target_index[4] - 1); i++, target_position++) {
        UI[i] = *target_position;
    }

    // req
    target_position += (target_index[6] - target_index[5] + 1);             // 징검다리
    for (int i = 0; i < (target_index[7] - target_index[6] - 1); i++, target_position++) {
        req[i] = *target_position;
    }

    // list
    target_position += (target_index[8] - target_index[7] + 1);             // 징검다리
    for (int i = 0; i < (target_index[9] - target_index[8] - 1); i++, target_position++) {
        list[i] = *target_position;
    }

    // 몇개의 배열이 들어올지 모를 경우 수정 필요
    // [ 와 ] 의 위치를 얻는다
    p = json;
    int index_square_bracket[2] = { 0, };             // 대괄호의 시작과 끝 위치(인덱스)

    int k = 0;
    for (; k < str_len; k++, p++) {
        if (*p == '[') {
            index_square_bracket[0] = k;            // [ 위치
            break;
        }
    }

    for (; k < str_len; k++, p++) {
        if (*p == ']') {
            index_square_bracket[1] = k;            // ] 위치
            break;
        }
    }
    
    // 추출한 문자열로 파싱하여 값을 대입한다
    char bracket_content[256] = { 0, };                 // 대괄호를 기준으로 추출하여 총 몇개의 content가 있는지 확인한다

    // 대괄호 이후의 target_index로 target_position을 이동시킨다

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
