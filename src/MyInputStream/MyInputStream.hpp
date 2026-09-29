#pragma once
#include <string>
#include "../CommandHistory/CommandHistory.hpp"

class MyInputStream {
private:

    CommandHistory *history;

    static const unsigned short BUFFER_SIZE = 256, BACKSPACE_ASCII = 127;

    char buffer[BUFFER_SIZE];
    unsigned short curr_buffer_size, curr_buffer_read_idx, cursor_position;

    void backspace();

    void moveLeft();
    void moveRight();

    void render_cursor_on_correct_position();

    void NavigateCommands(char key);

public:
    MyInputStream();
    void fillBuffer();
    void clearBuffer();
    
    std::string first_word(char &last_char);
    std::string getline();
};

