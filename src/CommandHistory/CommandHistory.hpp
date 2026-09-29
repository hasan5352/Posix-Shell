#pragma once

class CommandHistory {
private:
    const static int HISTORY_SIZE = 100, COMMAND_SIZE = 256;
    char history[HISTORY_SIZE][COMMAND_SIZE];
    int curr_history_size, curr_history_position;
public:
    CommandHistory();
    void append(const char cmd[], const short unsigned cmd_size);
    int fillPreviousCmd(char cmd[]);
    int fillNextCmd(char cmd[]);
};