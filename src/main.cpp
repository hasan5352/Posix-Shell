#include <iostream>
#include <string>
#include "MyInputStream.hpp"

using namespace std;

int main() {
    // Flush after every std::cout / std:cerr. Else, without unitbuf, c++ buffers output in memory before printing
    cout << unitbuf;
    cerr << unitbuf;

    MyInputStream *myIn = new MyInputStream(STDIN_FILENO);

    while (true) {
        cout << "$ ";
        
        char last_char;
        string userCommand = myIn->first_word(last_char);
        
        if (userCommand == "exit") break;

        if (userCommand == "echo") {
            if (last_char != '\n') cout << myIn->getline() << endl;
        } else {
            if (userCommand != "") cout << userCommand << ": command not found" << endl;
            if (last_char != '\n') myIn->flush();           // after \n there is nothing to flush, so read() will just block shell
        }
    }

    return 0;
}
