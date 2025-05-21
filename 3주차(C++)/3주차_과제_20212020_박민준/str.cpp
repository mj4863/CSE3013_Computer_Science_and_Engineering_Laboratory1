#include "str.h"
#include <string.h>

Str::Str(int leng) { // int형 변수 한 개를 인수로 받는 Str Class의 생성자이다.
    len = leng; // Str Class의 private 변수 len에 int형 인수 leng을 저장한다.
    str = new char[leng+1]; // leng+1의 길이만큼 str 변수를 동적 할당한다.
}

Str::Str(char *neyong) { // 문자열 변수 한 개를 인수로 받는 Str Class의 생성자이다.
    len = strlen(neyong); // len 변수에 문자열 인수의 길이를 저장한다.
    str = new char[len+1]; // str 변수를 len+1만큼 동적 할당한다.
    strcpy(str, neyong); // str 변수에 문자열 인수를 복사해서 저장한다.
}

Str::~Str() { // Str Class의 소멸자이다.
    delete[] str; // str 변수의 동적 할당을 해제한다.
}

int Str::length(void) { // 문자열의 길이를 반환하는 함수이다.
    return len; // int형 변수 len의 값을 반환한다.
}

char* Str::contents(void) { // 문자열의 내용을 반환하는 함수이다.
    return str; // 문자열 변수 str의 값을 반환한다.
}

int Str::compare(class Str& a) {
    return strcmp(str, a.contents()); // 문자열 변수 str과 Str Class 변수 a의 내용을 비교한 값을 출력한다.
}

int Str::compare(char *a) {
    return strcmp(str, a); // 문자열 변수 str과 문자열 인수 a를 비교한 값을 출력한다.
}

void Str::operator=(char *a) {
    delete[] str; // 이전에 str에 동적 할당된 메모리를 해제한다.
    len = strlen(a); // len 변수에 문자열 인수 a의 길이를 저장한다.
    str = new char[len+1]; // str 변수를 len+1만큼 동적 할당한다.
    strcpy(str, a); // str 변수에 문자열 인수 a를 복사해서 저장한다.
}

void Str::operator=(class Str& a) {
    delete[] str; // 이전에 str에 동적 할당된 메모리를 해제한다.
    len = a.length(); // len 변수에 Str Class 변수 a의 길이를 저장한다.
    str = new char[len+1]; // str 변수를 len+1만큼 동적 할당한다.
    strcpy(str, a.contents()); // str 변수에 St Class 변수 a의 내용을 복사해서 저장한다.
}