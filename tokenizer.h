#include "data.h"

std::queue<STToken> tokenize(std::string raw);
STToken tokenize_numeric(std::string raw, int pos);
STToken tokenize_string(std::string raw, int pos);
int op_index(string raw, int pos);

