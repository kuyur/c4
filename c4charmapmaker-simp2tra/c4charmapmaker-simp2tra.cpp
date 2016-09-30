/************************************************************************/
/*                                                                      */
/* Make Simplified Chinese character to Traditional Chinese character   */
/* map for c4-lib                                                       */
/*                                                                      */
/* Version: 1.0                                                         */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Copyright 2016 Project c4                                            */
/* https://github.com/kuyur/c4                                          */
/*                                                                      */
/************************************************************************/

// c4charmapmaker-simp2tra.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

unsigned char CharToHex(wchar_t ch) 
{ 
	//0-9 
	if (ch>='0'&&ch<='9') 
		return (ch-'0');         
	//9-15 
	if (ch>='A'&&ch<='F') 
		return (ch-'A'+0xA); 
	//9-15
	if (ch>='a'&&ch<='f') 
		return (ch-'a'+0xA);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	string inFilename = "../maps/src/simp2tra.txt";
	ifstream infile(inFilename.c_str(), ios::binary);
	if (!infile)
	{
		cerr<<"Unable to open ../maps/src/simp2tra.txt!\n";
		return -1;
	}

	ofstream outfile_littleendian("../maps/dest/simp2tra-little-endian.map",ios::binary);
	if (!outfile_littleendian)
	{
		cerr<<"Can not open ../maps/dest/simp2tra-little-endian.map!\n";
		return -1;
	}

	// 0xFF, 0xFE
	infile.seekg(2, ios::cur);
	wchar_t buffer[22];
	memset((void*)buffer, 0, 22*sizeof(wchar_t));
	int i=0;
	wchar_t offset=0x359E;
	wchar_t simpChar=0;
	unsigned char HighByte,LowByte;
	while(!infile.eof())
	{
		i++;
		infile.read((char*)buffer, 22*sizeof(wchar_t));
		if (buffer[20] != 0x000D || buffer[21] != 0x000A)
		{
			cout<<"Error happen at line "<<i<<endl; // last line will not be check
			break;
		}

		simpChar = CharToHex(buffer[6])+CharToHex(buffer[5])*16+CharToHex(buffer[4])*16*16+CharToHex(buffer[3])*16*16*16;
		while(offset!=simpChar)
		{
			outfile_littleendian.write((char*)&offset,2);
			offset++;
		}
		offset++;

		HighByte=CharToHex(buffer[15])*16+CharToHex(buffer[16]);
		LowByte =CharToHex(buffer[17])*16+CharToHex(buffer[18]);

		if ((HighByte>255)||(LowByte>255))
			cerr<<"Error occur in Line "<<i<<"!\n";

		outfile_littleendian.write((char*)&LowByte,1);
		outfile_littleendian.write((char*)&HighByte,1);
	}
	cout<<i<<" lines done!\n";
	infile.close();
	outfile_littleendian.close();

	return 0;
}

