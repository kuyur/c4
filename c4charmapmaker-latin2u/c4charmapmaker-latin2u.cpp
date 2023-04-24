/************************************************************************/
/*                                                                      */
/* Make Latin(Windows-1252) to Unicode Charmap for c4-lib               */
/*                                                                      */
/* Version: 1.0                                                         */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Copyright 2016 Project c4                                            */
/* https://github.com/kuyur/c4                                          */
/*                                                                      */
/************************************************************************/

// c4charmapmaker-latin2u.cpp : Defines the entry point for the console application.
//

#pragma once

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

unsigned char CharToHex(char ch) 
{ 
	// 0-9
	if (ch>='0'&&ch<='9') 
		return (ch-'0');         
	// 9-15
	if (ch>='A'&&ch<='F') 
		return (ch-'A'+0xA); 
	// 9-15
	if (ch>='a'&&ch<='f') 
		return (ch-'a'+0xA);

	return(0);
}

int _tmain(int argc, _TCHAR* argv[])
{
	string inFilename="../maps/src/CP1252.txt";
	ifstream infile(inFilename.c_str());
	if (!infile)
	{
		cerr<<"Unable to open ../maps/src/CP1252.txt!\n";
		return -1;
	}

	ofstream outfile_littleendian("../maps/dest/latin2u-little-endian.map",ios::binary);
	if (!outfile_littleendian)
	{
		cerr<<"Can not open ../maps/dest/latin2u-little-endian.map!\n";
		return -1;
	}

	string str;
	int i=0;
	int offset=0x80;
	char zero[2]={'\xFD','\xFF'};
	while(getline(infile,str))
	{
		i++;

		int latinOffset;
		latinOffset=CharToHex(str[3])+CharToHex(str[2])*16;

		while(offset!=latinOffset)
		{
			offset++;
			outfile_littleendian.write(zero,2);
		}
		offset++;

		unsigned char HighByte,LowByte;
		HighByte=CharToHex(str[10])*16+CharToHex(str[11]);
		LowByte =CharToHex(str[12])*16+CharToHex(str[13]);

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
