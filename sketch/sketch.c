// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Allocate memory for a drawing state and initialise it
state *newState() {
    // TO DO
    newState = (state *) malloc(9);
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
    
    for (int a = 0; a < 2; a++ ) {
        leftMost[a] = b[a];
    }
    
    int operandCode = atoi(leftMost);
    
    switch (operandCode) {
        case 0:
            return DX;
            break;
            
        case 1:
            return DY;
            break;
        
        case 10:
            return TOOL;
        
        case 11:
            return DATA;
            
        default:
            break;
    }
    
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
    //TO DO
    char rightMost[3];
    
    for (int c = 0; c < 6; c++ ) {
        rightMost[c] = b[c];
    }
    
    int operandCode = atoi(rightMost);
    
    switch (operandCode) {
        case 0:
            return NONE;
            break;
            
        case 1:
            return LINE;
            break;
        
        case 10:
            return BLOCK;
        
        case 11:
            return COLOUR;
            
        case 100:
            return TARGETX;
        
        case 101:
            return TARGETY;
        
        case 110:
            return SHOW;
        
        case 111:
            return PAUSE;
        
        case 1000:
            return NEXTFRAME;
            
        default:
            break;
    }
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
    //TO DO
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
