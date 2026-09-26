#include "MyInputStream.hpp"

using namespace std;

MyInputStream::MyInputStream(int file_descriptor){
    this->fd = file_descriptor;
}

void MyInputStream::flush() {
    ssize_t read_ans = 1;
    char ch;
    while (read(fd, &ch, 1) > 0) { 
        if (ch == '\n') return;
    }
}

string MyInputStream::first_word(char &last_char) {
    string result = "";
    char ch;
    bool word_started = false;

    while (read(fd, &ch, 1) > 0) {
        last_char = ch;

        if (ch == ' ' && !word_started) continue; 
        if (ch == ' ' || ch == '\n') break;

        word_started = true;
        result.push_back(ch);
    }
    return result;
}

string MyInputStream::getline() {
    string result = "";
    char ch;
    bool line_found = false;

    while (read(fd, &ch, 1) > 0) {
        if (ch == '\n') break;
        if (ch == ' ' && !line_found) continue;

        line_found = true;
        if (ch != ' ') result += ch;
        else if (result.back() != ' ') result += ch;
    }
    return result;
}

