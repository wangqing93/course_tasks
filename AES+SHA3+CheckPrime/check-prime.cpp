#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <random>
#include <bitset>
using namespace std;

int seed;

int find_first_one_bit(bitset<512> input_v)
{
	for(int i = 511; i >= 0; i--)
	{
		if (input_v.test(i)) 
			return i;
	}
	return -1;	
}

//a>=b return true;
bool compare(bitset<512> a, bitset<512> b) {  
	bool flag = false;
	for (int i = 0; i < 512; i++) {
		if (b.test(i)) {
			if (flag)
				return false;
			else
				flag = true;
			for (int j = i; j < 512; j++) {
				if (!a.test(j)) {
					a.set(j, 1);
				} else {
					a.set(j, 0);
					flag = false;
					break;
				}
			}
		}
	}
	if (flag)
		return false;
	return true;
}

bitset<512> reduce(bitset<512> a, bitset<512> b)
{
	bitset<512> temp = a;
	bool flag = false;
	for (int i = 0; i < 512; i++) {
		if (b.test(i)) {
			flag = true;
			for (int j = i; j < 512; j++) {
				if (!a.test(j)) {
					a.set(j, 1);
				} else {
					a.set(j, 0);
					flag = false;
					break;
				}
			}
		}
	}
	if (flag)
		return temp;
	return a;
	
}

bitset<512> add(bitset<512> a, bitset<512> b)
{
	bitset<512> temp = a;
	for (int i = 0; i < 512; i++) {
		if (b.test(i)) {
			for (int j = i; j < 512; j++) {
				if (!a.test(j)) {
					a.set(j, 1);
					break;
				} else {
					a.set(j, 0);
				}
			}
		}
	}
	return a;
	
}
//Galoias domain
bitset<512> gd_division(bitset<512> disivor, bitset<512> dividend)
{
 	bitset<512> quotients = 0;  //�� 
	bitset<512> residue = disivor; //����
	while (find_first_one_bit(residue) >= find_first_one_bit(dividend))
	{
		//�����ȱ�������
		if (compare(residue , (dividend << (find_first_one_bit(residue) -find_first_one_bit(dividend))))) {
			quotients.set((find_first_one_bit(residue) -find_first_one_bit(dividend)), 1);
			residue = reduce(residue , (dividend << (find_first_one_bit(residue) -find_first_one_bit(dividend))));
		} else {
			if (find_first_one_bit(residue) == find_first_one_bit(dividend))
				return residue;
			else {
				quotients.set((find_first_one_bit(residue) -find_first_one_bit(dividend))-1, 1);	
				residue = reduce(residue , (dividend << (find_first_one_bit(residue) -find_first_one_bit(dividend)-1)));
			}	
		}
			
	}
	return residue;
}

//a-b
bitset<512> reduce_one(bitset<512>  input_v) {
	for (int i = 0; i < 512; i++)
	{
		if (input_v.test(i))
		{
			input_v.set(i, 0);
			return input_v;
		}
	}

	for (int i = 0; i < 512; i++)
		input_v.set(i,1);	
	return input_v;
}

//a*b 
bitset<512> gd_multiply(bitset<512> a, bitset<512> b)
{
	bitset<512> res;
	while (find_first_one_bit(a) >= 0)
	{
		res = add(res , (b << find_first_one_bit(a)) );
		a. set(find_first_one_bit(a), 0);
	} 
	return res;
}

bitset<512> CalMod(bitset<512> n, bitset<512> a, bitset<512> d) {
	
	if (d == bitset<512>(0))
		return bitset<512>(1);
	if (d == bitset<512>(1))
		return a;
	if (d.test(0)) {
		d.set(0,0);
		return gd_division( gd_multiply( CalMod(n, gd_division(gd_multiply(a,a), n), d>>1), a), n);
	}
	else {
		return gd_division( CalMod(n, gd_division(gd_multiply(a,a), n), d>>1), n);	
	}
		
}

