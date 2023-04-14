#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define rows 10
#define cols 10
#define numShips 10

void printBoard(int r, int c, char board[r][c]){
    for (int i = 0; i <= r; i++){
        for (int j = 0; j <= c; j++){
            if (i == 0 && j == 0){ //at 0, 0 prints a square
                printf("[ ]");
            } else if(i == 0){
                printf("%2d ", j); //prints 1-10 at top
            } else if (j == 0){
                printf("%2d ", i); //prints 1-10 at side
            }
            else{
            printf("%2c ", board[i-1][j-1]); // -1 since its been offset
            }
        }
        printf("\n");
    }
}

int isValidPlace(int x, int y, int size, int r, int c, char orient, char playBoard[r][c]){
    int i;
    if (orient == 'h' && y+size <= c && y >= 0 && x < r && x >= 0){
        for(i = 0; i < size; i++){
            if (playBoard[x+i][y] == 'x'){
                return(0);
                break;
            }
            else{
                return(1);
            }
        }
    }
    else if (orient == 'v' && y < c && y >= 0 && x+size <= r && x >= 0){
        for(i = 0; i < size; i++){
            if (playBoard[x][y+i] == 'x'){
                return(0);
                break;
            }
            else{
                return(1);
            }
        }
    } 
    return(0);
}

void placeShipVert(int x, int y, int size, int r, int c, char playBoard[r][c]){
    for (int i = 0; i < size; i++){ //Has to fit ship, depending on size
        playBoard[x+i][y] = 'x';
    }
    system("cls"); 
    printBoard(r, c, playBoard);
    }

void placeShipHoriz(int x, int y, int size, int r, int c, char playBoard[r][c]){
    for (int i = 0; i < size; i++){ //Has to fit ship, depending on size
        playBoard[x][y+i] = 'x';
    }
    system("cls"); 
    printBoard(r, c, playBoard);
}

void setBoard(int p, int num, int size, int r, int c, char playBoard[r][c]){
    int xPosit, yPosit, valid;
    char orientation; 
    while(1){
        while(1){
            printf("Player %d, Request where you would like\nThe top of the ship %d size %d to be (Row 1-10, Colum 1-10): ", p, num+1, size);
            if ((!scanf("%d", &xPosit)) || (!scanf("%d", &yPosit)))
                {
                    scanf("%*[^\n]");
                    printf("Please enter integers\n");
            }
            else{
                xPosit = xPosit - 1;
                yPosit = yPosit -1;
                break;
            }
        }
        printf("Would you like to place it vertically (v) or horizontally (h): ");
        scanf(" %c", &orientation);
        valid = isValidPlace(xPosit, yPosit, size, r, c, orientation, playBoard);
        if (valid == 1 && orientation == 'h'){
            placeShipHoriz(xPosit, yPosit, size, r, c, playBoard);
            break;
        } else if (valid == 1 && orientation == 'v'){
            placeShipVert(xPosit, yPosit, size, r, c, playBoard);
            break;
        }
        else{
            printf("Error: Please enter a correct value and\nSomething within bounds that does not overwrite another ship.\n");
        }
    }
}

int checkHit(int x, int y, int r, int c, char hidBoard[r][c], char playBoard[r][c]){
            hidBoard[x][y] = 'x';
            if (hidBoard[x][y] != playBoard[x][y]){
                hidBoard[x][y] = 'o';
                system("cls");
                printBoard(r, c, hidBoard);
                printf("\nMiss"); //if miss
                sleep(3);
                system("cls");
                return(0);
            } else {
                system("cls");
                printBoard(r, c, hidBoard);
                printf("\nHit");
                sleep(3);
                system("cls");
                return(1);
            }
}

int isValidMove(int x, int y, int r, int c, char hidBoard[r][c]){
    if (y < c && y >= 0 && x < r && x >= 0 && hidBoard[x][y] == '?'){
        return(1);
    }
    else{
        return(0);
    }
}

int playTurn(int play, int r, int c, int hits, char hidBoard[r][c], char playBoard[r][c]){
    int xPosit, yPosit, valid;
    printBoard(r, c, hidBoard);
    while(1){
        printf("\nPlayer %d select row and columns: ", play);
        if ((!scanf("%d", &xPosit)) || (!scanf("%d", &yPosit))) {
            scanf("%*[^\n]");
            printf("Please enter integers\n");
        }
        else{
            xPosit = xPosit - 1;
            yPosit = yPosit -1;
            valid = isValidMove(xPosit, yPosit, r, c, hidBoard);
            if (valid == 1){
            hits += checkHit(xPosit, yPosit, r, c, hidBoard, playBoard);
            return(hits);
            break;
            }
            else {
                printf("Enter a valid move\n");
            }
        }
    }
}

int checkWin(int hits, int shipTotal){
    if (hits == shipTotal){
        return(1);
    }
    else {
        return(0);
    }
}

int main(){ //10 by 10, with 10 ships.
    int ships[10] = {2,2,2,2,3,3,3,4,4,5}, player = 1, turns, i, j, win, totalShips = 0, hits1 = 0, hits2 = 0;
    char setUp[rows][cols], hiddenBoard1[rows][cols], hiddenBoard2[rows][cols], board1[rows][cols], board2[rows][cols], ready;
    //Create boards based on size of rows and cols, rather than being set
    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            setUp[i][j] = '?';
        }
    }
    memcpy(hiddenBoard1, setUp, sizeof(setUp)); //rather than re-do the for loops
    memcpy(hiddenBoard2, setUp, sizeof(setUp));
    memcpy(board1, setUp, sizeof(setUp));
    memcpy(board2, setUp, sizeof(setUp));
   
    for (i = 0; i < numShips; i++){
        totalShips += ships[i];
    }

    printBoard(rows, cols, board1);
    printf("Player %d fill in your board\n", player);

    for (i = 0; i < numShips; i++){ //Max ships is 10, but it can be reduced
        setBoard(player, i, ships[i], rows, cols, board1);
    }

    printf("Board Complete, Write this down\n");

    do{
        printf("When ready enter 'R': ");
        scanf(" %c", &ready);
    } while (ready != 'R'); 

    system("cls"); 
    player = 2;
    printBoard(rows, cols, board2);
    printf("Player %d fill in your board\n", player);

    for (i = 0; i < numShips; i++){
        setBoard(player, i, ships[i], rows, cols, board2);
    }

    printf("Board Complete, Write this down\n");

    do{
        printf("When ready enter 'R': ");
        scanf(" %c", &ready);
    } while (ready != 'R'); 

    system("cls");

    for (turns = 0; turns < (2*rows*cols+1); turns++){ 
        player = turns%2 ? 2 : 1;
        if (player == 1){
            hits1 = playTurn(player, rows, cols, hits1, hiddenBoard2, board2);
            win = checkWin(hits1, totalShips);
            if (win == 1){
                break;
            }
        }
        else {
            hits2 = playTurn(player, rows, cols, hits2, hiddenBoard1, board1);
            win = checkWin(hits2, totalShips);
            if (win == 1){
                break;
            }
        }
    }
    
    printf("Winner is Player: %d", player);
    sleep(3);
    return(0);
}
