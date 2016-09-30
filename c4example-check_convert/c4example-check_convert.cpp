/************************************************************************/
/*                                                                      */
/* c4-lib example                                                       */
/* This project is for showing how to use c4-lib.                       */
/*                                                                      */
/* Version: 0.1                                                         */
/* Author:  wei_w (weiwl07@gmail.com)                                   */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Copyright 2016 Project c4                                            */
/* https://github.com/kuyur/c4                                          */
/*                                                                      */
/************************************************************************/

// c4example-check_convert.cpp : Defines the entry point for the console application.
//
#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "stdafx.h"
#include "../c4/c4context.h"
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
	wstring charmap = L"charmap-anisong.xml";
	CC4Context context(charmap, L"../charmaps/");
	if (!context.init())
	{
		cout<<"init failed. check charmap."<<endl;
		wcout<<context.getLastErrorMessage()<<endl;
		//cout<<CC4EncodeUTF16::convert2utf8(context.getLastErrorMessage(), wcslen(context.getLastErrorMessage()))<<endl;
		return 0;
	}
	cout<<"load charmaps success."<<endl;
	cout<<"amount of charmaps:"<<context.getEncodeAmount()<<endl;
	cout<<endl;

	CC4Encode* encode = (CC4Encode*)CC4EncodeUTF16::getInstance();
	cout<<"Static method get Unicode successfully."<<endl;
	wcout<<L"Name:"<<encode->getName()<<endl;
	wcout<<L"Version:"<<encode->getVersion()<<endl;
	wcout<<L"Description:"<<encode->getDescription()<<endl;
	cout<<endl;

	encode = (CC4Encode*)CC4EncodeUTF8::getInstance();
	cout<<"Static method get utf-8 successfully."<<endl;
	wcout<<L"Name:"<<encode->getName()<<endl;
	wcout<<L"Version:"<<encode->getVersion()<<endl;
	wcout<<L"Description:"<<encode->getDescription()<<endl;
	cout<<endl;

	const CC4Encode* const_encode = context.getEncode(L"UTF-16");
	if (!const_encode)
		cout<<"Context get unicode failed."<<endl;
	else
	{
		cout<<"Context get unicode success"<<endl;
		wcout<<L"Name:"<<const_encode->getName()<<endl;
		wcout<<L"Version:"<<const_encode->getVersion()<<endl;
		wcout<<L"Description:"<<const_encode->getDescription()<<endl;
		cout<<endl;
	}

	const_encode = context.getEncode(L"UTF-8");
	if (!const_encode)
		cout<<"Context get utf-8 failed."<<endl;
	else
	{
		cout<<"Context get utf-8 success"<<endl;
		wcout<<"Name:"<<const_encode->getName()<<endl;
		wcout<<L"Version:"<<const_encode->getVersion()<<endl;
		wcout<<L"Description:"<<const_encode->getDescription()<<endl;
		cout<<endl;
	}

	const_encode = context.getEncode(L"Shift-JIS");
	if (!const_encode)
		cout<<"Context get shift-jis failed."<<endl;
	else
	{
		cout<<"Context get shift-jis success"<<endl;
		wcout<<"Name:"<<const_encode->getName()<<endl;
		wcout<<L"Version:"<<const_encode->getVersion()<<endl;
		wcout<<L"Description:"<<const_encode->getDescription()<<endl;
		cout<<endl;
	}

	const_encode = context.getEncode(L"GBK");
	if (!const_encode)
		cout<<"Context get GBK failed."<<endl;
	else
	{
		cout<<"Context get GBK success"<<endl;
		wcout<<"Name:"<<const_encode->getName()<<endl;
		wcout<<L"Version:"<<const_encode->getVersion()<<endl;
		wcout<<L"Description:"<<const_encode->getDescription()<<endl;
		cout<<endl;
	}

	const_encode = context.getEncode(L"BIG5");
	if (!const_encode)
	{
		cout<<"Context get BIG5 failed."<<endl;
		return -1;
	}
	else
	{
		cout<<"Context get BIG5 success"<<endl;
		wcout<<"Name:"<<const_encode->getName()<<endl;
		wcout<<L"Version:"<<const_encode->getVersion()<<endl;
		wcout<<L"Description:"<<const_encode->getDescription()<<endl;
		cout<<endl;
	}

	const_encode = context.getEncode(L"EUC-KR(Korean)");
	if (!const_encode)
	{
		cout<<"Context get EUC-KR(Korean) failed."<<endl;
		return -1;
	}
	else
	{
		cout<<"Context get EUC-KR(Korean) success"<<endl;
		wcout<<"Name:"<<const_encode->getName()<<endl;
		wcout<<L"Version:"<<const_encode->getVersion()<<endl;
		wcout<<L"Description:"<<const_encode->getDescription()<<endl;
		cout<<endl;
	}

	string filePath = "../testfiles/Big5/02-big5.cue";
	ifstream infile(filePath.c_str(), ios::in|ios::binary);
	if (!infile)
		cout<<"failed to open file."<<endl;
	else
	{
		infile.seekg(0, ios::end);
		unsigned int rawLength = infile.tellg();
		char *rawStringBuffer = new char[rawLength+1];
		memset((void*)rawStringBuffer, 0, rawLength+1);
		infile.seekg(0, 0);
		infile.read(rawStringBuffer, rawLength);
		infile.close();

		const_encode = context.getMostPossibleEncode(rawStringBuffer);
		wcout<<"Find most possible encode: "<<const_encode->getName()<<endl;
		wstring &result = const_encode->wconvertText(rawStringBuffer, rawLength);
		delete []rawStringBuffer;

		string outfilePath = "../testfiles/out/02-big52unicode.cue";
		ofstream outfile(outfilePath.c_str(), ios::out|ios::binary);
		outfile.write(CC4Encode::LITTLEENDIAN_BOM, 2);
		outfile.write((char*)result.c_str(), result.length()*sizeof(wchar_t));
		outfile.close();
	}

	string filePath2 = "../testfiles/EUC-KR/01-euc-kr.cue";
	ifstream infile2(filePath2.c_str(), ios::in|ios::binary);
	if (!infile2)
		cout<<"failed to open file."<<endl;
	else
	{
		infile2.seekg(0, ios::end);
		unsigned int rawLength = infile2.tellg();
		char *rawStringBuffer = new char[rawLength+1];
		memset((void*)rawStringBuffer, 0, rawLength+1);
		infile2.seekg(0, 0);
		infile2.read(rawStringBuffer, rawLength);
		infile2.close();

		const_encode = context.getEncode(L"EUC-KR(Korean)");
		wstring &result = const_encode->wconvertText(rawStringBuffer, rawLength);
		delete []rawStringBuffer;

		string outfilePath = "../testfiles/out/01-euc-kr2unicode.cue";
		ofstream outfile(outfilePath.c_str(), ios::out|ios::binary);
		outfile.write(CC4Encode::LITTLEENDIAN_BOM, 2);
		outfile.write((char*)result.c_str(), result.length()*sizeof(wchar_t));
		outfile.close();
	}

	context.finalize();
	return 0;
}

