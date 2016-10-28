#include "tokenizer.h"

using namespace std;

queue<STToken> tokenize(string raw) {
    queue<STToken> tokens;
    string l_buffer = "";
    for (int i = 0; i < raw.length(); i++) {
        if (isdigit(raw.at(i))) {
            STToken tk = tokenize_numeric(raw, i);
            tokens.push(tk);
            i += tk.val.length() - 1;
        } else if (raw.at(i) == '"') {
            STToken tk = tokenize_string(raw, i);
            tokens.push(tk);
            i += tk.val.length() + 1;
        } else if (raw.at(i) != ' ') {
            int op = op_index(raw, i);
            if (op != -1) {
                if (l_buffer.size() > 0) {
                    STToken tk = { l_buffer, STTokenType::Reference };
                    tokens.push(tk);
                    l_buffer = "";
                }
                STToken tk = { to_str(op), STTokenType::Operator };
                tokens.push(tk);
                i += ST_OPS[op].val.length() - 1;
            } else {
                l_buffer.push_back(raw.at(i));
            }
        }
    }
    if (l_buffer.size() > 0) {
         STToken tk = { l_buffer, STTokenType::Reference };
         tokens.push(tk);                                                             l_buffer = "";
    }
    return tokens;
}

STToken tokenize_numeric(string raw, int pos) {
    STToken tk = { "", STTokenType::Integer };
    for (int i = pos; i < raw.length(); i++) {
        if (isdigit(raw.at(i))) {
            tk.val.push_back(raw.at(i));
        } else if (raw.at(i) == '.' && tk.type != STTokenType::Float) {
            tk.val.push_back(raw.at(i));
            tk.type = STTokenType::Float;
        } else {
            break;
        }
    }
    return tk;
}

STToken tokenize_string(string raw, int pos) {
    STToken tk = { "", STTokenType::String };
    for (int i = (pos + 1); i < raw.length(); i++) {
        if ((raw.at(i) != '"') || (raw.at(i - 1) == '/')) {
            tk.val.push_back(raw.at(i));
        } else {
            break;
        }
    }
    return tk;
}


int op_index(string raw, int pos) {
    for (int i = 0; i < ST_OP_COUNT; i++) {
        if (raw.at(pos) == ST_OPS[i].val.at(0)) {
             bool matched = true;
             for (int j = 1; j < ST_OPS[i].val.length(); j++) {
                  if ((raw.length() < pos + j + 1)
                      || (!(raw.at(pos + j) == ST_OPS[i].val.at(j)))) {
                      matched = false;
                      break;
                  }
             }
             if (matched) return i;
        }
    }
    return -1;
}    

            


