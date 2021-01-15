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
    s->data = 0x00000000;
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
    b = b >> 6;
    int opcode = (int)b;
    return opcode;
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
    //TO DO
    byte signExtend = 0x20;
    signed char a;
    int opcode = getOpcode(b);
    b = b << 2;
    b = b >> 2;
    int operand;
    if (opcode == DATA) {
        operand = (int)b;
        return operand;
    }
    if ((signExtend & b) != 0x00) {
        byte signExpression = 0x1f;
        a = (b & signExpression);
        operand = -32 + (int)a;
    }
    
    else {
        a = b;
        operand = (int)a;
    }
    return operand;
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
            s->end = true;
            break;
        
        case TOOL:
            switch (operand) {
                case NONE:
                    s->tool = NONE;
                    s->data = 0x00000000;
                    break;
                
                case LINE:
                    s->tool = LINE;
                    s->data = 0x00000000;
                    break;
                    
                case BLOCK:
                    s->tool = BLOCK;
                    s->data = 0x0000000;
                    break;
                    
                case COLOUR:
                    colour(d, s->data);
                    s->data = 0x00000000;
                    break;
                    
                case TARGETX:
                    s->tx = (int)s->data;
                    s->data = 0x00000000;
                    break;
                
                case TARGETY:
                    s->ty = (int)s->data;
                    s->data = 0x00000000;
                    break;
                
                case SHOW:
                    show(d);
                    break;
                
                case PAUSE:
                    pause(d, (int)s->data);
                    break;
                
                case NEXTFRAME:
                    s->end = true;
                    return;
                    
                default:
                    break;
            }
            break;
        
        case DATA:
            s->data = s->data << 6;
            s->data = s->data | operand;
            break;
            
        default:
            break;
    }
    
    if (s->tool == LINE && s->end == true) {
        line(d, s->x, s->y, s->tx, s->ty);
    }
    
    if (s->tool == BLOCK && s->end == true) {
        block(d, s->x, s->y, (s->tx - s->x), (s->ty - s->y));
    }
    
    if (s->end == true) {
        s->end = false;
        s->x = s->tx;
        s->y = s->ty;
    }
    
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
        return (pressedKey == 27);
    }
    // From Test
    char *filename = getName(d);
    state *s = (state *) data;
    FILE *file = fopen(filename, "rb");
    unsigned int position = 0x00;
    bool shown = false;
    
    
    while (!feof(file)) {
        byte command = getc(file);
        if (s->start != position) {
            if (command == 0x88) {
                position += 1;
            }
        }
        else {
            obey(d, s, command);
            if (s->end == true) {
                show(d);
                shown = true;
                position += 1;
                s->start = position;
                break;
            }
        }
    }
    
    s->x = 0;
    s->y = 0;
    s->tx = 0;
    s->ty = 0;
    s->tool = LINE;
    s->data = 0x00;
    s->end = false;
    fclose(file);
    if (shown == false) {
        s->start = 0x0000000;
        show(d);
    }
    
    
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
