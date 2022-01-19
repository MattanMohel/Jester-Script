#include "StrCon.h"
#include "Object.h"
#include "Token.h"
#include "VM.h"

namespace jts {

    bool tokIsLtrl(Tok* tok) {
        size_t len = tok->symbol.length();

        if (tok->symbol[0] == '\"' && tok->symbol[len - 1] == '\"') {
            if (len == 3 || len == 4 && tok->symbol[1] == '\\') tok->type = Type::CHAR;
            else tok->type = Type::STRING;

            return true;
        }

        else {
            size_t perCount = 0;

            for (size_t i = 0; i < len; i++) {
                if (tok->symbol[i] == '-') continue;

                if (tok->symbol[i] == '.') {
                    perCount++;
                    continue;
                }

                // if not an integer
                if (tok->symbol[i] - '0' < 0 || tok->symbol[i] - '0' > 9) {
                    return false;
                }
            }

            if (perCount == 0) tok->type = Type::INT;
            else if (perCount == 1) tok->type = Type::FLOAT;
            else return false;
        }

        return true;
    }

    Obj* tokToLtrl(VM* vm, Tok* tok) {
        Obj* obj = env::cmplObj(vm);
        obj->type = tok->type;
        obj->spec = Spec::VALUE;

        switch (tok->type) {
        case Type::CHAR:

            if (tok->symbol.length() == 4) {
                char c = tok->symbol.substr(2, 2)[0];
                switch (c) {
                case 'n':
                    obj->_char = '\n';
                    break;
                case '0':
                    obj->_char = '\0';
                    break;
                case 't':
                    obj->_char = '\t';
                    break;
                default:
                    env::assert(true, "created invalid char '\\'" + c);
                }            
            }
            else {
                obj->_char = tok->symbol.substr(1, 1)[0];
            }
            break;

        case Type::STRING:

            obj->_string = new str(tok->symbol.substr(1, tok->symbol.length() - 2));
            break;

        case Type::FLOAT:

            obj->_float = std::stof(tok->symbol);
            break;

        case Type::INT:

            obj->_int = std::stoi(tok->symbol);
            break;
        }

        return obj;
    }
}
