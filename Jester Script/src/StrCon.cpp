#include "StrCon.h"
#include "Object.h"
#include "Token.h"

namespace jts
{
    bool TokIsLtrl(Tok* tok)
    {
        size_t len = tok->value.length();

        /* ADD CHARS & BOOLS */
        if (tok->value[0] == '\'' && tok->value[len - 1] == '\'')
        {
            tok->type = Type::CHAR;
            return true;
        }
        if (tok->value[0] == '\"' && tok->value[len - 1] == '\"')
        {
            tok->type = Type::STRING;
            return true;
        }
        if (tok->value == "true" || tok->value == "false")
        {
            tok->type = Type::BOOL;
            return true;
        }
        else
        {
            size_t perCount = 0;

            for (size_t i = 0; i < len; i++)
            {
                if (tok->value[i] == '.')
                {
                    perCount++;
                    continue;
                }

                /* If not an integer */
                if (tok->value[i] - '0' < 0 || tok->value[i] - '0' > 9)
                {
                    return false;
                }
            }

            if (perCount == 0) tok->type = Type::INT;
            else if (perCount == 1) tok->type = Type::FLOAT;
            else return false;
        }

        return true;
    }

    Obj* TokToLtrl(Tok* tok)
    {
        Obj* obj = new Obj();

        if (tok->type == Type::CHAR)
        {
            obj->_char = tok->value.substr(1, 1)[0];
        }
        else if (tok->type == Type::BOOL)
        { 
            if (tok->value == "true")
            {
                obj->_bool = true;
            }
            else
            {
                obj->_bool = false;
            }
        }
        else // Case INT or FLOAT
        {
            bool fPart = false;
            int iPartLen = tok->value.length() - 1;

            for (size_t i = 0; i < tok->value.length(); ++i)
            {
                if (fPart)
                {
                    iPartLen--;
                }
                if (tok->value[i] == '.')
                {
                    fPart = true;
                    --iPartLen;
                    continue;
                }
            }

            if (tok->type == Type::INT)
            {
                obj->_int = 0;

                for (size_t i = 0; i < tok->value.length(); i++)
                {
                    obj->_int += (tok->value[i] - '0') * pow(10, iPartLen--);
                }
            }
            else // case FLOAT
            {
                obj->_float = 0.0f;

                for (size_t i = 0; i < tok->value.length(); i++)
                {
                    if (tok->value[i] == '.')
                    {
                        continue;
                    }
                    obj->_float += (tok->value[i] - '0') * pow(10.0f, iPartLen--);
                }
            }
        }

        return obj;
    }
}
