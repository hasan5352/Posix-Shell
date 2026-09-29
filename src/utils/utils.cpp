#include "utils.hpp"
#include <termios.h>
#include <poll.h>
#include <iostream>

static struct termios orig_termios;

void disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(){
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

ssize_t readChar(char *ch){
    ssize_t ans = read(STDIN_FILENO, ch, 1);
    // std::cout << *ch;
    return ans;
}

void flush_os_read_buffer(){
    char junk;
    pollfd pfd = { STDIN_FILENO, POLLIN, 0};

    while (poll(&pfd, 1, 0) > 0 && (POLLIN & pfd.revents)){
        if (read(STDIN_FILENO, &junk, 1) <= 0) break;
    }
};

char isArrowKey(){
    char ch;
    pollfd pfd = { STDIN_FILENO, POLLIN, 0};

    for (int i = 0; i < 2; i++) {
        bool pendingToRead = poll(&pfd, 1, 0) > 0 && (POLLIN & pfd.revents);
        if (!pendingToRead || read(STDIN_FILENO, &ch, 1) <= 0) break;
        if (i == 0) {
            if (ch != '[') break;
            continue;
        }

        if (ch == 'D') return 'L';
        if (ch == 'C') return 'R';
        if (ch == 'A') return 'U';
        if (ch == 'B') return 'D';
    }

    return 0;
}