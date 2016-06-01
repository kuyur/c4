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

// c4example-utf8tounicode.cpp : Defines the entry point for the console application.
//
#pragma once

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include "../c4/c4encode.h"
#ifdef _DEBUG_MD
#pragma comment(lib, "../build/dll/debug/c4.lib")
#endif
#ifdef _DEBUG_MT
#pragma comment(lib, "../build/static/debug/c4.lib")
#endif
#ifdef _RELEASE_MD
#pragma comment(lib, "../build/dll/release/c4.lib")
#endif
#ifdef _RELEASE_MT
#pragma comment(lib, "../build/static/release/c4.lib")
#endif

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string filename = "../testfiles/utf-8/wukongzhuang-utf-8.txt";
	ifstream infile(filename.c_str(), ios::in|ios::binary);
	if (!infile)
	{
		cerr<<"unable to open input file!\n";
		return -1;
	}
	ofstream outfile("../testfiles/out/wukongzhuang-unicode.txt", ios::binary);
	if (!outfile)
	{
		cerr<<"unable to open output file\n";
		infile.close();
		return -1;
	}

	infile.seekg(0, ios::end);
	unsigned int length = infile.tellg();
	char * stringBuffer = new char[length+1];
	memset((void*)stringBuffer, 0, length+1);
	infile.seekg(0, 0);
	infile.read(stringBuffer, length);

	outfile.write(CC4Encode::LITTLEENDIAN_BOM, 2);
	// or
	// outfile.write((char*)&(CC4Encode::LITTLEENDIAN_MARK),2);

	if (CC4EncodeUTF8::_match(stringBuffer, length))
	{
		cout<<"utf-8 matched!"<<endl;

		// output
		/*
		wstring resultBuffer = CC4EncodeUTF8::convert2unicode(stringBuffer, length);
		cout<<"length: "<<resultBuffer.length()<<endl;
		const wchar_t *p = resultBuffer.c_str();
		unsigned int resultLength = resultBuffer.length();
		outfile.write((const char*)p, resultLength*sizeof(wchar_t));
		*/
		// another method to convet
		unsigned int resultLenth = CC4EncodeUTF8::calcUnicodeStringLength(stringBuffer, length);
		wchar_t *resultBuffer = new wchar_t[resultLenth];
		CC4EncodeUTF8::convert2unicode(stringBuffer, length, resultBuffer, resultLenth,false);
		cout<<"length: "<<resultLenth<<endl;
		outfile.write((char*)resultBuffer, resultLenth*sizeof(wchar_t));
		delete []resultBuffer;
	}
	
	infile.close();
	outfile.close();
	delete []stringBuffer;

	const CC4EncodeUTF8 *utf8Encode = CC4EncodeUTF8::getInstance();
	if (utf8Encode != NULL)
	{
		cout<<"address:"<<(int)utf8Encode<<endl;
		utf8Encode = CC4EncodeUTF8::getInstance();
		cout<<"address:"<<(int)utf8Encode<<endl;
		// Do not need to delete the instance!
		cout<<"get UTF-8 Encode instance success!"<<endl;
		utf8Encode = NULL;
	}

	return 0;
}

