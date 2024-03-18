#include "vigenere_cipher.h"
#include <iostream>

Tools::Tools() {
    alphabet = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
}

void Tools::OpenFile(const std::string& inputFileName) {
    this->inputFileName = inputFileName; // Store input file name
    inputFile.open(this->inputFileName);

    inputFile.imbue(std::locale(inputFile.getloc(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));


    if (!inputFile) {
        std::cerr << "Error: Unable to open the file '" << this->inputFileName << "'\n";
    }
    else {
        std::cout << "File " << this->inputFileName << " opened successfully!\n";
    }
}

void Tools::CloseFile() {
    inputFile.close();
    std::cout << "File closed successfully!\n";
}

wchar_t Tools::ToLowerCase(wchar_t c) {
    if (c >= L'А' && c <= L'Я') {
        return c + (L'а' - L'А');
    }

    else if (c == L'Ё') {
        return L'е'; // Special case for 'Ё'
    }

    return c;
}


std::wstring Tools::ProcessLine(const std::wstring& line) {
    std::wstring result;
    for (auto c : line) {
        c = ToLowerCase(c); // Convert to lowercase first

        if (c == L'ъ') {
            c = L'ь';
        }
        if (c == L'ё') {
            c = L'е';
        }

        // Check if the character is in the modified alphabet or it's a space
        if (alphabet.find(c) != std::wstring::npos) {
            result += c;
        }
    }

    return result;
}

void Tools::ProcessTextFile() {
    outputFile.open("output.txt");
    outputFile.imbue(std::locale(inputFile.getloc(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));
    if (!outputFile) {
        std::cerr << "Error: Unable to create the file 'output.txt'\n";
        return;
    }

    std::wstring line;
    while (std::getline(inputFile, line)) {
        std::wstring processedLine = ProcessLine(line);
        //std::wcout << processedLine;
        outputFile << processedLine;
    }
    inputFile.close();
    outputFile.close(); 
}

std::wstring Tools::EncodeVigenere(const std::wstring& text, const std::wstring& key) {
    std::wstring result;
    //int alphaLen = alphabet.length();
    int keyLen = key.length();

    for (size_t i = 0, j = 0; i < text.length(); ++i) {
        wchar_t c = text[i];
        if (alphabet.find(c) != std::wstring::npos) {
            result += alphabet[(alphabet.find(c) + alphabet.find(key[j])) % 31];
            j = (j + 1) % keyLen;
        }
        else {
            result += c;
        }
    }
    return result;
}

void Tools::ProcessAndEncodeTextFile(const std::wstring& key) {
    std::wifstream inputFile("output.txt");
    std::wofstream outputFile("encoded_" + std::to_string(key.length()) + ".txt");

    inputFile.imbue(std::locale(inputFile.getloc(), new std::codecvt_utf8<wchar_t>));
    outputFile.imbue(std::locale(inputFile.getloc(), new std::codecvt_utf8<wchar_t>));

    std::wstring line;
    while (std::getline(inputFile, line)) {
        std::wstring encodedLine = EncodeVigenere(line, key);
        outputFile << encodedLine << std::endl;
    }

    inputFile.close();
    outputFile.close();
}

double Tools::CalculateIoC(const std::string& fileName) {
    std::wifstream file(fileName);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

    std::map<wchar_t, int> frequencies;
    int totalChars = 0;
    std::wstring line;

    while (std::getline(file, line)) {
        for (wchar_t ch : line) {
            if (alphabet.find(ch) != std::wstring::npos) {
                frequencies[ch]++;
                totalChars++;
            }
        }
    }

    file.close();

    double sum = 0.0;
    for (const auto& pair : frequencies) {
        sum += pair.second * (pair.second - 1);
    }

    return sum / (totalChars * (totalChars - 1));
}

void Tools::CalculateAndCompareIoCs() {
    double openTextIoC = CalculateIoC("output.txt");
    std::wcout << L"Index of Coincidence for open text: " << openTextIoC << std::endl;

    std::vector<int> keyLengths = { 2, 3, 4, 5, 15 };
    for (int len : keyLengths) {
        std::string fileName = "encoded_" + std::to_string(len) + ".txt";
        double encodedTextIoC = CalculateIoC(fileName);
        std::wcout << L"Index of Coincidence for encoded text with key length " << len << L": " << encodedTextIoC << std::endl;
    }
}