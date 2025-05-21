#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

long long* num_cnt; // 각 숫자(0~9)들이 총 몇 번 나왔는지를 저장할 num_cnt 배열을 선언한다.
void Count(int N, int pos);
void PS(int a, int b, int pos);
