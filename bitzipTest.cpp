#define _CRT_SECURE_NO_WARNINGS


// 基于Huffman树
//#include<iostream>
//using namespace std;
//#include"HuffmanTree.h"
#include"FileCompressHuffman.h"

void menu() {
	cout << "**********************************************" << endl;
	cout << "***  0.退出                         **********" << endl;
	cout << "***  1.Huffman压缩                  **********" << endl;
	cout << "***  2.Huffman解压缩                **********" << endl;
	cout << "**********************************************" << endl;
}

int main() {
	//TestHuffmanTree();

	FileCompressHM fc;
	int input = 0;
	bool isQuit = false;
	string fileName;

	while (true)
	{
		menu();
		cin >> input;
		switch (input)
		{
		case 0: 
			isQuit = true;
			break;
		case 1:
			cout << "输入压缩文件名称:";
			cin >> fileName;
			fc.CompressFile(fileName);
			break;

		case 2:
			cout << "输入解压缩文件名称:";
			cin >> fileName;
			fc.UnCompressFile(fileName);
			break;

		default:
			cout << "输入错误";
			break;
		}

		if (isQuit)
			break;

	}

	//FileCompressHM fc;
	//fc.CompressFile("111.txt");
	//fc.UnCompressFile("2.hz");


	// char: -128 -- 127
	//const char* p = "你好";
	// 调试框里输入p,5 查看
	return 0;
}