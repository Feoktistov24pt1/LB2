#include <iostream>
#include <locale>
#include <wctype.h>
#include "modAlphaCipher.h"

std::wstring filterRussian(const std::wstring& s) {
    static const std::wstring russianAlphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::wstring result;
    for (wchar_t c : s) {
        wchar_t upperC = std::towupper(c);
        if (russianAlphabet.find(upperC) != std::wstring::npos) {
            result += upperC;
        }
    }
    return result;
}

int main()
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::wstring keyInput;
    std::wcout << L"Программа шифрования\n";
    std::wcout << L"Введите ключ: ";
    std::getline(std::wcin, keyInput);

    std::wstring key = filterRussian(keyInput);
    if (key.empty()) {
        std::wcerr << L"Ключ не из русских букв!";
        return 1;
    }

    modAlphaCipher cipher(key);

    unsigned op;
    do {
        std::wcout << L"Выберите действие (Выйти=>0, Шифровать=>1, Расшифровать=>2): ";
        std::wcin >> op;
        std::wcin.ignore(); // очистка буфера

        if (op == 0) {
            break;
        }

        if (op != 1 && op != 2) {
            std::wcout << L"Неверный выбор!\n";
            continue;
        }

        std::wstring textInput;
        std::wcout << L"Введите текст: ";
        std::getline(std::wcin, textInput);

        std::wstring text = filterRussian(textInput);
        if (text.empty()) {
            std::wcout << L"Текст не содержит русских букв!\n";
            continue;
        }

        try {
            if (op == 1) {
                std::wstring encrypted = cipher.encrypt(text);
                std::wcout << L"Зашифрованный текст: " << encrypted << L"\n";
            } else {
                std::wstring decrypted = cipher.decrypt(text);
                std::wcout << L"Расшифрованный текст: " << decrypted << L"\n";
            }
        } catch (const std::exception& e) {
            std::wcerr << L"Ошибка: " << e.what() << L"\n";
        }

    } while (true);

    return 0;
}
