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
        L"шо", L"фес", L"куда", L"исчез", L"пропалбезвести"
    };

    for (auto& key : keys) {
        tool.ProcessAndEncodeTextFile(key);
    }

    tool.CalculateAndCompareI();

    auto text = tool.ReadFile("../encoded.txt");
    for (int i = 2; i < 41; i++) {
        auto key = tool.CrackKeyMi(text, i);
        std::wcout << "i:" << i << " key: " << key << " \\\\ " << std::endl;
    }
    
    size_t keyLength = tool.DetermineKeyLength("../encoded.txt");
    std::cout << "Determined key length: " << keyLength << std::endl;

    std::wstring key = tool.DetermineKeyByMostFrequentLetters(text, keyLength);
    std::wcout << L"Key determined by most frequent letters: " << key << std::endl;

    std::wstring properkey = L"улановсеребряныепули";
    auto decodedText = tool.DecodeVigenere(text, properkey);
    std::wcout << L"Decoded Text: " << decodedText << std::endl;

    tool.ProcessAndDecodeTextFile(properkey);

    return 0;
}
