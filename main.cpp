#include <iostream>
#include "parser.h"
#include "tokenizer.h"

using namespace std;

int main(void) {
    cout << "Ree Interpreter 0.1" << endl;

    string line = "";
    while (true) {
        cout << "ree=> ";
        getline(cin, line);
        if (line == "exit") break;
        std::queue<STToken> tks = tokenize(line);
        std::queue<STToken> dbg;
        while (!tks.empty()) {
            cout << "Token " << tks.front().val << endl;
            dbg.push(tks.front());
            tks.pop();
        }
        cout << line << endl;
        cout << parse(dbg) << endl;
    }
    return 0;
}

