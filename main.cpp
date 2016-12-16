#include <iostream>
#include "parser.h"
#include "tokenizer.h"
#include "evaluator.h"

#include "lang/core.h"

using namespace std;

int main(void) {
    Lang::Core c;
    while (true) {
        string line = "", buf = "";
        cout << "peridot=> " << endl;
        while (!cin.eof()) {
            string buf;
            getline(cin, buf);
            if (!line.empty()) line += "\n";
            line += buf;
        }
        cin.clear();

        if (line == "exit") break;
        std::istringstream is(line); /* TODO: dirty! */
        c.parse(is);
    }
    return 0;
}

