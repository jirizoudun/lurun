
#include "../common.h"

namespace Lua {

    using std::string;

    StringObject::StringObject(string str) {
        this->str = new string(str);
        compute_hash();
    }
    StringObject::StringObject(const char* str) {
        this->str = new string(str);
        compute_hash();
    }
    StringObject::StringObject(const StringObject & other) {
        str = new string(other.getString());
        compute_hash();
    }
    StringObject::~StringObject() {
        delete str;
    }

    void StringObject::compute_hash() {
        hash = std::hash<string>()(*str);
    }

    void StringObject::print() const {
        printf("<%zu> %s\n", hash, str->c_str());
    }
    const char*StringObject::toString() const {
        return str->c_str();
    }
    string StringObject::getString() const {
        return *str;
    }
    size_t StringObject::getHash() const {
        return hash;
    }

    bool StringObject::operator==(const StringObject & other) const {
        return *str == *(other.str);
    }
}
