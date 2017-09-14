#include<iostream>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

bool deleteformax(int a[], int a_mark[], int k, int a_len, int fix_num, int need_num)
{
	if (k == 0 ) 
		return false;
	else if (fix_num == need_num)
		return true;
	int reserve = need_num - fix_num - 1;
	int max = -1, max_num = -1;
	
	for (int i = a_len-1; i >= 0; i--)
	{                               
		if (reserve == 0)
		{
			reserve = i;
			break;
		}
		if (a_mark[i] == 0) 
		{		
			reserve--;
		}
	}
	for (int i = 0; i <= reserve; i++)
	{
		if (a_mark[i] == 0)
		{
			if (a[i] == 9)
			{
				max_num = i;
				break;
			}
			else if(a[i] > max)
			{
				max = a[i];
				max_num = i;
			}
		}
	}
	a_mark[max_num] = 2;
	fix_num++;
	
	for (int j = 0 ;j < max_num; j ++)
	{
		if (a_mark[j] == 0){
			a_mark[j] = 1;
			k--;
		}
	}
	return deleteformax(a, a_mark, k, a_len,fix_num, need_num);
}

int main()
{
	int T;
	cin >> T;
	string s,temp; 
	int k,len; 
	int num[100001] = {0}; 
	int mark[100001] = {0}; // 0-in  1-out 2-mustin
	bool outflag;
	
	for (int i = 0; i < T; i++)
	{
		cin >> s >> k;
		len = s.length();
		for (int j = 0; j < len; j++)
		{
			temp = s[j];
			num[j] = atoi(temp.c_str());
			mark[j] = 0;
		}
		outflag = deleteformax(num, mark, k, len, 0, len-k);
		if (outflag) {
			for(int j = 0; j< len; j++)
			{
				if (mark[j] == 2)
					cout << num[j];
			}
			cout << endl;
		} else
		{
			for(int j = 0; j< len; j++)
			{
				if (mark[j] != 1)
					cout << num[j];
			}
			cout << endl;
		}
		
	} 
	return 0;
} 
