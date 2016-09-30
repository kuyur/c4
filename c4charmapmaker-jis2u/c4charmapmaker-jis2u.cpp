/************************************************************************/
/*                                                                      */
/* Make Ansi to Unicode Charmap 1.0 (Shift-JIS) for c4-lib              */
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

// c4charmapmaker-jis2u.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

unsigned char CharToHex(char ch) 
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
	ifstream infile1("../maps/src/CP932-1.txt");
	if (!infile1)
	{
		cerr<<"Unable to open ../maps/src/CP932-1.txt!\n";
		return -1;
	}

	ifstream infile2("../maps/src/CP932-2.txt");
	if (!infile2)
	{
		cerr<<"Unable to open ../maps/src/CP932-2.txt!\n";
		return -1;
	}

	ofstream outfile_littleendian("../maps/dest/jis2u-little-endian.map",ios::binary);
	if (!outfile_littleendian)
	{
		cerr<<"Can not open ../maps/dest/jis2u-little-endian.map!\n";
		return -1;
	}

	string str;
	int i=0;
	while(getline(infile1,str))
	{
		i++;

		unsigned char HighByte,LowByte;
		HighByte=CharToHex(str[7])*16 +CharToHex(str[8]);
		LowByte =CharToHex(str[9])*16+CharToHex(str[10]);

		if ((HighByte>255)||(LowByte>255))
			cerr<<"Error occur in CP932-1.txt's Line "<<i<<"!\n";

		outfile_littleendian.write((char*)&LowByte,1);
		outfile_littleendian.write((char*)&HighByte,1);
	}
	cout<<i<<" lines in CP932-1.txt done!\n";
	infile1.close();

	i=0;
	int offset=0x8140;
	char zero[2]={'\xFD','\xFF'};
	while(getline(infile2,str))
	{
		i++;

		int JISoffset;
		JISoffset=CharToHex(str[5])+CharToHex(str[4])*16+CharToHex(str[3])*16*16+CharToHex(str[2])*16*16*16;

		while(offset!=JISoffset)
		{
			offset++;
			outfile_littleendian.write(zero,2);
		}
		offset++;

		unsigned char HighByte,LowByte;
		HighByte=CharToHex(str[9])*16 +CharToHex(str[10]);
		LowByte =CharToHex(str[11])*16+CharToHex(str[12]);

		if ((HighByte>255)||(LowByte>255))
			cerr<<"Error occur in Line "<<i<<"!\n";

		outfile_littleendian.write((char*)&LowByte,1);
		outfile_littleendian.write((char*)&HighByte,1);
	}

	cout<<i<<" lines in CP932-2.txt done!\n";
	infile2.close();

	outfile_littleendian.close();

	return 0;
}

