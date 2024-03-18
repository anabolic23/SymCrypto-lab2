#include <iostream>
#include "vigenere_cipher.h"
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "");

    Tools tool;
    tool.OpenFile("../input.txt");
    tool.ProcessTextFile(); 
    tool.CloseFile();

    std::vector<std::wstring> keys = {
        L"��", L"���", L"����", L"�����", L"���������������"
    };

    for (auto& key : keys) {
        tool.ProcessAndEncodeTextFile(key);
    }

    tool.CalculateAndCompareIoCs();
    
    return 0;
}