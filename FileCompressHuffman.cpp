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
	//1.ͳ��Դ�ļ���ÿ���ֽڳ��ֵĴ���
	//FILE* fIn = fopen(filePath.c_str(), "r"); // �ı���ʽ��ȡ������ 0xff ����Ϊ����ĩβ
	FILE* fIn = fopen(filePath.c_str(), "rb");
	if (nullptr == fIn) {
		cout << "��ѹ���ļ�������" << endl;
		return;
	}

	//char rdBuff[1024]; // ������Щ���Ǹ��� rdBuff[i]
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

	//2.��ͳ�ƵĽ�� ������Huffman��
	HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());

	// ��ȡHuffman����
	GenerateHuffmanCode(ht.GetRoot());

	// д������ѹ����������Ϣ
	FILE* fOut = fopen("2.hz", "wb");
	WriteHeadInfo(filePath, fOut);

	// �û�ȡ���ı����Դ�ļ����и�д

	//char bits = 0;
	uch bits = 0;
	int bitCount = 0;
	fseek(fIn, 0, SEEK_SET); // �ص��ļ���ͷ 
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
	// ע��:���һ��bits�е�8������λ����û�������
	if (bitCount > 0 && bitCount < 8) {
		bits <<= (8 - bitCount);
		fputc(bits, fOut);
	}
	fclose(fIn);
	fclose(fOut);


}

void FileCompressHM::WriteHeadInfo(const string& filePath, FILE* fOut) {
	// ��ȡԴ�ļ���׺
	string headInfo;
	headInfo += GetFilePostFix(filePath);
	headInfo += '\n';

	// ����Ƶ����Ϣ
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

	// root����Ҷ�ӽ��
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
		cout << "ѹ���ļ���ʽ����" << endl;
		return;
	}
	
	// ��ȡ��ѹ����Ҫ�õ�����Ϣ
	FILE* fIn = fopen(filePath.c_str(),"rb");
	if (nullptr == fIn) {
		cout << "��ѹ���ļ�·������" << endl;
		return;
	}

	// ��ȡԴ�ļ���׺
	string unCompressFile("222.");
	string strInfo;
	GetLine(fIn, strInfo);
	unCompressFile += strInfo;

	// ��ȡƵ����Ϣ������
	strInfo = "";
	GetLine(fIn, strInfo);
	size_t lineCount = atoi(strInfo.c_str());

	for (size_t i = 0; i < lineCount; ++i) {
		strInfo = "";
		GetLine(fIn, strInfo);
		// ע��:������Ҫ���⴦��
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

	// ��ԭHuffman��
	HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());

	// ��ѹ��
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
	while (!feof(fIn)) { // û�ж����ļ���β�ͼ���
		char ch = fgetc(fIn);
		if (ch == '\n')
			break;
		strInfo += ch;
	}
}
