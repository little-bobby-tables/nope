#include <iostream>
#include <sstream>

#include "lang/driver.hpp"
#include "vm/driver.hpp"

using namespace std;

static_assert((sizeof(uintptr_t) == sizeof(long) && sizeof(long) == 8),
"Unsupported architecture: peridot was written with a single architecture in mind — x86-64. \
It relies on long having the size of 8 bytes to store integers, providing enough capacity \
for demonstration purposes and removing the need to handle bignums separately. \
Any attempt to make this thing cross-platform is just not worth the time.");

int main(void) {
    Lang::Driver lang;
    VM::Driver vm;

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

        Lang::ExpressionListNode* program = lang.parse(is);
        vm.process(program);
    }
    return 0;
}

