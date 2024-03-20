#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cctype> 
#include <locale>
#include <codecvt>
#include <map>
#include <regex>
#include <vector>
#include <stdlib.h>

class Tools {
private:
	std::wstring alphabet;
	std::wifstream inputFile;
	std::wofstream outputFile;
	std::string inputFileName;

public:
	Tools();
	void OpenFile(const std::string& inputFileName);
	void CloseFile();
	void ProcessTextFile();
	wchar_t ToLowerCase(wchar_t c);
	std::wstring ProcessLine(const std::wstring& line);

	std::wstring EncodeVigenere(const std::wstring& text, const std::wstring& key);
	void ProcessAndEncodeTextFile(const std::wstring& key);

	double CalculateI(const std::string& fileName);
	void CalculateAndCompareI();

	std::wstring ReadFile(const std::string& inputFileName);
	std::vector<std::wstring> DivideIntoBlocks(const std::wstring& text, size_t blockSize);
	std::wstring CrackKeyMi(const std::wstring& text, size_t key_length);
	size_t DetermineKeyLength(const std::string& inputfileName);
	double CalculateIForBlocks(const std::vector<std::wstring>);
	double CalculateExpectedI(const std::vector<double>& probabilities);
	std::wstring DetermineKeyByMostFrequentLetters(const std::wstring& text, size_t keyLength);
	std::wstring DecodeVigenere(const std::wstring& encodedText, const std::wstring& key);
	void ProcessAndDecodeTextFile(const std::wstring& key);
};

