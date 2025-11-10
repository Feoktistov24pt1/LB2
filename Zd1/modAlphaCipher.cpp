#include "modAlphaCipher.h"
#include <cwctype>
#include <algorithm>
using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    for (size_t i = 0; i < numAlpha.size(); ++i) {
        alphaNum[numAlpha[i]] = static_cast<int>(i);
    }
    key = convert(getValidKey(skey));
}

wstring modAlphaCipher::getValidKey(const wstring& s)
{
    if (s.empty())
        throw cipher_error("Ключ не может быть пустым");
    wstring valid;
    for (wchar_t c : s) {
        wchar_t uc = towupper(c);
        if (numAlpha.find(uc) != wstring::npos) {
            valid += uc;
        }
    }
    if (valid.empty())
        throw cipher_error("Ключ должен содержать русские буквы");
    return valid;
}

wstring modAlphaCipher::getValidOpenText(const wstring& s)
{
    wstring valid;
    for (wchar_t c : s) {
        wchar_t uc = towupper(c);
        if (numAlpha.find(uc) != wstring::npos) {
            valid += uc;
        }
    }
    if (valid.empty())
        throw cipher_error("Открытый текст не содержит русских букв");
    return valid;
}

wstring modAlphaCipher::getValidCipherText(const wstring& s)
{
    if (s.empty())
        throw cipher_error("Шифротекст пуст");
    for (wchar_t c : s) {
        if (numAlpha.find(c) == wstring::npos) {
            throw cipher_error("Шифротекст содержит недопустимые символы");
        }
    }
    return s;
}

vector<int> modAlphaCipher::convert(const wstring& s)
{
    vector<int> result;
    for (wchar_t c : s) {
        auto it = alphaNum.find(c);
        if (it == alphaNum.end()) {
            throw cipher_error("Символ не найден в алфавите");
        }
        result.push_back(it->second);
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{
    wstring result;
    for (int i : v) {
        if (i < 0 || i >= static_cast<int>(numAlpha.size())) {
            throw cipher_error("Некорректное значение при расшифровке");
        }
        result += numAlpha[i];
    }
    return result;
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{
    wstring valid = getValidOpenText(open_text);
    vector<int> work = convert(valid);
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + key[i % key.size()]) % static_cast<int>(numAlpha.size());
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    wstring valid = getValidCipherText(cipher_text);
    vector<int> work = convert(valid);
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + static_cast<int>(numAlpha.size()) - key[i % key.size()]) % static_cast<int>(numAlpha.size());
    }
    return convert(work);
}
