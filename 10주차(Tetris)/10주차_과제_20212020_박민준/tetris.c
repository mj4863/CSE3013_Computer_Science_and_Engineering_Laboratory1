#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	createRankList();
	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_REC_PLAY: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	for (i = 0; i < BLOCK_NUM; i++)
		nextBlock[i] = rand() % 7;

	// 추천 시스템을 위한 루트 노드 초기화
	recRoot = (RecNode *)malloc(sizeof(RecNode));
	recRoot->level = 0;
    recRoot->accumulatedScore = 0;
    memcpy(recRoot->recField, field, sizeof(field));
	recRoot->curBlockID = nextBlock[0];

	modified_recommend(recRoot); // 추천 함수 호출
	
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	for( i = 0; i < 4; i++ ){
		move(10+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();

	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    for (int i = 0; i < BLOCK_HEIGHT; i++) {
        for (int j = 0; j < BLOCK_WIDTH; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				if (f[blockY + i][blockX + j] == 1) return 0;
				if (blockY + i >= HEIGHT || blockX + j >= WIDTH || blockX + j < 0) return 0;
			}
        }
    }
    return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	int prevY = blockY, prevX = blockX, prevRotate = blockRotate;
	switch (command) {
        case KEY_UP:
            prevRotate = (prevRotate + 3) % 4;
            break;
        case KEY_DOWN:
            prevY--;
            break;
        case KEY_RIGHT:
            prevX--;
            break;
        case KEY_LEFT:
            prevX++;
            break;
        default:
            break;
    }

	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for(int i = 0; i < BLOCK_HEIGHT; i++){
		for(int j = 0; j < BLOCK_WIDTH; j++){
			if(block[currentBlock][prevRotate][i][j] == 1 && i + prevY >= 0){
				move(i + prevY + 1, j + prevX + 1);
				printw(".");
			}
		}
	}

	while(CheckToMove(f, currentBlock, prevRotate, prevY + 1, prevX)) {
		prevY++;
	}

	for(int i = 0; i < BLOCK_HEIGHT; i++){
		for(int j = 0; j < BLOCK_WIDTH; j++){
			if(block[currentBlock][prevRotate][i][j] == 1 && i + prevY >= 0){
				move(i + prevY + 1, j + prevX + 1);
				printw(".");
			}
		}
	}
	
	//3. 새로운 블록 정보를 그린다. 
    DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	move(HEIGHT, WIDTH + 10);
}

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.
	timed_out = 0;
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
		return;
    } 
	else {
		if (blockY == -1) gameOver = 1;
	}

	score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
	score += DeleteLine(field);
	PrintScore(score);

	// 다음 블록들 준비
	for (int i = 0; i < BLOCK_NUM - 1; i++)
		nextBlock[i] = nextBlock[i + 1];
	nextBlock[BLOCK_NUM - 1] = rand() % 7;
	
	// 추천 시스템을 위한 루트 노드 초기화
	recRoot->level = 0;
    recRoot->accumulatedScore = 0;
    memcpy(recRoot->recField, field, sizeof(field));
	recRoot->curBlockID = nextBlock[0];

	modified_recommend(recRoot); // 추천 함수 호출
	DrawNextBlock(nextBlock);
	DrawField();

	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH / 2 - 2;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int touched = 0;
	//Block이 추가된 영역의 필드값을 바꾼다.
    for (int i = 0; i < BLOCK_HEIGHT; i++) {
        for (int j = 0; j < BLOCK_WIDTH; j++)
            if(block[currentBlock][blockRotate][i][j] == 1) {
				f[blockY + i][blockX + j] = 1;
				if(blockY+i == HEIGHT - 1 || f[blockY + i + 1][blockX + j] == 1) 
					touched++;
			}
    }
	return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int full = 0, DeletedLine = 0;
	for(int i = 0; i < HEIGHT; i++) {
		full = 1;
		for(int j = 0; j < WIDTH; j++){
			if (f[i][j] == 0) {
				full = 0;
				break;
			}
		}
		if (full) {
			DeletedLine++;
			for(int k = i; k >= 1; k--)
				for(int t = 0; t < WIDTH; t++)
					f[k][t] = f[k - 1][t];
		}
	}

	return DeletedLine * DeletedLine * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate){
	// user code
	while(CheckToMove(field, blockID, blockRotate, y + 1, x)) {
		y++;
	}
	DrawBlock(y, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
	DrawRecommend(recommendY, recommendX, blockID, recommendR);
}

void createRankList(){
	// user code
	FILE *fp = fopen("rank.txt", "r");

	if (fp == NULL) return;
	fscanf(fp, "%d", &ranking_num);

	Node *current = head;

	for (int i = 0; i< ranking_num; i++) {
		Node *newNode = (Node *)malloc(sizeof(Node));
		fscanf(fp, "%s %d", newNode->name, &newNode->score);
		newNode->next = NULL;

		if (head == NULL)
			head = newNode;
		else
			current->next = newNode;
		current = newNode;
	}
	fclose(fp);
}

void rank(){
	// user code
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	char ch;
	ch = wgetch(stdscr);

	if (ch == '1') {
		int X = 1, Y = ranking_num;
		printw("X: ");
		echo();
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		noecho();
		printw("       name     |   score\n");
		printw("----------------------------\n");

		if (X > Y || Y > ranking_num || X > ranking_num || !ranking_num)
			printw("search failure: no rank in the list\n");
		else {
			Node *current = head;
			int count = 1;

			while (current != NULL && count <= Y) {
				if (count >= X) {
					printw("%s\t\t| %d\n", current->name, current->score);
				}
				current = current->next;
				count++;
			}
		}
	}
	else if (ch == '2') {
		char name[NAMELEN + 1];
		printw("input the name: ");
		echo();
		scanw("%s", name);
		noecho();
		printw("       name     |   score\n");
		printw("----------------------------\n");

		Node *current = head;
		int count = 0;

		while (current != NULL) {
			if (!strcmp(name, current->name)) {
				printw("%s\t\t| %d\n", current->name, current->score);
				count++;
			}
			current = current->next;
		}
		if (!count)
			printw("search failure: no name in the list\n");
	}
	else if (ch == '3') {
		int rank;
		printw("input the rank: ");
		echo();
		scanw("%d", &rank);
		noecho();

		if (rank > ranking_num || rank < 1 || head == NULL)
			printw("search failure: the rank not in the list\n");
		else {
			Node *current = head;
			Node *prev = NULL;
			int count = 1;

			while (current != NULL) {
				if (count == rank) {
					if (prev == NULL)
						head = head->next;
					else
						prev->next = current->next;
					break;
				}
				prev = current;
				current = current->next;
				count++;
			}
			printw("result: the rank deleted\n");
			ranking_num--;
			writeRankFile();
		}
	}
	getch();
}

void newRank(int score){
	// user code
	clear();
	char name[NAMELEN + 1];
	printw("your name: ");
	echo();
	scanw("%s", name);
	noecho();

	Node *newNode = (Node *)malloc(sizeof(Node));
	strcpy(newNode->name, name);
	newNode->score = score;
	newNode->next = NULL;

	if (head == NULL)
		head = newNode;
	else {
		Node *current = head;
		Node *prev = NULL;

		while (current != NULL && current->score > newNode->score) {
			prev = current;
			current = current->next;
		}
		if (prev == NULL) {
			newNode->next = head;
			head = newNode;
		}
		else {
			prev->next = newNode;
			newNode->next = current;
		}
	}
	ranking_num++;
	writeRankFile();
}

void writeRankFile(){
	// user code
	FILE *fp = fopen("rank.txt", "w");
	if (fp == NULL) return;

	fprintf(fp, "%d\n", ranking_num);

	Node *current = head;
	while (current != NULL) {
		fprintf(fp, "%s %d\n", current->name, current->score);
		current = current->next;
	}
	fclose(fp);
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(RecNode *root){	
	// user code
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
    int maxX = 0, maxY = 0, maxR = 0; // 최대 점수를 얻는 위치와 회전

	// 블록의 모든 회전과 위치에 대해 검사
    for (int r = 0; r < NUM_OF_ROTATE; r++) { 	// 블록의 회전 수 고려
        for (int x = -2; x < WIDTH; x++) { 		// 블록의 X 좌표 고려

			// y 좌표의 값을 초기화한 후, 이동 가능한 위치까지 y값을 증가시킴
            int y = -1; 
            while (CheckToMove(root->recField, root->curBlockID, r, y + 1, x)) y++;

            if (y == -1) continue; // 유효한 위치가 아니면 건너뜀

            RecNode *child = (RecNode *)malloc(sizeof(RecNode));
            child->level = root->level + 1;
            memcpy(child->recField, root->recField, sizeof(root->recField)); // child Node에 parent Node의 field 정보 복사
            child->accumulatedScore = root->accumulatedScore + DeleteLine(child->recField);
			child->accumulatedScore += AddBlockToField(child->recField, root->curBlockID, r, y, x);
            child->curBlockID = nextBlock[child->level];
			
			// child Node의 level이 2(VISIBLE_BLOCKS - 1) 미만인 경우, 재귀 호출을 통해 Tree 생성
            if (child->level < VISIBLE_BLOCKS - 1) {
                int score = recommend(child); // 재귀 호출을 통해 다음 블록 고려
                if (score > max) {
                    max = score;
                    maxX = x;
                    maxY = y;
                    maxR = r;
                }
            } 
			
			// 그렇지 않은 경우는 마지막 level에 해당함
			else {
                if (child->accumulatedScore > max) {
                    max = child->accumulatedScore;
                    maxX = x;
                    maxY = y;
                    maxR = r;
                }
            }
			free(child); // child 노드 메모리 해제
        }
    }
	
	// 루트 노드일 때 추천 좌표 설정
    if (root->level == 0) { 
        recommendX = maxX;
        recommendY = maxY;
        recommendR = maxR;
    }

    return max;
}

void updateFieldHeight(char recField[HEIGHT][WIDTH], int height[WIDTH]) {
    for (int x = 0; x < WIDTH; x++) {
        height[x] = 0;
        for (int y = 0; y < HEIGHT; y++) {
            if (recField[y][x] == 1) {
                height[x] = HEIGHT - y;
                break;
            }
        }
    }
}

int modified_recommend(RecNode *root) {
    int max = 0; // 최대 점수
    int maxX = 0, maxY = 0, maxR = 0; 
	recommendSpace += sizeof(*root);
	// 추천 시스템의 시작 시간 측정
    clock_t tree_start = clock();

    for (int r = 0; r < NUM_OF_ROTATE; r++) { 
        for (int x = -2; x < WIDTH; x++) { 

            int y = -1;
            while (CheckToMove(root->recField, root->curBlockID, r, y + 1, x)) y++;

            if (y == -1) continue; 

            RecNode *child = (RecNode *)malloc(sizeof(RecNode));
            child->level = root->level + 1;
            memcpy(child->recField, root->recField, sizeof(root->recField)); 
            child->accumulatedScore = root->accumulatedScore;
            child->accumulatedScore += AddBlockToField(child->recField, root->curBlockID, r, y, x);
            child->accumulatedScore += DeleteLine(child->recField);
            child->curBlockID = nextBlock[child->level];

            // 필드의 높이 정보만 저장 (데이터 단순화)
            int height[WIDTH] = {0};
            updateFieldHeight(child->recField, height);
            memcpy(child->recField, height, sizeof(height)); // 필드 높이 정보만 저장

            if (child->level < VISIBLE_BLOCKS - 1) {
                int score = modified_recommend(child);
                if (score > max) {
                    max = score;
                    maxX = x;
                    maxY = y;
                    maxR = r;
                }
            } 
            else {
                if (child->accumulatedScore > max) {
                    max = child->accumulatedScore;
                    maxX = x;
                    maxY = y;
                    maxR = r;
                }
            }
            free(child);
        }
    }

    if (root->level == 0) { 
        recommendX = maxX;
        recommendY = maxY;
        recommendR = maxR;
    }

	// 추천 시스템의 종료 시간 측정
    clock_t tree_end = clock();
    recommendTime += (double)(tree_end - tree_start) / CLOCKS_PER_SEC * 100;

    return max;
}

void recommendedPlay(){
	// user code
	start = time(NULL);
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();

	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}

		// 추천 블록 배치
		if (CheckToMove(field, nextBlock[0], recommendR, recommendY, recommendX)) {
			blockRotate = recommendR;
			blockY = recommendY;
			blockX = recommendX;

			DrawChange(field, ' ', nextBlock[0], blockRotate, blockY, blockX);
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
		end = time(NULL);
        duration = (double)difftime(end, start);

		move(3, WIDTH + 25);
        printw("time (t)                   : %.2lf   s", duration);
        move(4, WIDTH + 25);
        printw("score per time (score(t))  : %.2lf / s", score / duration);
		move(6, WIDTH + 25);
        printw("space                      : %.2lf Kb", (double)recommendSpace / 1024);
        move(7, WIDTH + 25);
        printw("space per time (space(t))  : %.2lf  Kb / s", (double)recommendSpace / duration / 1024);
		
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();

	// 시간 및 공간 효율성 계산 및 출력
    double timeEfficiency = (double)score / recommendTime;
    double spaceEfficiency = (double)score / (recommendSpace / duration / 1024);

    clear();
    printw("Play Time (t)                      : %.2lf s\n", duration);
    printw("Score                              : %d point\n", score);
    printw("Tree Construction Time (time(t))   : %.2lf s\n", recommendTime);
    printw("Tree Construction Space (space(t)) : %.2lf\n", (double)recommendSpace / duration / 1024);
    printw("Time Efficiency                    : %.2lf\n", timeEfficiency);
    printw("Space Efficiency                   : %.2lf\n", spaceEfficiency);
    refresh();
    getch();

	newRank(score);
}
