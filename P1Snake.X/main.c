/* 
 * File:   main.c
 * Author: Maria
 *
 * Created on June 24, 2016, 5:07 PM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <p30f4011.h>
#include "TERMlib.h"
#include "UARTlib.h"
#include "LEDlib.h"
#include "KEYBlib.h"
#include "LCDlib.h"
#include "CANlib.h"
#include "BUZZERlib.h"
#include "delay.h"
#include "libTIMER.h"
#include "TIMERlib.h"
#include <time.h>

/******************************************************************************/
/* Configuration words                                                        */
/******************************************************************************/
_FOSC(CSW_FSCM_OFF & EC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF);
_FGS(CODE_PROT_OFF);

/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/
int startGame = 0;

int screenHeight = 45;
int screenWidth = 95;

int snake [2][200]; //Limit snake length is 1000
int lastPositionsnake [2][1] = {0, 0, 0, 0};
int snakeSize = 3;

char direction = 'B';
char lastDirection = 'B';
char velocity = '2';

int crash = 0;
int points = 0;

unsigned int SID;


int isNormalFood = 0;
int normalFoodPosition [2][1] = {0, 0, 0, 0};
int timesNormalFood = 0;


int isSpecialFood = 0;
int specialFoodPosition [2][1] = {0, 0, 0, 0};
int timesSpecialFood = 0;

char players[100] = {0};
/******************************************************************************/
/* Procedures declaration                                                     */

/******************************************************************************/

void _ISR _C1Interrupt(void) {
    if (C1INTFbits.RX0IF == 1) {
        toggleLED(0);

        SID = C1RX0SIDbits.SID;

        if (SID == 1 && (C1RX0B1 & 0x00FF) == 'S' && startGame == 0) {
            initGame();
        } else if (SID == 2) {
            direction = C1RX0B1 & 0x00FF;
        } else if (SID == 3) {
            velocity = C1RX0B1 & 0x00FF;
        }

        // Clear rx buffer 0
        C1RX0CONbits.RXFUL = 0;

        // Clear interrupt flag associated to rx buffer 0
        C1INTFbits.RX0IF = 0;
        // Clear CAN reception buffer 0 interrupt
        IFS1bits.C1IF = 0;

    }
}

void _ISR _T1Interrupt(void) { //Special food
    TIMER1ClearInt();

    timesSpecialFood++;
}

void _ISR _T2Interrupt(void) { //Normal food
    TIMER2ClearInt();

    timesNormalFood++;
}

void initGame() {

    // Init variables
    startGame = 1;
    crash = 0;
    points = 0;
    timesNormalFood = 0;
    timesSpecialFood = 0;

    // Clear terminal
    Delay5ms();
    TermErase();

    initSnake();
    printBorder();
    //Activate timers
    TIMER1Start();
    TIMER2Start();
}

void initSnake() {

    // Init variables
    snakeSize = 3;
    direction = 'B';

    // Init matrix snake
    memset(snake, 0, sizeof (snake[0][0]) * 2 * 100); // clear array

    // Set first positions
    snake[0][0] = rand() % (screenWidth - 1);
    snake[1][0] = rand() % (screenHeight - 1);
}

void controlSnake() {

    controlCrash();
    controlFood();
    refreshSnake();

    switch (lastDirection) {

        case 'T':
            snake[1][0] = snake[1][0] - 1;
            break;
        case 'B':
            snake[1][0] = snake[1][0] + 1;
            break;
        case 'L':
            snake[0][0] = snake[0][0] - 1;
            break;
        case 'R':
            snake[0][0] = snake[0][0] + 1;
            break;
    }

    if (direction != lastDirection && lastDirection == 'T' && direction != 'B' && direction != 'S') {
        lastDirection = direction;
    } else if (direction != lastDirection && lastDirection == 'B' && direction != 'T' && direction != 'S') {
        lastDirection = direction;
    } else if (direction != lastDirection && lastDirection == 'L' && direction != 'R' && direction != 'S') {
        lastDirection = direction;
    } else if (direction != lastDirection && lastDirection == 'R' && direction != 'L' && direction != 'S') {
        lastDirection = direction;
    }
}

