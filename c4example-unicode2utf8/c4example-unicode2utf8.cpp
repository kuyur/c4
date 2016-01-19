/************************************************************************/
/*                                                                      */
/* c4-lib example                                                       */
/* This project is for showing how to use c4-lib.                       */
/*                                                                      */
/* Version: 0.1                                                         */
/* Author:  wei_w (weiwl07@gmail.com)                                   */
/* Published under Apache License 2.0                                   */
/* http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                      */
/* Copyright 2016 Project c4                                            */
/* https://github.com/kuyur/c4                                          */
/*                                                                      */
/************************************************************************/

// c4example-unicode2utf8.cpp : Defines the entry point for the console application.
//

#pragma once

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include "convertMethod.h"
#include "../c4/c4encode.h"
#ifdef _DEBUG
#pragma comment(lib, "../Debug/c4.lib")
#else
#pragma comment(lib, "../Release/c4.lib")
#endif

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string filename = "../testfiles/utf-16/bungakusyoujyo-unicode.txt";
	ifstream infile(filename.c_str(), ios::in|ios::binary);
	if (!infile)
	{
		cerr<<"unable to open input file!\n";
		return -1;
	}
	ofstream outfile("../testfiles/out/bungakusyoujyo-utf-8.txt", ios::binary);
	if (!outfile)
	{
		cerr<<"unable to open output file\n";
		infile.close();
		return -1;
	}

	infile.seekg(0, ios::end);
	unsigned int rawLength = infile.tellg();
	char *rawStringBuffer = new char[rawLength+1];
	memset((void*)rawStringBuffer, 0, rawLength+1);
	infile.seekg(0, 0);
	infile.read(rawStringBuffer, rawLength);
	char *stringBuffer = rawStringBuffer + 2;
	unsigned int length = rawLength - 2;

	bool isLittleEndian;
	if (((unsigned char)rawStringBuffer[0]==0xFF)&&((unsigned char)rawStringBuffer[1]==0xFE))
		isLittleEndian = true;
	else
		isLittleEndian = false;

	outfile.write(CC4Encode::UTF_8_BOM,3);

	if (match(stringBuffer, length))
	{
		cout<<"Unicode matched!"<<endl;
		// output

		long beginTime, endTime;

		/*method 1*/
		beginTime = clock();
		string& resultBuffer1 = convert2utf8_pushback(stringBuffer, length, isLittleEndian);
		endTime = clock();
		cout<<"utf-8 string length: "<<resultBuffer1.length()<<endl;
		cout<<"pushback method costs time: "<<endTime - beginTime<<"ms"<<endl;

		/*method 2*/
		beginTime = clock();
		string& resultBuffer2 = convert2utf8_copy(stringBuffer, length, isLittleEndian);
		endTime = clock();
		cout<<"utf-8 string length: "<<resultBuffer2.length()<<endl;
		cout<<"copy construct method costs time: "<<endTime - beginTime<<"ms"<<endl;

		/*method 3*/
		beginTime = clock();
		string& resultBuffer3 = convert2utf8_allocate(stringBuffer, length, isLittleEndian);
		endTime = clock();
		cout<<"utf-8 string length: "<<resultBuffer3.length()<<endl;
		cout<<"allocate method costs time: "<<endTime - beginTime<<"ms"<<endl;

		/*method 4*/
		beginTime = clock();
		string& resultBuffer4 = CC4EncodeUTF16::convert2utf8(stringBuffer, length, isLittleEndian);
		endTime = clock();
		cout<<"utf-8 string length: "<<resultBuffer4.length()<<endl;
		cout<<"allocate method (CC4EncodeUTF16) costs time: "<<endTime - beginTime<<"ms"<<endl;

		/*method 5*/
		beginTime = clock();
		unsigned int dst_length = CC4EncodeUTF16::calcUtf8StringLength(stringBuffer, length, isLittleEndian);
		char *dst = new char[dst_length];
		bool result = CC4EncodeUTF16::convert2utf8(stringBuffer, length, dst, dst_length, isLittleEndian);
		endTime = clock();
		if (!result)
			cerr<<"Fail to convert to utf-8\n";
		else
		{
			cout<<"utf-8 string length: "<<dst_length<<endl;
			cout<<"memory copy method (CC4EncodeUTF16) costs time: "<<endTime - beginTime<<"ms"<<endl;
			/*writing method 1*/
			beginTime = clock();
			outfile.write(dst, dst_length);
			endTime = clock();
			cout<<"writing file by pointer costs time: "<<endTime - beginTime<<"ms"<<endl;
		}
		delete []dst;
		dst = NULL;

		/*writing method 2*/
		/*
		const char *p = resultBuffer1.c_str();
		unsigned int resultLength = resultBuffer1.length();
		beginTime = clock();
		outfile.write(p, resultLength);
		endTime = clock();
		cout<<"writing file by pointer costs time: "<<endTime - beginTime<<"ms"<<endl;
		*/
		
		/*writing method 3*/
		/*
		beginTime = clock();
		outfile<<resultBuffer4;
		endTime = clock();
		cout<<"writing file by stream costs time: "<<endTime - beginTime<<"ms"<<endl;
		*/
	}

	infile.close();
	outfile.close();
	delete []rawStringBuffer;
	rawStringBuffer = NULL;
	stringBuffer = NULL;

	CC4EncodeUTF16 *unicodeEncode = CC4EncodeUTF16::getInstance();
	if (unicodeEncode != NULL)
	{
		cout<<"get Unicode Encode instance success!"<<endl;
		cout<<"address: "<<unicodeEncode<<endl;
		unicodeEncode = CC4EncodeUTF16::getInstance();
		cout<<"address: "<<unicodeEncode<<endl;
		unicodeEncode = NULL;
	}

	return 0;
}