bitset<512> Produce512Random() {	
	bitset<512> res;
	for (int i = 0; i < 512; i++) {
		res.set(i, rand()%2);
	}
	return res;
}

 
bool MillerRabinBits(bitset<512> n, bitset<512> a) {
	bitset<512> d, res;
	if (n==bitset<512>("10")) {
		//cout << "n������" << endl;
		return true;
	}
	if (n==bitset<512>("1") or !n.test(0)) {
		//cout << "n��������" << endl;
		return false;
	}
	d = reduce_one(n);
	while (!d.test(0))
		d = d >> 1;
	//cout << d.to_ulong() << endl;
 	res = CalMod(n, a, d);
	//cout << n.to_ulong() << " " << a.to_ulong() << " " << d.to_ulong() << " " << res.to_ulong()<< endl;
	while ((res!= reduce_one(n)) and (res != bitset<512>(1)) and (d!=reduce_one(n)) ){  //d�����ܵ���1 
	    //cout << gd_multiply(res,res).to_ulong() << endl;
		res = gd_division(gd_multiply(res,res), n);
		d = d << 1;
		//cout << d.to_ulong() << " " << res.to_ulong()<< endl;
	}
	if (res == reduce_one(n) or d.test(0))  {
		//cout << "n������" << endl;
		return true;
	}
	else {
		//cout <<  "n��������" << endl;
		return false;
	}
}

void rand512(bitset<512>& a, int highest_bit) {
	
	if (highest_bit < 511)
		highest_bit++;
	bitset<32> mid;
	for (int i = 0; i < highest_bit; i++) {
		a.set(i, rand()%2);
	}
} 
 
bool MillerRabinMain(bitset<512> n, int t)
{
	if (n==bitset<512>(2) or n==bitset<512>(3))
		return true;
	if (n==bitset<512>(1) or !n.test(0) or n==bitset<512>(0))  
		return false;
	bool flag = true;
	int temp;
	bitset<512> a;
	for (int i = 0; i < t; i++) {
		rand512(a, find_first_one_bit(n)); 
		a = gd_division(a, n); 
		while(compare(a, reduce_one(n)) or compare(bitset<512>(1), a)) {
			rand512(a, find_first_one_bit(n)); 
			a = gd_division(a, n); 
			//cout << "a:" << a.to_ullong() << " " << endl;
		}
		
		if (!MillerRabinBits(n, a)) {
			flag = false;
			break;
		}
	}
	return flag;
}

void printResult(bitset<512> n, bool flag) {
	if (!flag)
		cout << "****************�������512bits������������****************" << endl;
	else
		cout << "****************�������512bits����������****************" << endl;
	cout << n << endl;
}
bitset<512> a_set[2046];
int getPrime(unsigned int a) {
	if (a >= 2048) {
		cout << "������С�ڵ���2047������" << endl;
		return 0; 
	}
	int index = 0;	
	for (unsigned int i = 1; i < a; i++)
	{
		if (MillerRabinBits(bitset<512>(i), bitset<512>(2))) {
			a_set[index] = bitset<512>(i);
			index++;
		}
	}
	return index;
}
void test() {
	cout << "�������2000���ڵ�������" << endl;
	int index = getPrime(2000);
	for (int i = 0; i < index; i++)
		cout << a_set[i].to_ulong() << " ";
	cout << endl;
}

int main()
{	
	
	cout << "���������������������" << endl;
	cin >> seed; 
	srand(seed);
	bitset<512> p1 = Produce512Random();
	bitset<512> p2 = Produce512Random();

	cout << "��һ���������p1: " << p1 << endl;
	cout << "�ڶ����������p2:  "<< p2 << endl;
	cout << "p1*p2: " << gd_multiply(p1, p2) << endl;
	
	cout << "������Miller Rabin�㷨�İ�ȫ���� T,�ò���Ӧ����10" << endl;
	int t;
	cin >> t;
	while(t<10) {
		cout << "����������Miller Rabin�㷨�İ�ȫ���� T,�ò���Ӧ����10" << endl;
		cin >> t; 
	}
	//cout << MillerRabinMain(bitset<512>(1373653), 10);
	cout << "Miller Rabin�㷨�İ�ȫ���� T = " << t << endl;
	cout << "*************p1���������*************" << endl; 
	printResult(p1, MillerRabinMain(p1, t));
	cout << "*************p2���������*************" << endl; 
	printResult(p2, MillerRabinMain(p2, t));

	
	cout << "�������3000���ڵ�����" << endl;
	bitset<512> temp(0);
	for(int i = 0; i < 3000; i++) {
		if (MillerRabinMain(temp, t))
			cout << temp.to_ulong() << " "; 
		temp = add(temp, bitset<512>("1"));
	}
	
	//cout << MillerRabinBits(bitset<512>(2047), bitset<512>(3))<< endl;
	system("pause"); 
	return 0;
}
