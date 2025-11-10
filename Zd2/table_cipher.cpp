#include "table_cipher.h"
#include <vector>
#include <cctype>
#include <cwctype>
#include <algorithm>
using namespace std;

TableCipher::TableCipher(int key) {
    if (key <= 0)
        throw cipher_error("Количество столбцов должно быть > 0");
    columns = key;
}

wstring TableCipher::getValidText(const wstring& s) {
    if (s.empty())
        throw cipher_error("Текст пуст");
    wstring rus = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    wstring result;
    for (wchar_t c : s) {
        if (c != L' ') {
            wchar_t uc = towupper(c);
            if (rus.find(uc) == wstring::npos)
                throw cipher_error("Текст содержит недопустимые символы");
            result += uc;
        }
    }
    if (result.empty())
        throw cipher_error("Текст не содержит русских букв");
    return result;
}

wstring TableCipher::getValidCipherText(const wstring& s) {
    if (s.empty())
        throw cipher_error("Шифротекст пуст");
    wstring rus = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (wchar_t c : s) {
        if (rus.find(c) == wstring::npos)
            throw cipher_error("Шифротекст содержит недопустимые символы");
    }
    return s;
}

wstring TableCipher::encrypt(const wstring& plain) {
    wstring valid = getValidText(plain);
    int n = static_cast<int>(valid.length());
    int rows = (n + columns - 1) / columns;
    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(columns, L' '));
    int pos = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (pos < n) {
                grid[r][c] = valid[pos++];
            }
        }
    }
    wstring out;
    for (int c = columns - 1; c >= 0; --c) {
        for (int r = 0; r < rows; ++r) {
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}

wstring TableCipher::decrypt(const wstring& cipher) {
    wstring valid = getValidCipherText(cipher);
    int n = static_cast<int>(valid.length());
    int rows = (n + columns - 1) / columns;
    int fullCols = n % columns;
    if (fullCols == 0) fullCols = columns;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(columns, L' '));
    int pos = 0;
    for (int c = columns - 1; c >= 0; --c) {
        int h = rows;
        if (c >= fullCols) h = rows - 1;
        for (int r = 0; r < h; ++r) {
            if (pos < n) {
                grid[r][c] = valid[pos++];
            }
        }
    }

    wstring out;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}
