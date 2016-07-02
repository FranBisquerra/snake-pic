#include <math.h>

#include "delay.h"
#include "LCDlib.h"
#include "LEDlib.h"
#include <time.h>
#include <stdlib.h>

#define DISPLACEMENT_X 2
#define DISPLACEMENT_Y 1

int ballX = 37;
int ballY = 23;
int ballAngle = 40;
int ballSpeed = 2;
int ballSize = 1;
int width = 80;
int height = 22;
int playerWidth = 5;
int matrixRows = 5;
int lives = 50;
int livesX = 68;

int volatile Wall [5][11] = {
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0},
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0},
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}
};

void moveBall(int playerX) {
    //remove last ball
    TermMove(ballY, ballX);
    UARTSendString("   ");

    // move ball
    ballX += sin(ballAngle) * ballSpeed;
    ballY += cos(ballAngle) * ballSpeed;

    // check for collision on top
    if (ballY == 1) {

        ballAngle -= 90;
    }


    // check for collision on bricks
    if (ballY < 12) {
        collision(ballY);
    }

    // check for collision on left
    if (ballX < ballSize) {

        ballAngle -= 90;
        onLED(1);
    }

    // check for collision on right
    if (ballX > width - ballSize) {

        ballAngle -= 90;
        onLED(2);
    }

    // check for collision on bottom
    if (ballY > height) {
        if (lives < 0) {
            ballSpeed = 0;
        } else {
            lives--;
            TermMove(23, livesX);
            UARTSendString("   ");
            livesX = livesX + 3;
            ballAngle -= 90;

        }
    }

    // check for collision on paddle
    if (ballX >= playerX && ballX <= playerX + playerWidth && ballY == height) {

        ballAngle += 90;
        onLED(4);
    }

    TermMove(ballY, ballX);

    LCDMoveFirstLine();

    char buffer1[20];
    sprintf(buffer1, "X:%i Y:%i B:%i", ballX, ballY, playerX);
    LCDPrint(buffer1);

    LCDMoveSecondLine();

    char buffer[20];
    sprintf(buffer, "%i, <%i", ballY, ballAngle);
    LCDPrint(buffer);

    UARTSendString("o");

    //
    //    int y;
    //    for (y = 0; y < 5; y++) {
    //        Delay5ms();
    //    }

}





char topBrick [10] = " _____ ";
char bottomBrick [10] = "|_____|";

char topWhite [10] = "       ";
char bottomWhite [10] = "       ";

void printwall() {
    int row = 0, col = 0;
    for (row = 0; row < matrixRows; row++) {
        for (col = 0; col < 11; col++) {

            if (Wall[row][col] > 0) {
                TermMove(row + row + 1, col * 7 + DISPLACEMENT_X);
                UARTSendString(topBrick);
                TermMove(row + row + 2, col * 7 + DISPLACEMENT_X);
                UARTSendString(bottomBrick);
            } else {
                TermMove(row + row + 1, col * 7 + DISPLACEMENT_X);
                UARTSendString(topWhite);
                TermMove(row + row + 2, col * 7 + DISPLACEMENT_X);
                UARTSendString(bottomWhite);
            }

        }
    }


}

int collision(int ballY) {

    switch (ballY) {
        case 11: checkRowCollision(4);
            onLED(5);
            break;
        case 9: checkRowCollision(3);
            break;
        case 7: checkRowCollision(2);
            break;
        case 5: checkRowCollision(1);
            break;
        case 3: checkRowCollision(0);
            break;
    }
}

void checkRowCollision(int j) {
    int i;
    //    if (Wall[j][11] < 11) {
    for (i = 0; i < 11; i++) { //para cada ladrillo de la linea
        if (Wall[j][i] > 0 && ballX >= i * 7 + DISPLACEMENT_X && ballX <= i * 7 + DISPLACEMENT_X + 7) {
            Wall[j][i] = Wall[j][i] - 1;
            //
            //                if (Wall[j][i] == 0) {
            //                    Wall[j][11] = Wall[j][11] + 1;
            //                }

            ballAngle -= 90;
        }
    }
    //    }
}

