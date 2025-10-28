#include <iostream>
#include <locale>
#include <codecvt>
#include <wctype.h>
#include <stdexcept>
#include "table_cipher.h"

using namespace std;

wstring removeSpaces(const wstring& s) {
    wstring result;
    for (wchar_t c : s) {
        if (c != L' ') {
            result += c;
        }
    }
    return result;
}

wstring toUpper(const wstring& s) {
    wstring result = s;
    for (auto& c : result) {
        c = towupper(c);
    }
    return result;
}

bool Valid(const wstring& s) {
    wstring RusAlf = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto c : s) {
        if (RusAlf.find(c) == wstring::npos) {
            return false;
        }
    }
    return true;
}

wstring stringToWstring(const string& str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstringToString(const wstring& wstr) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string stlb;
    string txt;
    unsigned vbr;

    cout << "Шифр табличной перестановки" << endl;
    cout << "Введите количество столбцов: ";
    getline(cin, stlb);

    try {
        int key = stoi(stlb);
        TableCipher cipher(key);

        wstring last_encrypted;

        do {
            cout << "Выберите действие (Выйти=>0, Шифровать=>1, Расшифровать=>2): ";
            if (!(cin >> vbr)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Неверный ввод действия. Попробуйте снова." << endl;
                continue;
            }
            cin.ignore();

            if (vbr == 0) {
                break;
            } else if (vbr > 2) {
                cout << "Неверное действие" << endl;
                continue;
            }

            cout << "Введите текст: ";
            getline(cin, txt);
            wstring text = stringToWstring(txt);

            text = toUpper(text);
            text = removeSpaces(text);

            if (text.empty()) {
                cout << "Текст пуст после удаления пробелов." << endl;
                continue;
            }

            if (!Valid(text)) {
                cout << "Используйте только русские буквы (А-Я, а-я). Пробелы будут удалены автоматически." << endl;
                continue;
            }

            if (vbr == 1) {
                try {
                    last_encrypted = cipher.encrypt(text);
                    cout << "Зашифрованный текст: " << wstringToString(last_encrypted) << endl;
                } catch (const exception& e) {
                    cout << "Ошибка шифрования: " << e.what() << endl;
                }
            } else { // vbr == 2
                if (last_encrypted.empty()) {
                    cout << "Сначала зашифруйте текст!" << endl;
                } else {
                    try {
                        wstring decrypted = cipher.decrypt(last_encrypted);
                        cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                    } catch (const exception& e) {
                        cout << "Ошибка расшифровки: " << e.what() << endl;
                    }
                }
            }
        } while (true);

    } catch (const invalid_argument& e) {
        cerr << "Ошибка ввода: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Неизвестная ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
