#define _CRT_SECURE_NO_WARNINGS


// ����Huffman��
//#include<iostream>
//using namespace std;
//#include"HuffmanTree.h"
#include"FileCompressHuffman.h"

void menu() {
	cout << "**********************************************" << endl;
	cout << "***  0.�˳�                         **********" << endl;
	cout << "***  1.Huffmanѹ��                  **********" << endl;
	cout << "***  2.Huffman��ѹ��                **********" << endl;
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
			cout << "����ѹ���ļ�����:";
			cin >> fileName;
			fc.CompressFile(fileName);
			break;

		case 2:
			cout << "�����ѹ���ļ�����:";
			cin >> fileName;
			fc.UnCompressFile(fileName);
			break;

		default:
			cout << "�������";
			break;
		}

		if (isQuit)
			break;

	}

	//FileCompressHM fc;
	//fc.CompressFile("111.txt");
	//fc.UnCompressFile("2.hz");


	// char: -128 -- 127
	//const char* p = "���";
	// ���Կ�������p,5 �鿴
	return 0;
}