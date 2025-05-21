#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int W, H; // 미로의 너비, 높이
vector<vector<int> > set; 
vector<vector<char> > maze;
string filename = "maze.maz";

void InitMaze() {
    // 초기 미로 설정
    maze.resize(H * 2 + 1, vector<char>(W * 2 + 1, ' '));
    for (int i = 0; i <= H * 2; i++) {
        for (int j = 0; j <= W * 2; j++) {
            if (i % 2 == 0) {
                maze[i][j] = (j % 2 == 0) ? '+' : '-';
            }
            else {
                maze[i][j] = (j % 2 == 0) ? '|' : ' ';
            }
        }
    }

    // 초기 집합 번호 설정
    set.resize(H, vector<int>(W, 0));
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            set[i][j] = i * W + j;
        }
    }
}

void MakeMaze() {
    for (int i = 0; i < H; i++) {
        // 세로 벽 제거
        for (int j = 0; j < W - 1; j++) {
            if ((i == H - 1 || rand() % 2) && set[i][j] != set[i][j + 1]) {
                int oldset = set[i][j + 1];
                int newset = set[i][j];
                for (int j = 0; j < W; j++) {
                    if (set[i][j] == oldset) {
                        set[i][j] = newset;
                    }
                }
                maze[i * 2 + 1][j * 2 + 2] = ' ';
            }
        }

        if (i == H - 1) break; // 마지막 행 처리 완료

        // 가로 벽 제거
        vector<bool> connected(H * W, false);
        for (int j = 0; j < W; j++) {
            if (rand() % 2 || !connected[set[i][j]]) {
                set[i + 1][j] = set[i][j];
                maze[i * 2 + 2][j * 2 + 1] = ' ';
                connected[set[i][j]] = true;
            }
        }

        // 각 집합이 적어도 한 번은 아래로 연결되도록 보장
        for (int j = 0; j < W; j++) {
            if (!connected[set[i][j]]) {
                set[i + 1][j] = set[i][j];
                maze[i * 2 + 2][j * 2 + 1] = ' ';
                connected[set[i][j]] = true;
            }
        }
    }
}

void OutputMaze() {
    ofstream of(filename);
    for (int i = 0; i <= H * 2; i++) {
        for (int j = 0; j <= W * 2; j++)
            of << maze[i][j];
        of << '\n';
    }
    of.close();
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    cin >> W >> H;

    srand(time(NULL));
    InitMaze();
    MakeMaze();
    OutputMaze();

    return 0;
}