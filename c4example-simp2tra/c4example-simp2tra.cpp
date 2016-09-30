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
// c4example-simp2tra.cpp : Defines the entry point for the console application.
//

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
	wstring charmap = L"charmap-chinese.xml";
	CC4Context context(charmap, L"../charmaps/");
	if (!context.init())
	{
		cout<<"init failed. check charmap."<<endl;
		wcout<<context.getLastErrorMessage()<<endl;
		return 0;
	}
	cout<<"load charmaps success."<<endl;
	cout<<"amount of charmaps:"<<context.getEncodeAmount()<<endl;
	cout<<endl;

	const CC4Encode *const_encode = context.getEncode(L"Simp-CHN(Unicode) to Tra-CHN(Unicode)");
	if (!const_encode)
	{
		cout<<"Context get Simp-CHN(Unicode) to Tra-CHN(Unicode) failed."<<endl;
		return -1;
	}
	else
	{
		cout<<"Context get Simp-CHN(Unicode) to Tra-CHN(Unicode) success"<<endl;
		wcout<<"Name:"<<const_encode->getName()<<endl;
		wcout<<L"Version:"<<const_encode->getVersion()<<endl;
		wcout<<L"Description:"<<const_encode->getDescription()<<endl;
		cout<<endl;
	}

	string filePath = "../testfiles/utf-16/wukongzhuang-unicode-simp.txt";
	ifstream infile(filePath.c_str(), ios::in|ios::binary);
	if (!infile)
		cout<<"failed to open file."<<endl;
	else
	{
		infile.seekg(0, ios::end);
		unsigned int rawLength = infile.tellg();
		if ((rawLength&1) !=0 )
		{
			cout<<"Error file."<<endl;
			return -1;
		}
		char *rawStringBuffer = new char[rawLength+2];
		memset((void*)rawStringBuffer, 0, rawLength+2);
		infile.seekg(0, 0);
		infile.read(rawStringBuffer, rawLength);
		infile.close();

		wstring &result = const_encode->wconvertWideText((wchar_t*)rawStringBuffer, rawLength>>1);
		delete []rawStringBuffer;

		string outfilePath = "../testfiles/out/wukongzhuang-unicode-tra.txt";
		ofstream outfile(outfilePath.c_str(), ios::out|ios::binary);
		//outfile.write(CC4Encode::LITTLEENDIAN_BOM, 2);
		outfile.write((char*)result.c_str(), result.length()*sizeof(wchar_t));
		outfile.close();
	}

	context.finalize();
	return 0;
}

