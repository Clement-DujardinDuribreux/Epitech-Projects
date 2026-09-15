#pragma once

#include <string>
#include <uuid/uuid.h>

class Uuid {
public:
    static std::string generate()
    {
        uuid_t id;
        char   str[37];
        uuid_generate(id);
        uuid_unparse_lower(id, str);
        return std::string(str);
    }

private:
    Uuid() = delete;
};

