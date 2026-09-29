#include <unistd.h>
#pragma once

void disableRawMode();
void enableRawMode();
ssize_t readChar(char *ch);
void flush_os_read_buffer();
char isArrowKey();