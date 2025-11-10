#ifndef TABLE_CIPHER_H
#define TABLE_CIPHER_H

#include <string>
#include <stdexcept>
#include <vector>

class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};

class TableCipher {
private:
    int columns;

    std::wstring getValidText(const std::wstring& s);
    std::wstring getValidCipherText(const std::wstring& s);

public:
    TableCipher(int key);
    std::wstring encrypt(const std::wstring& plain_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};

#endif
