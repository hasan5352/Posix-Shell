#pragma once
#include <string>
#include <unistd.h>

class MyInputStream {
private:
    int fd;
public:
    MyInputStream(int file_descriptor);
    void flush();
    std::string first_word(char &last_char);
    std::string getline();
};

