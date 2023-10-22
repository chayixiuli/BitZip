#define _CRT_SECURE_NO_WARNINGS
#include"FileCompressHuffman.h"
//#include"Common.h"


FileCompressHM::FileCompressHM() {
	_fileInfo.resize(256);
	for (int i = 0; i < 256; ++i) {
		_fileInfo[i]._ch = i;  // A -> 65 a -> 97
	}
}

void FileCompressHM::CompressFile(const string& filePath) {
	//1.统计源文件中每个字节出现的次数
	//FILE* fIn = fopen(filePath.c_str(), "r"); // 文本方式读取，读到 0xff 会以为读到末尾
	FILE* fIn = fopen(filePath.c_str(), "rb");
	if (nullptr == fIn) {
		cout << "待压缩文件不存在" << endl;
		return;
	}

	//char rdBuff[1024]; // 中文有些会是负数 rdBuff[i]
	uch rdBuff[1024];
	while(true) {
		size_t rdSize = fread(rdBuff, 1, 1024, fIn);
		if (0 == rdSize) {
			break;
		}
		for (size_t i = 0; i < rdSize; i++)
		{

			_fileInfo[rdBuff[i]]._appearCount++;
		}
	}

	//2.用统计的结果 来创建Huffman树
	HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());

	// 获取Huffman编码
	GenerateHuffmanCode(ht.GetRoot());

	// 写用来解压缩的数据信息
	FILE* fOut = fopen("2.hz", "wb");
	WriteHeadInfo(filePath, fOut);

	// 用获取到的编码对源文件进行改写

	//char bits = 0;
	uch bits = 0;
	int bitCount = 0;
	fseek(fIn, 0, SEEK_SET); // 回到文件开头 
	while (true)
	{
		size_t rdSize = fread(rdBuff, 1, 1024, fIn);
		if (0 == rdSize)
			break;
		for (size_t i = 0; i < rdSize; ++i) {
			string& strCode = _fileInfo[rdBuff[i]]._chCode;
			for (size_t j = 0; j < strCode.size(); ++j) {
				bits <<= 1;
				if ('1' == strCode[j]) {
					bits |= 1;
				}
				bitCount++;
				if (8 == bitCount) {
					fputc(bits, fOut);
					bits = 0;
					bitCount = 0;
				}
			}
		}
	}
	// 注意:最后一次bits中的8个比特位可能没有填充满
	if (bitCount > 0 && bitCount < 8) {
		bits <<= (8 - bitCount);
		fputc(bits, fOut);
	}
	fclose(fIn);
	fclose(fOut);


}

void FileCompressHM::WriteHeadInfo(const string& filePath, FILE* fOut) {
	// 获取源文件后缀
	string headInfo;
	headInfo += GetFilePostFix(filePath);
	headInfo += '\n';

	// 构造频次信息
	size_t appearLineCount = 0;
	string charInfo;
	for (auto& e : _fileInfo) {
		if (0 == e._appearCount) 
			continue;
		charInfo += e._ch;
		charInfo += ':';
		charInfo += std::to_string(e._appearCount);
		charInfo += '\n';

		appearLineCount++;
	}

	headInfo += std::to_string(appearLineCount);
	headInfo += '\n';
	fwrite(headInfo.c_str(), 1, headInfo.size(), fOut);
	fwrite(charInfo.c_str(), 1, charInfo.size(), fOut);
}

string FileCompressHM::GetFilePostFix(const string& filePath) {
	return filePath.substr(filePath.find_last_of(".") + 1);
}


void FileCompressHM::GenerateHuffmanCode(HuffmanTreeNode<ByteInfo>* root) {
	if (nullptr == root) {
		return;
	}
	GenerateHuffmanCode(root->_left);
	GenerateHuffmanCode(root->_right );

	// root就是叶子结点
	if (nullptr == root->_left && nullptr == root->_right) {
		string& chCode = _fileInfo[root->_weight._ch]._chCode;
		HuffmanTreeNode<ByteInfo>* cur = root;
		HuffmanTreeNode<ByteInfo>* parent = cur->_parent;
		while (parent)
		{
			if (cur == parent->_left)
				chCode += '0';
			else
				chCode += '1';

			cur = parent;
			parent = parent->_parent;
		}
		reverse(chCode.begin(), chCode.end());
	}
}


void FileCompressHM::UnCompressFile(const string& filePath) {
	if (GetFilePostFix(filePath) != "hz") 
	{
		cout << "压缩文件格式不对" << endl;
		return;
	}
	
	// 读取解压缩需要用到的信息
	FILE* fIn = fopen(filePath.c_str(),"rb");
	if (nullptr == fIn) {
		cout << "解压缩文件路径不对" << endl;
		return;
	}

	// 读取源文件后缀
	string unCompressFile("222.");
	string strInfo;
	GetLine(fIn, strInfo);
	unCompressFile += strInfo;

	// 读取频次信息总行数
	strInfo = "";
	GetLine(fIn, strInfo);
	size_t lineCount = atoi(strInfo.c_str());

	for (size_t i = 0; i < lineCount; ++i) {
		strInfo = "";
		GetLine(fIn, strInfo);
		// 注意:换行需要特殊处理
		if ("" == strInfo) {
			strInfo += '\n';
			GetLine(fIn, strInfo);
		}
		// A:1 -> _fileInfo;
		uch ch = strInfo[0];
		//_fileInfo[strInfo[0]]._ch = strInfo[0];
		//_fileInfo[strInfo[0]]._appearCount = atoi(strInfo.c_str() + 2);
		_fileInfo[ch]._ch = strInfo[0];
		_fileInfo[ch]._appearCount = atoi(strInfo.c_str() + 2);
	}

	// 还原Huffman树
	HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());

	// 解压缩
	FILE* fOut = fopen(unCompressFile.c_str(), "wb");
	//char rdBuff[1024];
	uch rdBuff[1024];
	HuffmanTreeNode<ByteInfo>* cur = ht.GetRoot();
	size_t fileSize = 0;

	while (true)
	{
		size_t rdSize = fread(rdBuff, 1, 1024, fIn);
		if (0 == rdSize)
			break;

		for (size_t i = 0; i < rdSize; ++i) {
			char ch = rdBuff[i];
			for (int j = 0; j < 8; ++j) {
				if (ch & 0x80)
					cur = cur->_right;
				else
					cur = cur->_left;

				ch <<= 1;

				if (nullptr == cur->_right && nullptr == cur->_left)
				{
					fputc(cur->_weight._ch, fOut);
					cur = ht.GetRoot();
					fileSize += 1;
					if (fileSize == cur->_weight._appearCount)
						break;
				}
			}
		}
	}
	fclose(fIn);
	fclose(fOut);

}

void FileCompressHM::GetLine(FILE* fIn, string& strInfo) {
	while (!feof(fIn)) { // 没有读到文件结尾就继续
		char ch = fgetc(fIn);
		if (ch == '\n')
			break;
		strInfo += ch;
	}
}
