#include<iostream>
#include<cmath>
#include<stdlib.h>

#include <sys/time.h>
using namespace std;


char* str="abcdefghigklmnop\n";
long long int* fac;
long long int factorial(int n) { 
	long long int result = 1;
	for (int i = 1; i <= n ; i++) {
		result *= i;
	}
	return result;
}

int* dec2dic(int index, int n) {
	int* allRes = new int[n-1];
	for (int i = 0; i < n-1; i++) {
		allRes[i] = index/fac[n-2-i];
		index = index % fac[n-2-i];
	}
	return allRes;
}

int* dec2inc(int index, int n) {
	int modRes;
	int* allRes = new int[n-1];
	for (int i = 2; i <= n; i++) {
		modRes = index%i;
		index = index/i;
		allRes[n-i] = modRes;
	}
	return allRes;
}

int* dec2dec(int index, int n) {
	int modRes;
	int* allRes = new int[n-1];
	for (int i = n; i > 1; i--) {
		modRes = index%i;
		index = index/i;
		allRes[i-2] = modRes;
	}
	return allRes;
}

void dicNum2per(int* incNum, int n) {
	char* aimper = new char[n+1];
	int temp = 0; 
	aimper[n]='\0';
	int* mark = new int[n];
	for (int i = 0; i < n; i++)
		mark[i] = -1; 
	for (int i = 0; i < n-1; i++) {
		temp = 0;
		for(int j = 0; j < n; j++) {
			if (mark[j] == -1) {
				if (temp == incNum[i]) {
					aimper[i] = str[j];
					mark[j] = 1;
					break;
				} else
					temp++;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		if (mark[i] == -1) {
			aimper[n-1] = str[i];
		}
	}
	//cout << aimper << endl;
	delete[] aimper;
	delete[] mark;
}
	
void changeNum2per(int* incNum, int n) {
	char* aimper = new char[n+1];
	int temp = 0, direction = 0;   // 1:left -1:right
	aimper[n]='\0';
	int* mark = new int[n];
	for (int i = 0; i < n; i++)
		mark[i] = -1; 
	for (int i = n-2; i > 0; i--) {
		temp = 0;
		if (i%2 == 1) {
			if (incNum[i-1]%2 == 1)
				direction = -1;
			else
				direction = 1;
		} else {
			if ((incNum[i-1] + incNum[i-2])%2 == 1)
				direction = -1;
			else
				direction = 1;
		}
		if (direction == -1) {
			//cout << "right" << endl;
			for(int j = 0; j < n; j++) {
				if (mark[j] == -1) {
					if (temp == incNum[i]) {
						aimper[j] = str[i+1];
						mark[j] = 1;
						break;
					} else
					temp++;
				}
			}
		} else {
			//cout << "left" << endl; 
			for(int j = n-1; j >= 0; j--) {
				if (mark[j] == -1) {
					if (temp == incNum[i]) {
						aimper[j] = str[i+1];
						mark[j] = 1;
						break;
					} else
					temp++;
				}
			}
		}	
	}
	temp = 0;
	for(int j = n-1; j >= 0; j--) {
		if (mark[j] == -1) {
			if (temp == incNum[0]) {
				aimper[j] = str[1];
				mark[j] = 1;
				break;
			} else
				temp++;
		}
	}
	for (int i = 0; i < n; i++) {
		if (mark[i] == -1) {
			aimper[i] = str[0];
			break;
		}
	}
	//cout << aimper << endl;
	delete[] aimper;
	delete[] mark;
}		
//from Intermediary number  to permutation
void incNum2per(int* incNum, int n) {
	char* aimper = new char[n+1];
	aimper[n]='\0';
	int temp = 0;
	for (int i = 0 ; i < n; i++)
		aimper[i]='z';
	for (int i = 0; i < n-1; i++) {
		temp = 0;
		for(int j = n-1; j >= 0; j--) {
			if (aimper[j] == 'z') {
				if (temp == incNum[i]) {
					aimper[j] = str[n-1-i];
					break;
				} else
					temp++;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		if (aimper[i] == 'z') {
			aimper[i] = 'a';
			break;
		}
	}
	//cout << aimper << endl;
	delete[] aimper;
}
 
void decNum2per(int* incNum, int n) {
	char* aimper = new char[n+1];
	aimper[n]='\0';
	int temp = 0;
	for (int i = 0 ; i < n; i++)
		aimper[i]='z';
	for (int i = n-2; i >= 0; i--) {
		temp = 0;
		for(int j = n-1; j >= 0; j--) {
			if (aimper[j] == 'z') {
				if (temp == incNum[i]) {
					aimper[j] = str[i+1];
					break;
				} else
					temp++;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		if (aimper[i] == 'z') {
			aimper[i] = 'a';
			break;
		}
	}
	//cout << aimper << endl;
	delete[] aimper;
}
 
int main()
{
	fac = new long long int[12];
	for (int i = 1; i <= 12; i++) {
		fac[i-1] = factorial(i);
	}
	struct timeval start, end;
	int n;
	cin >> n; 
	while (n > 16) {
		cout << "please input the integer which is less than 16" << endl;
		cin >> n;
	} 
	long long int sum = factorial(n); 
	
	gettimeofday(&start,NULL);
	for (int i = 0; i < sum; i++) {
		int * allres = dec2dic(i,n);
		/*for (int j = 0; j < n-1; j++)
			cout << allres[j] << " ";
		cout << endl;*/
		dicNum2per(allres, n);
		delete[] allres;
	}
	gettimeofday(&end,NULL);
    long temp = (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
    cout <<"dictionary,the time it cost : "<< temp << "ms" << endl;
    
    
	gettimeofday(&start,NULL);
	for (int i = 0; i < sum; i++) {
		int * allres = dec2inc(i,n);
		/*for (int j = 0; j < n-1; j++)
			cout << allres[j] << " ";
		cout << endl;*/
		incNum2per(allres, n);
		delete[] allres;
	}
	gettimeofday(&end,NULL);
    temp = (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
    cout <<"increase Base, the time it cost : "<< temp << "ms" << endl;
        
    gettimeofday(&start,NULL);
	for (int i = 0; i < sum; i++) {
		int * allres = dec2dec(i,n);
		/*for (int j = 0; j < n-1; j++)
			cout << allres[j] << " ";
		cout << endl;*/
		decNum2per(allres, n);
		delete[] allres;
	}
	gettimeofday(&end,NULL);
    temp = (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
    cout <<"decrease Base,the time it cost : "<< temp << "ms" << endl;
        
    
    
    gettimeofday(&start,NULL);
	for (int i = 0; i < sum; i++) {
		int * allres = dec2dec(i,n);
		/*for (int j = 0; j < n-1; j++)
			cout << allres[j] << " ";
		cout << endl;*/
		changeNum2per(allres, n);
		delete[] allres;
	}
	gettimeofday(&end,NULL);
    temp = (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
    cout <<"near change,the time it cost : "<< temp << "ms" << endl;
    
	system("pause");
	return 0;
} 
