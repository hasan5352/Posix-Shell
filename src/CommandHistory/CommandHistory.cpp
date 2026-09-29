#include "CommandHistory.hpp"
#include <cstring>


CommandHistory::CommandHistory(){
    curr_history_size = curr_history_position = 0;
}

void CommandHistory::append(const char cmd[], const short unsigned cmd_size){
    int idx = curr_history_size % HISTORY_SIZE;
    memcpy(history[idx], cmd, cmd_size);
    curr_history_position = ++curr_history_size;
}

int CommandHistory::fillPreviousCmd(char cmd[COMMAND_SIZE]){
    if (curr_history_size == 0) return 0;
    
    int oldest = (curr_history_size <= HISTORY_SIZE) ? 0 : curr_history_size - HISTORY_SIZE;
    if (curr_history_position <= oldest) return 0;
    
    curr_history_position--;
    
    memcpy(cmd, history[curr_history_position % HISTORY_SIZE], COMMAND_SIZE);
    return 1;
}

int CommandHistory::fillNextCmd(char cmd[COMMAND_SIZE]){
    if (curr_history_position == curr_history_size) return 0;
    curr_history_position++;

    if (curr_history_position == curr_history_size) 
        memset(cmd, '\n', COMMAND_SIZE);
    else 
        memcpy(cmd, history[curr_history_position % HISTORY_SIZE], COMMAND_SIZE);
    
    return 1;
}