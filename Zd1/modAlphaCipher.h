#ifndef MODALPHACIPHER_H
#define MODALPHACIPHER_H

#include <vector>
#include <string>
#include <map>
#include <stdexcept>

class modAlphaCipher
{
private:
    static const std::wstring numAlpha;
    std::map<wchar_t, int> alphaNum;
    std::vector<int> key;

    std::vector<int> convert(const std::wstring& s);
    std::wstring convert(const std::vector<int>& v);

public:
    modAlphaCipher() = delete;
    modAlphaCipher(const std::wstring& skey);
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};

#endif
