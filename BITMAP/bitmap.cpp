#include<iostream>
#include<fstream>
#include<stdio.h> 
#include<stdlib.h>
#include<random> 
using namespace std;

void produceTestfile(char* filename, int testnum) {
	FILE* fileStream;
	fileStream = fopen(filename,"wb+");
	unsigned long long maxNum = 18446744073709551615ll;
	
	std::random_device rd;
  	std::mt19937_64 mt(rd());
  	
	unsigned long long randomNum;
	for (int i = 0; i < testnum; i++)
	{
		randomNum = mt()%maxNum;
		fseek(fileStream, 8*i, SEEK_SET);
		fwrite((char*)&randomNum, 8, sizeof(char), fileStream);	
	}
	fclose(fileStream);
} 

void produceTestfile2(char* filename, int testnum) {
	FILE* fileStream;
	fileStream = fopen(filename,"wb+");
  	
	int randomNum;
	for (int i = 0; i < testnum; i++)
	{
		randomNum = rand();  //65534
		fseek(fileStream, 8*i, SEEK_SET);
		fwrite((char*)&randomNum, 8, sizeof(char), fileStream);	
	}
	fclose(fileStream);
}

unsigned long long getTestdata(char* filename, int index) {
	FILE* fileStream;
	fileStream = fopen(filename, "rb+");
	unsigned long long randomNum;
	fseek(fileStream, 8*index, SEEK_SET);
	fread(&randomNum, 8, sizeof(char), fileStream);	
	fclose(fileStream);
	return randomNum;
}
int main()
{
	//produceTestfile("10test.dat\0", 10);
	int size = 100000000/512+1;
	unsigned long long mark[size];
	unsigned long long data = 0;
	unsigned long long nBytePos;
	unsigned short val;
	int nbitPos; 
	for (int i = 0; i < size; i++)
		mark[i] = 0;
	for (int i = 0; i < 100000000; i++)
	{
		data = getTestdata("100000000test.dat\0", i);
		//cout << data << endl;
		if (data >= 100000000)
			continue;
		else 
		{
			//cout << "hello";
			nBytePos = data/512;
			nbitPos = data&511;
			val = 1 << nbitPos;
			//cout << nBytePos << "  " << nbitPos <<"  " << val << endl;
			mark[nBytePos] = mark[nBytePos] | val;
			//cout << mark[nBytePos] << endl; 
		}
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			if ((mark[i] >> j) & 1 == 1)
				continue;
			else
			{
				cout << i*512+j << endl;
				return 0;
			}
		}
	}

	return 0;
}    
