#include <iostream>
#include "parser.h"
#include "tokenizer.h"

using namespace std;

int main(void) {
    cout << "Nope 0.1" << endl;

    STExecScope sc;
    string line = "";
    while (true) {
        sc.nodes.clear();
        sc.vals.clear();
        cout << "nope=> ";
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
        cout << parse(sc, dbg) << endl;
    }
    return 0;
}

