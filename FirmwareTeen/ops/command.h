// Adapted from TeleType source code.
// See license on https://github.com/monome/teletype

#ifndef _COMMAND_H_
#define _COMMAND_H_


/////////////////////////////////////////
// command.h
/////////////////////////////////////////
const int STACK_SIZE=16;

class command_state_t {
private:
    int16_t values[STACK_SIZE];
    int16_t top;
public:
    command_state_t() { init(); }

    int16_t pop() {
        top--;
        return values[top];
        }
    void push( int16_t data) {
        if( top==STACK_SIZE-1) return; // prevent overflow
        values[top] = data;
        top++;
    }
    int16_t stack_size() { return top; }
    void init() { top = 0; }
};

#define cs_pop(x) x->pop()

#endif
