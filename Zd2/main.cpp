#include <iostream>
#include <locale>
#include <codecvt>
#include <wctype.h>
#include "table_cipher.h"
using namespace std;

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
    cout << "Шифр табличной перестановки\n";
    cout << "Введите количество столбцов: ";
    getline(cin, stlb);

    try {
        int key = stoi(stlb);
        TableCipher cipher(key);

        unsigned vbr;
        wstring last_encrypted;
        do {
            cout << "Выберите действие (Выйти=>0, Шифровать=>1, Расшифровать=>2): ";
            cin >> vbr;
            cin.ignore();

            if (vbr == 0) break;
            if (vbr > 2) {
                cout << "Неверное действие\n";
                continue;
            }

            string txt;
            cout << "Введите текст (русские буквы, можно с пробелами): ";
            getline(cin, txt);
            wstring text = stringToWstring(txt);

            try {
                if (vbr == 1) {
                    last_encrypted = cipher.encrypt(text);
                    cout << "Зашифрованный текст: " << wstringToString(last_encrypted) << endl;
                } else {
                    if (last_encrypted.empty()) {
                        cout << "Сначала зашифруйте текст!\n";
                    } else {
                        wstring decrypted = cipher.decrypt(last_encrypted);
                        cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                    }
                }
            } catch (const cipher_error& e) {
                cerr << "Ошибка обработки текста: " << e.what() << endl;
            }

        } while (vbr != 0);

    } catch (const cipher_error& e) {
        cerr << "Ошибка инициализации шифра: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
