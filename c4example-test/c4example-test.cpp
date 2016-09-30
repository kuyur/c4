/************************************************************************/
/*                                                                      */
/* c4-lib example                                                       */
/* Memory Testing                                                       */
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
// c4example-test.cpp : Defines the entry point for the console application.
//

#pragma  once

#if defined( WIN32 ) && defined( TUNE )
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

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

#if defined( WIN32 ) && defined( _DEBUG )
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	wstring charmap = L"charmap-anisong.xml";
	CC4Context context(charmap, L"../charmaps/");
	if (!context.init())
	{
		cout<<"init failed. check charmap."<<endl;
		wcout<<context.getLastErrorMessage()<<endl;
		return 0;
	}
	cout<<"load charmap.xml success."<<endl;
	cout<<"amount of charmaps in charmap.xml:"<<context.getEncodeAmount()<<endl;
	list<wstring> &names = context.getEncodesNameList();
	list<wstring>::iterator name_iter;
	for (name_iter = names.begin(); name_iter != names.end(); ++name_iter)
	{
		wcout<<*name_iter<<endl;
	}
	cout<<endl;
	context.finalize();

	CC4Context *newContext = new CC4Context(L"charmap-chinese.xml", L"../charmaps/");
	if (!newContext->init())
	{
		cout<<"init failed. check charmap."<<endl;
		wcout<<newContext->getLastErrorMessage()<<endl;
		return 0;
	}
	cout<<"load charmap-chinese success."<<endl;
	cout<<"amount of charmaps in charmap-chinese:"<<newContext->getEncodeAmount()<<endl;
	list<wstring> &chn_names = newContext->getEncodesNameList();
	list<wstring>::iterator chnname_iter;
	for (chnname_iter = chn_names.begin(); chnname_iter != chn_names.end(); ++chnname_iter)
	{
		wcout<<*chnname_iter<<endl;
	}
	cout<<endl;
	newContext->finalize();
	delete newContext;

	// CC4EncodeUTF16 and CC4EncodeUTF8 static instances will be deleted automatically.
	const CC4EncodeUTF16* instance_utf16 = CC4EncodeUTF16::getInstance();
	const CC4EncodeUTF8*  instance_utf8  = CC4EncodeUTF8::getInstance();
	// delete instance_utf16; /* forbidden */
	// delete instance_utf8;  /* forbidden */

	return 0;
}

