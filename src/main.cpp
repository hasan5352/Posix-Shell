#include <iostream>
#include <string>

using namespace std;

int main() {
    // Flush after every std::cout / std:cerr. Else, without unitbuf, c++ buffers output in memory before printing
    cout << unitbuf;
    cerr << unitbuf;

    while (true) {
        cout << "$ ";
        string userCommand;
        cin >> userCommand;
        if (userCommand == "exit") break;
        cout << userCommand << ": command not found" << endl;
    }

}
