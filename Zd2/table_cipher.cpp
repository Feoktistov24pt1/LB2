#include "table_cipher.h"
#include <algorithm>
#include <vector>
#include <stdexcept>

TableCipher::TableCipher(int key)
{
    if (key <= 0) {
        throw std::invalid_argument("Ключ должен быть положительным целым числом (> 0)");
    }
    columns = key;
}

std::wstring TableCipher::encrypt(const std::wstring& plain)
{
    if (plain.empty()) {
        throw std::invalid_argument("Текст для шифрования не может быть пустым");
    }

    int n = static_cast<int>(plain.length());
    int rows = (n + columns - 1) / columns;

    std::vector<std::vector<wchar_t>> grid(rows, std::vector<wchar_t>(columns, L' '));
    int pos = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (pos < n) {
                grid[r][c] = plain[pos++];
            }
        }
    }

    std::wstring out;
    out.reserve(n);

    for (int c = columns - 1; c >= 0; --c) {
        for (int r = 0; r < rows; ++r) {
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}

std::wstring TableCipher::decrypt(const std::wstring& cipher)
{
    if (cipher.empty()) {
        throw std::invalid_argument("Шифротекст не может быть пустым");
    }

    int n = static_cast<int>(cipher.length());
    int rows = (n + columns - 1) / columns;
    int fullCols = (n % columns == 0) ? columns : (n % columns);

    std::vector<std::vector<wchar_t>> grid(rows, std::vector<wchar_t>(columns, L' '));
    int pos = 0;

    for (int c = columns - 1; c >= 0; --c) {
        int h = (c < fullCols) ? rows : (rows - 1);
        for (int r = 0; r < h; ++r) {
            if (pos >= n) {
                throw std::runtime_error("Некорректная длина текста");
            }
            grid[r][c] = cipher[pos++];
        }
    }

    if (pos != n) {
        throw std::runtime_error("Ошибка при расшифровке (избыточные символы)");
    }

    std::wstring out;
    out.reserve(n);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}
