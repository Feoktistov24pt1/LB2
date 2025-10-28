#include "modAlphaCipher.h"
#include <algorithm>
#include <stdexcept>

const std::wstring modAlphaCipher::numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    if (skey.empty()) {
        throw std::invalid_argument("Ключ не может быть пустым");
    }

    for (size_t i = 0; i < numAlpha.size(); ++i) {
        alphaNum[numAlpha[i]] = static_cast<int>(i);
    }

    for (wchar_t c : skey) {
        if (alphaNum.find(c) == alphaNum.end()) {
            throw std::invalid_argument("Ключ содержит недопустимые символы");
        }
    }

    key = convert(skey);
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for (wchar_t c : s) {
        auto it = alphaNum.find(c);
        if (it == alphaNum.end()) {
            throw std::invalid_argument("Текст содержит недопустимые символы");
        }
        result.push_back(it->second);
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for (int i : v) {
        if (i < 0 || static_cast<size_t>(i) >= numAlpha.size()) {
            throw std::out_of_range("Некорректное значение при конвертации в символ");
        }
        result += numAlpha[i];
    }
    return result;
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    if (open_text.empty()) {
        throw std::invalid_argument("Открытый текст пуст");
    }
    std::vector<int> work = convert(open_text);
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + key[i % key.size()]) % static_cast<int>(numAlpha.size());
    }
    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    if (cipher_text.empty()) {
        throw std::invalid_argument("Шифротекст пуст");
    }
    std::vector<int> work = convert(cipher_text);
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + static_cast<int>(numAlpha.size()) - key[i % key.size()]) % static_cast<int>(numAlpha.size());
    }
    return convert(work);
}
