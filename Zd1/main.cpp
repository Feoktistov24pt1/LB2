#include <iostream>
#include <locale>
#include <wctype.h>
#include <codecvt>
#include "modAlphaCipher.h"
using namespace std;

wstring strt(const string& s)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    wstring keyInput;
    wstring textInput;
    unsigned op;

    wcout << L"Программа шифрования\n";
    wcout << L"Введите ключ: ";
    getline(wcin, keyInput);

    try {
        modAlphaCipher cipher(keyInput);

        do {
            wcout << L"Шифр готов. Выберите действие (Выйти=>0, Шифровать=>1, Расшифровать=>2): ";
            wcin >> op;
            wcin.ignore();

            if (op == 0) break;
            if (op > 2) {
                wcout << L"Неверное действие!" << endl;
                continue;
            }

            wcout << L"Введите текст: ";
            getline(wcin, textInput);

            try {
                if (op == 1) {
                    wstring encrypted = cipher.encrypt(textInput);
                    wcout << L"Зашифрованный текст: " << encrypted << endl;
                } else {
                    wstring decrypted = cipher.decrypt(textInput);
                    wcout << L"Расшифрованный текст: " << decrypted << endl;
                }
            } catch (const cipher_error& e) {
                wcerr << L"Ошибка обработки текста: " << strt(e.what()) << endl;
            }
        } while (op != 0);

    } catch (const cipher_error& e) {
        wcerr << L"Ошибка инициализации шифра: " << strt(e.what()) << endl;
        return 1;
    }

    return 0;
}
