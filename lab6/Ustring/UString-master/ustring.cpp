#include <cctype>
#include "memtrace.h"
#include "ustring.h"

// Statikus init
bool UString::ucase_flag = false;

// Kiiras ostream-re
std::ostream& operator<<(std::ostream& os, const UString& s) {

    // Ha UCase aktiv, nagybetusitve irja ki a sztringet
    if (UString::UCase()) {
        for (size_t i = 0; i < s.size(); i++)
            os << (char)toupper((unsigned char)s[i]);
    } else {
        os << s.c_str();
    }
    return os;
}
