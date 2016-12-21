#include <iostream>
#include <sstream>

#include "lang/driver.hpp"

using namespace std;

int main(void) {
    Lang::Driver lang;
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
        lang.parse(is);
    }
    return 0;
}

