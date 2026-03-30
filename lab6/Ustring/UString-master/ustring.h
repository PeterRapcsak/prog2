#ifndef USTRING_H
#define USTRING_H

#include "string5.h"

// UString osztaly
// A String osztalybol orokol, UCase() segitsegevel
class UString : public String {
    static bool ucase_flag; // statikus flag a nagybetus kiirashoz
public:
    // Konstruktor C-sztringbol
    // p - C-string (default: "")
    UString(const char *p = "") : String(p) {}

    // Konstruktor karakterbol
    // ch - karakter
    UString(char ch) : String(ch) {}

    // Konvertalo konstruktor String-bol
    // s - String
    UString(const String& s) : String(s) {}

    // Nagybetus kiiras be/kikapcsolasa
    static void UCase(bool b) { ucase_flag = b; }

    // Nagybetus kiiras allapotanak lekerdezese
    // true ha nagybetus kiiras aktiv
    static bool UCase() { return ucase_flag; }
};

// Kiiras ostream-re
// os - ostream objektum
// s  - UString, amit kiirunk
std::ostream& operator<<(std::ostream& os, const UString& s);

#endif // USTRING_H
