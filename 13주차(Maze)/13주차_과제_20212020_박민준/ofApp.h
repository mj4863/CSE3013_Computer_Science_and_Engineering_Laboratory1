/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/

#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon
#include <vector>
#include <stack>
#include <queue>
#include <stdlib.h>

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key); // Traps escape key if exit disabled
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	bool readFile();
	void freeMemory();
	bool DFS();
	bool BFS();
	void dfsdraw();
	void bfsdraw();

	int HEIGHT;//미로의 높이
	int WIDTH;//미로의 너비
	char** input;//텍스트 파일의 모든 정보를 담는 이차원 배열이다.
	vector<vector<char> > maze;		// file의 미로를 저장할 자료구조
	vector<pair<int, int> > dfsPath; // DFS 경로를 저장하는 배열

	vector<vector<bool> > visited;//방문여부를 저장할 2차원 배열

	vector<pair<int, int> > dfsFullPath; // DFS 전체 경로 저장
	vector<pair<int, int> > dfsShortestPath; // DFS 최단 경로 저장

	vector<pair<pair<int, int>, pair<int, int>>> bfsFullPath; // BFS 전체 경로 저장
	vector<pair<int, int> > bfsShortestPath; // BFS 최단 경로 저장

	int maze_col;//미로칸의 열의 인덱스를 가리킨다.
	int maze_row;//미로칸의 행의 인덱스를 가리킨다.
	int k;
	int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.
	int isDFS;//DFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
	int isBFS;//BFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
	// Menu
	ofxWinMenu* menu; // Menu object
	void appMenuFunction(string title, bool bChecked); // Menu return function

	// Used by example app
	ofTrueTypeFont myFont;
	ofImage myImage;
	float windowWidth, windowHeight;
	HWND hWnd; // Application window
	HWND hWndForeground; // current foreground window

	// Example menu variables
	bool bShowInfo;
	bool bFullscreen;
	bool bTopmost;
	bool isdfs;
	bool isbfs;
	// Example functions
	void doFullScreen(bool bFull);
	void doTopmost(bool bTop);

};
