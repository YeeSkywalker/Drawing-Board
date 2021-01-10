// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Allocate memory for a drawing state and initialise it
state *newState() {
    // TO DO
    state *s = malloc(sizeof(state));
    s->x = 0;
    s->y = 0;
    s->tx = 0;
    s->ty = 0;
    s->tool = LINE;
    s->start = 0;
    s->data = 0;
    s->end = false;
    
    return s;
}

// Release all memory associated with the drawing state
void freeState(state *s) {
    // TO DO
    free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
    // TO DO
    char leftMost[3];
    int opcode, decimal = 0, i = 0, remainder;
    
    for (int a = 0; a < 3; a++ ) {
        leftMost[a] = b[a];
    }
    
    opcode = atoi(leftMost);
    
    while (opcode != 0) {
        remainder = opcode % 10;
        opcode /= 10;
        decimal += remainder * pow(2, i);
        ++i;
    }
    
    return decimal;
    
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
    //TO DO
    char rightMost[7];
    int operrand, decimal, i = 0, remainder;
    
    for (int c = 0; c < 7; c++ ) {
        rightMost[c] = b[2 + c];
    }
    
    if (rightmost[0] == 1) {
        decimal = -32;
    }
    else decimal = 0;
    
    operand = atoi(rightMost);
    
    while (operand != 0) {
        remainder = operand % 10;
        operand /= 10;
        decimal += remainder * pow(2, i);
        ++i;
    }
    
    return decimal;
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
    //TO DO
    int opcode = getOpcode(op), operand = getOperand(op);
    
    switch (opcode) {
        case DX:
            s->tx += operand;
            break;
            
        case DY:
            s->ty += operand;
            break;
        
        case TOOL:
            switch (operand) {
                case NONE:
                    s->tool = TOOL;
                    break;
                
                case LINE:
                    s->tool = LINE;
                    break;
                    
                default:
                    break;
            }
        default:
            break;
    }
    
    if (s->tool == LINE) {
        line(*d, s->x, s->y, s->tx, s->y);
    }
    
    s->x = s->tx;
    s->y = s->ty;
}

// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, void *data, const char pressedKey) {

    //TO DO: OPEN, PROCESS/DRAW A SKETCH FILE BYTE BY BYTE, THEN CLOSE IT
    //NOTE: CHECK DATA HAS BEEN INITIALISED... if (data == NULL) return (pressedKey == 27);
    //NOTE: TO GET ACCESS TO THE DRAWING STATE USE... state *s = (state*) data;
    //NOTE: TO GET THE FILENAME... char *filename = getName(d);
    //NOTE: DO NOT FORGET TO CALL show(d); AND TO RESET THE DRAWING STATE APART FROM
    //      THE 'START' FIELD AFTER CLOSING THE FILE
    if (data == NULL) {
        return (pressedKey == 27)
    }
    
    char *filename = getName(d);
    
    FILE *file = fopen();

    return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
    display *d = newDisplay(filename, 200, 200);
    state *s = newState();
    run(d, s, processSketch);
    freeState(s);
    freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
    if (n != 2) { // return usage hint if not exactly one argument
        printf("Use ./sketch file\n");
        exit(1);
    } else view(args[1]); // otherwise view sketch file in argument
    return 0;
}
#endif
