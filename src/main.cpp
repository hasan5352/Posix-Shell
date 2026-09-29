#include <iostream>
#include "MyInputStream/MyInputStream.hpp"

using namespace std;

int main() {
    // Flush after every std::cout / std:cerr. Else, without unitbuf, c++ buffers output in memory before printing
    cout << unitbuf;
    cerr << unitbuf;

    MyInputStream *myIn = new MyInputStream();

    while (true) {
        cout << endl << "$ ";
        
        myIn->clearBuffer();
        myIn->fillBuffer();


        char last_char;
        string userCommand = myIn->first_word(last_char);
        
        if (userCommand == "exit") {
            cout << endl;
            break;
        }

        if (userCommand == "type") {
            if (last_char == '\n') continue;
            string nxt = myIn->getline();

            if (nxt == "type" || nxt == "exit" || nxt == "echo") cout << endl << nxt << " is a shell builtin";
            else cout << endl << nxt << ": not found";

        } else if (userCommand == "echo") {
            if (last_char != '\n') cout << endl << myIn->getline();
        } else {
            if (userCommand != "") cout << endl << userCommand << ": command not found";
            if (last_char != '\n') myIn->clearBuffer();
        }
    }

    return 0;
}
