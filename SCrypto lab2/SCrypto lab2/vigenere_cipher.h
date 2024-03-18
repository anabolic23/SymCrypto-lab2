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

	double CalculateIoC(const std::string& fileName);
	void CalculateAndCompareIoCs();
};

