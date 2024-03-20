#include "vigenere_cipher.h"
#include <iostream>

Tools::Tools() {
    alphabet = L"абвгдежзийклмнопрстуфхцчшщъыьэюя";
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
    size_t keyLen = key.length();

    for (size_t i = 0, j = 0; i < text.length(); ++i) {
        wchar_t c = text[i];
        if (alphabet.find(c) != std::wstring::npos) {
            result += alphabet[(alphabet.find(c) + alphabet.find(key[j])) % alphabet.size()];
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

double Tools::CalculateI(const std::string& fileName) {
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

    float sum = 0.0;
    for (const auto& pair : frequencies) {
        sum += pair.second * (pair.second - 1);
    }

    return sum / (totalChars * (totalChars - 1));
}

void Tools::CalculateAndCompareI() {
    double openTextIoC = CalculateI("output.txt");
    std::wcout << L"Index of Coincidence for open text: " << openTextIoC << std::endl;

    std::vector<int> keyLengths = { 2, 3, 4, 5, 14 };
    for (int len : keyLengths) {
        std::string fileName = "encoded_" + std::to_string(len) + ".txt";
        double encodedTextIoC = CalculateI(fileName);
        std::wcout << L"Index of Coincidence for encoded text with key length " << len << L": " << encodedTextIoC << std::endl;
    }
}

const std::vector<double> probabilities = {
    0.0726005, 0.0197271, 0.0444918, 0.0159125, 0.0289677, 0.0920283, 0.0143712,
    0.0154028, 0.0698403, 0.00995388, 0.0252744, 0.0407459, 0.0349896, 0.0691608, 0.11012,
    0.026701, 0.0387058, 0.0544389, 0.067927, 0.0279618, 0.000836008, 0.00783285, 0.00279658,
    0.0183153, 0.00814029, 0.0050511, 0.00036, 0.0195895, 0.0221974, 0.00350449, 0.00754295, 0.0248726
};  

std::wstring Tools::ReadFile(const std::string& inputFileName) {
    std::wifstream encodedInputFile(inputFileName);
    encodedInputFile.imbue(std::locale(encodedInputFile.getloc(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));

    if (!encodedInputFile.is_open()) {
        std::wcerr << L"Failed to open file: " << std::wstring(inputFileName.begin(), inputFileName.end()) << std::endl;
        return L"";
    }

    std::wstring line;
    std::wstring encodedText;

    while (std::getline(encodedInputFile, line)) {
        encodedText += line;
    }

    encodedInputFile.close();
    return encodedText;
}

std::vector<std::wstring> Tools::DivideIntoBlocks(const std::wstring& text, size_t keyLength) {
    std::vector<std::wstring> blocks(keyLength);
    for (size_t i = 0; i < text.length(); ++i) {
        blocks[i % keyLength] += text[i];/*
        std::wcout << blocks[i % keyLength];*/
    }
    return blocks;
}

std::wstring Tools::CrackKeyMi(const std::wstring& text, size_t key_length) {
    auto blocks = DivideIntoBlocks(text, key_length);
    if (blocks.empty()) {
        return L"Error: Text is empty.";
    }
    if (blocks.size() < key_length) {
        return L"Error: Key length exceeds the number of blocks.";
    }

    std::wstring key;
    for (size_t block_index = 0; block_index < key_length; ++block_index) {
        size_t max_shift = 0;
        double max_m = -1.0; // Ensuring any positive match will be greater.

        for (size_t g = 0; g < alphabet.size(); ++g) {
            double current_m = 0.0;
            for (size_t t = 0; t < alphabet.size(); ++t) {
                size_t shift_index = (t + g) % alphabet.size();
                wchar_t shifted_char = alphabet[shift_index];
                size_t char_count = 0;
                for (wchar_t ch : blocks[block_index]) {
                    if (ch == shifted_char) {
                        ++char_count;
                    }
                }
                current_m += probabilities[t] * char_count;
            }

            if (current_m > max_m) {
                max_m = current_m;
                max_shift = g;
            }
        }
        key.push_back(alphabet[max_shift]);
    }

    return key;
}

size_t Tools::DetermineKeyLength(const std::string& fileName) {
    std::wifstream encodedInputFile(fileName);
    encodedInputFile.imbue(std::locale(encodedInputFile.getloc(),
        new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>));

    Tools tool;
    auto text = tool.ReadFile(fileName);
    double averageIoC = 0.0;
    size_t keyLength = 0;
    double expectedI = tool.CalculateExpectedI(probabilities);

    for (size_t len = 1; len <= 40; ++len) {
        double IoC = tool.CalculateIForBlocks(tool.DivideIntoBlocks(text, len));
        if (abs(expectedI - IoC) < abs(expectedI - averageIoC)) {
            averageIoC = IoC;
            keyLength = len;
        }
        /*std::cout << IoC << std::endl;*/
    }

    return keyLength;
}

double Tools::CalculateIForBlocks(std::vector<std::wstring> blocks) {
    double sums = 0;
    int size = blocks.size();
    for (auto& block : blocks) {
        std::map<wchar_t, int> frequencies;
        int totalChars = 0;

        for (wchar_t ch : block) {
            if (alphabet.find(ch) != std::wstring::npos) {
                frequencies[ch]++;
                totalChars++;
            }
        }

        float sum = 0.0;
        for (const auto& pair : frequencies) {
            sum += pair.second * (pair.second - 1);
        }

        sum /= (totalChars * (totalChars - 1));
        sums += sum;
    }
    return sums/size;
}

double Tools::CalculateExpectedI(const std::vector<double>& probabilities) {
    double expectedI = 0.0;
    for (auto p : probabilities) {
        expectedI += p * p;
    }
    return expectedI;
}

std::wstring Tools::DetermineKeyByMostFrequentLetters(const std::wstring& text, size_t keyLength) {
    std::wstring key;
    auto blocks = DivideIntoBlocks(text, keyLength);

    wchar_t mostFrequentLetter = L'о'; // The most frequent letter in Russian
    for (auto& block : blocks) {
        std::map<wchar_t, size_t> frequencies;
        for (wchar_t ch : block) {
            frequencies[ch]++;
        }

        wchar_t maxChar = 0;
        size_t maxFreq = 0;
        for (auto& pair : frequencies) {
            if (pair.second > maxFreq) {
                maxFreq = pair.second;
                maxChar = pair.first;
            }
        }

        size_t charPosition = alphabet.find(maxChar);
        size_t mostFrequentLetterPosition = alphabet.find(mostFrequentLetter);
        size_t keyCharPosition = (charPosition + alphabet.size() - mostFrequentLetterPosition) % alphabet.size();
        key.push_back(alphabet[keyCharPosition]);
    }

    return key;
}

std::wstring Tools::DecodeVigenere(const std::wstring& encodedText, const std::wstring& key) {

    std::wstring result;
    size_t keyLen = key.length();

    for (size_t i = 0, j = 0; i < encodedText.length(); ++i) {
        wchar_t c = encodedText[i];
        if (alphabet.find(c) != std::wstring::npos) {
            size_t encodedIndex = alphabet.find(c);
            size_t keyIndex = alphabet.find(key[j]);
            size_t newIndex = (encodedIndex + alphabet.size() - keyIndex) % alphabet.size();
            result += alphabet[newIndex];
            j = (j + 1) % keyLen;
        }
        else {
            result += c;
        }
    }

    return result;
}

void Tools::ProcessAndDecodeTextFile(const std::wstring& key) {
    std::wifstream inputFile("../encoded.txt");
    std::wofstream outputFile("decoded.txt");

    inputFile.imbue(std::locale(inputFile.getloc(), new std::codecvt_utf8<wchar_t>));
    outputFile.imbue(std::locale(inputFile.getloc(), new std::codecvt_utf8<wchar_t>));

    std::wstring line;
    while (std::getline(inputFile, line)) {
        std::wstring decodedLine = DecodeVigenere(line, key);
        outputFile << decodedLine << std::endl;
    }

    inputFile.close();
    outputFile.close();
}