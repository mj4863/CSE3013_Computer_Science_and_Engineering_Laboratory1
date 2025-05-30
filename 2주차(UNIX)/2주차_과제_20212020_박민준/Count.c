#include "Header.h"

// Count() 함수는 숫자 N을 구성하는 값을 현재 자릿수 pos만큼 num_cnt 배열에 더해준다.
// 두 번째 인자 pos는 자릿수를 의미한다. (1의 자리, 10의자리, 100의 자리 등등)
void Count(int N, int pos) {
	// N이 0이 될 때까지 반복한다. N이 0이 됐다면 N을 구성하는 값을 전부 처리해줬다는 의미이므로 반복문이 종료된다.
	// 예를 들어, N=347이고 pos=10이면 3470~3479까지 3, 4, 7이 총 10번씩 나오기 때문에 
	// N을 구성하는 값이 전부 처리될 때까지 pos의 값을 num_cnt에 더해주면 된다.
	while (N > 0) { 
		num_cnt[N % 10] += pos;
		N /= 10;
	}
}