void controlCrash() {

    if (snake[0][0] < 2 || snake[1][0] < 2 || snake[0][0] > screenWidth - 1 || snake[1][0] > screenHeight - 1) {
        startGame = 0;
        crashed();
    }
    if (checkIfSnakeEatsItself() == 1) {
        startGame = 0;
        crashed();
    }
}

void crashed() {
    crash = 1;

    playNote(escalaNotas[3], 4, 250);
    playNote(escalaNotas[2], 4, 300);
    playNote(escalaNotas[1], 4, 350);

    TIMER1Reset();
    TIMER2Reset();

    int i;
    for (i = 0; i < snakeSize; i++) {
        TermMove(snake[1][i], snake[0][i]);
        UARTSendString(" ");
    }

    TermMove(specialFoodPosition[1][0], specialFoodPosition[0][0]);
    UARTSendString(" ");

    TermMove(normalFoodPosition[1][0], normalFoodPosition[0][0]);
    UARTSendString(" ");

    TermMove(screenHeight / 2 - 5, screenWidth / 2 - 3);
    TermPrintRed();
    UARTSendString("Game over");
    TermPrintDefault();

    TermMove(screenHeight / 2 - 3, screenWidth / 2 - 15);
    UARTSendString("Enter your name and press space:");
}

int checkIfSnakeEatsItself() {
    int i = 1;
    while (i < snakeSize) {
        if (snake[0][i] == snake[0][0] && snake[1][i] == snake[1][0]) {
            return 1;
        }
        i++;
    }
    return 0;
}

void controlFood() {

    if (isNormalFood == 1 && normalFoodPosition[0][0] == snake[0][0] && normalFoodPosition[1][0] == snake[1][0]) {

        isNormalFood = 0;
        timesNormalFood = 0;
        snakeSize = snakeSize + 1;

        lastPositionsnake[1][0] = -1; //The snake is eating
        points = points + ((int) velocity - 48);

        playNote(escalaNotas[5], 4, 250);
    }

    if (isSpecialFood == 1 && specialFoodPosition[0][0] == snake[0][0] && specialFoodPosition[1][0] == snake[1][0]) {

        isSpecialFood = 0;
        timesSpecialFood = 0;
        snakeSize = snakeSize + 1;

        lastPositionsnake[1][0] = -1; //The snake is eating
        points = points + 3 * ((int) velocity - 48);

        playNote(escalaNotas[6], 4, 250);
    }
}

void refreshSnake() {

    int i;

    //Update last position snake
    lastPositionsnake[0][0] = snake[0][snakeSize - 1];
    lastPositionsnake[1][0] = snake[1][snakeSize - 1];

    //Update position snake
    for (i = snakeSize - 1; i >= 1; i--) {
        snake[0][i] = snake[0][i - 1];
        snake[1][i] = snake[1][i - 1];
    }

    //Delete last position snake
    TermMove(lastPositionsnake[1][0], lastPositionsnake[0][0]);
    UARTSendString(" ");
}

void controlVelocity() {
    int j;
    switch (velocity) {
        case '1': for (j = 0; j <= 80; j++) Delay5ms();
            onLED(1);
            break;
        case '2': for (j = 0; j <= 50; j++) Delay5ms();
            onLED(2);
            break;
        case '3': for (j = 0; j <= 40; j++) Delay5ms();
            onLED(4);
            break;
        case '4': for (j = 0; j <= 20; j++) Delay5ms();
            onLED(4);
            break;
    }

    if ((lastDirection == 'T' || lastDirection == 'B') && (direction == 'T' || direction == 'B')) {
        for (j = 0; j <= 5 * velocity; j++) Delay150us();
    }
}

void printBorder() {
    int i;

    TermPrintDefault();

    for (i = 1; i <= screenWidth; i++) {

        TermMove(1, i);
        UARTSendString("*");
        TermMove(screenHeight, i);
        UARTSendString("*");
    }

    for (i = 1; i < screenHeight; i++) {

        TermMove(i, 1);
        UARTSendString("*");
        TermMove(i, screenWidth);
        UARTSendString("*");
    }
}

void printStart() {
    TermMove(screenHeight / 2 - 5, screenWidth / 2 - 8);
    TermPrintCyan();
    UARTSendString("Pulsa la tecla 4 para empezar!");
    TermPrintDefault();
    Delay5ms();
}

