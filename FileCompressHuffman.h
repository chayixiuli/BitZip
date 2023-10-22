#pragma once
#include"HuffmanTree.h"
#include"Common.h"
#include<string>
//#include<vector>
using std::string;
using std::vector;

struct ByteInfo
{
	//char _ch; 
	uch _ch;
	size_t _appearCount;
	std::string _chCode;
	ByteInfo(size_t appearCount = 0)
		:_appearCount(appearCount)
	{}

	ByteInfo operator+(const ByteInfo& other) const{
		return ByteInfo(this->_appearCount + other._appearCount);
	}
	bool operator>(const ByteInfo& other) const {
		return this->_appearCount > other._appearCount;
	}
	bool operator!=(const ByteInfo& other) const {
		return this->_appearCount != other._appearCount;
	}
};

class FileCompressHM {
public:
	FileCompressHM();

	void CompressFile(const string& filePath);

	void UnCompressFile(const string& filePath);

private:

	void GenerateHuffmanCode(HuffmanTreeNode<ByteInfo>* root);
	
	void WriteHeadInfo(const string& filePath,FILE* fOut);
	
	string GetFilePostFix(const string& filePath);
	void GetLine(FILE* fIn, string& strInfo);

	vector<ByteInfo> _fileInfo;

};
