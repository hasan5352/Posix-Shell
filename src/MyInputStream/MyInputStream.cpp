#include "MyInputStream.hpp"
#include "../utils/utils.hpp"
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <poll.h>

using namespace std;


MyInputStream::MyInputStream(){ 
    history = new CommandHistory();
    curr_buffer_size = curr_buffer_read_idx = cursor_position = 0;
    enableRawMode(); 
}


void MyInputStream::fillBuffer(){
    char ch;
    while (readChar(&ch) > 0) {

        if (curr_buffer_size == BUFFER_SIZE && ch != '\n') {
            cerr << endl << "Buffer overflow: command too long. Max limit = " << BUFFER_SIZE << " chars" << endl;
            flush_os_read_buffer();  // Handle case when text was pasted before hitting BUFFER_SIZE limit.
            break;
        }

        if (ch == '\033') {             // Handle Escape sequences (Arrow keys, Home, End, F-keys, etc.)

            char arrowKey = isArrowKey();

            if (arrowKey == 'L') moveLeft();
            else if (arrowKey == 'R') moveRight();
            else if (arrowKey == 'D' || arrowKey == 'U') NavigateCommands(arrowKey);

            flush_os_read_buffer();
            continue;
        }

        if (ch == 23) continue;
        
        if (static_cast<int>(ch) == BACKSPACE_ASCII) {
            backspace();
            continue;
        };

        if (ch == '\n') {
            buffer[curr_buffer_size++] = ch;
            if (curr_buffer_size > 1) history->append(buffer, curr_buffer_size);
            break;
        }
        
        curr_buffer_size++;

        for (int i = cursor_position; i < curr_buffer_size; i++) {
            char temp = buffer[i];
            buffer[i] = ch;
            ch = temp;
            cout << buffer[i];
        }

        cursor_position++;

        render_cursor_on_correct_position();
    }
}


void MyInputStream::clearBuffer(){
    if (curr_buffer_size > 0) memset(buffer, 0, curr_buffer_size);
    curr_buffer_size = curr_buffer_read_idx = cursor_position = 0;
}


void MyInputStream::backspace(){
    if (cursor_position == 0) return;
    
    cursor_position--; curr_buffer_size--;
    cout << "\b";

    for (int i = cursor_position; i < curr_buffer_size; i++) {
        buffer[i] = buffer[i+1];
        cout << buffer[i];
    }

    cout << " \b";

    render_cursor_on_correct_position();
    
    buffer[curr_buffer_size] = 0;
}



// ---------------------------------------------------------------------------------------------

string MyInputStream::first_word(char &last_ch) {
    string result = "";
    bool word_started = false;
    curr_buffer_read_idx = 0;

    while (curr_buffer_read_idx < curr_buffer_size) {
        last_ch = buffer[curr_buffer_read_idx++];

        if (last_ch == ' ' && !word_started) continue; 
        if (last_ch == ' ' || last_ch == '\n') break;

        word_started = true;
        result += last_ch;
    }
    return result;
}


string MyInputStream::getline() {
    string result = "";
    char ch;
    bool line_found = false;

    while (curr_buffer_read_idx < curr_buffer_size) {
        ch = buffer[curr_buffer_read_idx++];

        if (ch == '\n') break;
        if (ch == ' ' && !line_found) continue;

        line_found = true;
        if (ch != ' ') result += ch;
        else if (result.back() != ' ') result += ch;
    }
    return result;
}

// --------------------------------------------------------------------------------------------
void MyInputStream::moveLeft(){
    if (cursor_position == 0) return;

    cout << '\b';
    cursor_position--;
}

void MyInputStream::moveRight(){
    if (cursor_position == curr_buffer_size) return;

    cout << "\033[C";
    cursor_position++;
}

// ---------------------------------------------------------------

void MyInputStream::render_cursor_on_correct_position(){
    // by default, cursor moves to the end of rendered sequence. Thus bring it back to correct position

    int cursor_positions_to_move_back = curr_buffer_size - cursor_position;
    if (cursor_positions_to_move_back > 0) cout << "\033[" << cursor_positions_to_move_back << "D";
    cout << flush;
}

//-------------------------------------------------------------------------

void MyInputStream::NavigateCommands(char key){
    if (key == 'U' && !history->fillPreviousCmd(buffer)) return;
    if (key == 'D' && !history->fillNextCmd(buffer)) return;

    if(cursor_position > 0) cout << "\033[" << cursor_position << "D";
    cout << "\033[K";

    cursor_position = curr_buffer_size = 0;
    
    for (int i = 0; i < BUFFER_SIZE && buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') { buffer[i] = '\0'; break; }

        cursor_position++; curr_buffer_size++;
        cout << buffer[i];
    }
    cout << flush;
}