void printSnake() {
    TermMove(snake[1][0], snake[0][0]);
    TermPrintGreen();
    UARTSendString("O");
    TermPrintDefault();
}

void printSpecialFood() {
    if (timesSpecialFood == 1) {
        //Initialize position food
        specialFoodPosition[0][0] = rand() % (screenWidth - 5) + 2;
        specialFoodPosition[1][0] = rand() % (screenHeight - 5) + 2;

        //Print Food
        TermMove(specialFoodPosition[1][0], specialFoodPosition[0][0]);
        TermPrintDefault();
        UARTSendString("S");

        isSpecialFood = 1;
        timesSpecialFood++;

    } else if (timesSpecialFood == 14) {
        //warm that time of food is short
        TermMove(specialFoodPosition[1][0], specialFoodPosition[0][0]);
        TermPrintCyan();
        UARTSendString("S");
        TermPrintDefault();

        timesSpecialFood++;
    } else if (timesSpecialFood > 20) {
        //Delete food
        timesSpecialFood = 0;
        TermMove(specialFoodPosition[1][0], specialFoodPosition[0][0]);
        UARTSendString(" ");
        isSpecialFood = 0;
    }
}

void printNormalFood() {
    if (timesNormalFood == 1) {
        //Initialize position food
        normalFoodPosition[0][0] = rand() % (screenWidth - 5) + 2;
        normalFoodPosition[1][0] = rand() % (screenHeight - 5) + 2;

        //Print Food
        TermMove(normalFoodPosition[1][0], normalFoodPosition[0][0]);
        TermPrintDefault();
        UARTSendString("B");

        isNormalFood = 1;
        timesNormalFood++;

    } else if (timesNormalFood == 22) {
        //warm that time of food is short
        TermMove(normalFoodPosition[1][0], normalFoodPosition[0][0]);
        TermPrintCyan();
        UARTSendString("B");
        TermPrintDefault();

    } else if (timesNormalFood > 30) {
        //Delete food
        timesNormalFood = 0;
        TermMove(normalFoodPosition[1][0], normalFoodPosition[0][0]);
        UARTSendString(" ");
        isNormalFood = 0;
    }
}

void printCounter() {
    char buffer[15];
    TermPrintYellow();
    sprintf(buffer, "Score: %i  ", points);
    TermMove(screenHeight + 1, screenWidth - 10);
    UARTSendString(buffer);
    TermPrintDefault();
}

//Save last plays

void printPlayers() {

    if (crash == 1) {
        unsigned int buffer[15] = {0};
        char auxPlayers[20] = {0};
        int i = 0;


        unsigned int word = UARTRecv();

        while ((char) word != 13) {
            strcat(buffer, &word);

            TermMove(screenHeight / 2 - 2, 32 + i);
            UARTSendString(&word);

            i++;
            word = UARTRecv();
        }

        sprintf(auxPlayers, "%s: %i \n\r*\t\t\t\t", buffer, points);
        strcat(players, auxPlayers);

        TermMove(screenHeight / 2 - 3, screenWidth / 2 - 15);
        UARTSendString("                                 ");

        TermMove(screenHeight / 2 - 2, screenWidth / 2 - 15);
        UARTSendString("                                 ");

        TermMove(screenHeight / 2 - 3, screenWidth / 2 - 14);
        UARTSendString("Last plays:");

        TermMove(screenHeight / 2 - 1, screenWidth / 2 - 14);
        UARTSendString(players);
    }
}

/******************************************************************************/
/* Main                                                                       */

/******************************************************************************/

int main(void) {

    LEDInit();
    KeyInit();
    BuzzerInit();
    LCDInit();
    CANInit(0);
    UARTInit();

    TermErase();

    // Generate seed
    time_t t;
    srand((unsigned) time(&t));

    //Initialize Timers
    TIMER1Init(0b11, 172500, 0, 1); //15 s
    TIMER2Init(0b11, 230000, 0, 1); //20 s


    while (1) {

        printPlayers();

        while (startGame == 0) {
            printStart();
        }

        controlVelocity();

        controlSnake();

        printSnake();
        printSpecialFood();
        printNormalFood();
        printCounter();
    }
    return 0;
}
