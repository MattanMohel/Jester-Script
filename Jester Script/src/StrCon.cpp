#include "StrCon.h"
#include "Object.h"
#include "Token.h"
#include "VM.h"

namespace jts {

    bool tokIsLtrl(Tok* tok) {
        size_t len = tok->symbol.length();

        if (tok->symbol[0] == '\"' && tok->symbol[len - 1] == '\"') {
            if (len == 3) tok->type = Type::CHAR;
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
        Obj* obj = vm->objPool->acquire();
        obj->type = tok->type;
        obj->spec = Spec::VALUE;

        switch (tok->type) {
        case Type::CHAR:

            obj->_char = tok->symbol.substr(1, 1)[0];
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

        default: // case FLOAT, INT
        {
            bool fPart = false;
            int iPartLen = tok->symbol.length() - 1;

            for (size_t i = 0; i < tok->symbol.length(); ++i) {
                if (fPart) {
                    iPartLen--;
                }
                if (tok->symbol[i] == '.') {
                    fPart = true;
                    --iPartLen;
                    continue;
                }
            }

            if (tok->type == Type::INT) {
                obj->_int = 0;

                for (size_t i = 0; i < tok->symbol.length(); i++) {
                    obj->_int += (tok->symbol[i] - '0') * pow(10, iPartLen--);
                }
            }
            else // case FLOAT
            {
                obj->_float = 0.0f;

                for (size_t i = 0; i < tok->symbol.length(); i++) {
                    if (tok->symbol[i] == '.') {
                        continue;
                    }
                    obj->_float += (tok->symbol[i] - '0') * pow(10.0f, iPartLen--);
                }
            }
        }
        }

        return obj;
    }
}
