#include "Header.h"

int main() {
    int T; // 테스트 케이스의 개수
    scanf("%d", &T);

    while (T--) {
        int N; // 책의 페이지 수
        scanf("%d", &N);

        // 각 숫자(0~9)들이 총 몇 번 나왔는지를 저장할 num_cnt 배열을 동적 할당 해준다.
        num_cnt = (long long*)malloc(10 * sizeof(long long));

        PS(1, N, 1); // 1부터 N페이지까지 1의 자리부터 각 숫자(0~9)들이 총 몇 번 나왔는지를 구해준다.

        for (int i = 0; i < 10; ++i)
            printf("%lld ", num_cnt[i]); // for문을 돌며 0~9까지 num_cnt 배열의 값을 순서대로 한 줄에 출력한다.
        printf("\n"); // 다음 테스트 케이스를 위해 줄바꿈 해준다.
    }

    return 0;
}
