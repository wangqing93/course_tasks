#include<iostream>
#include<string.h>
#include<cmath>
#include<ctime>
#include<bitset>

using namespace std;
//https://github.com/gvanas/KeccakCodePackage/blob/master/Modes/KeccakHash.h

const int  b = 1600;
const int  w = 64;
const int l = 6;
void printBits(bitset<1>*** A, int x, int y, int z, string tips) {
	cout << "******* " << tips << " *******" << endl;
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			cout << "[" << j << "," << i << "]: " ;
			for (int k = 0; k < z; k++) {
				cout << A[j][i][k];
				if ((k+1)%8 == 0)
					cout << " ";
			}
			cout << endl;
		}
	}
}


void printStr(string temp, string tips) {
	cout << "******* " << tips << " *******" << endl;
	for (int i = 0; i < temp.length(); i=i+8) {
		if (i%32 == 0 and i != 0)  
			cout << endl;
		cout << temp.substr(i,4) << " " << temp.substr(i+4, 4) << " ";	
	}
	cout << endl;
}
// pad10*1
string pad101(int x, int m) {
	int j = ((-m-2)%x+x)%x;
	string res = "1";
	for (int i = 0; i < j; i++) {
		res+="0";
	}
	res+="1";
	return res;
}

void algorithm1(bitset<1>*** A) {
	bitset<1> C[5][w];
	bitset<1> D[5][w];
	for (int x = 0; x < 5; x++) {
		for (int z = 0; z < w; z++) {
			C[x][z].set(0,0);
			for (int y = 0; y < 5; y++) {
				C[x][z] ^= A[x][y][z];
			}
		}
	}
	
	for (int x = 0; x < 5; x++) {
		for (int z = 0; z < w; z++) {
			D[x][z] = C[(x+4)%5][z] ^ C[(x+1)%5][(z+w-1)%w];
		}
	}
	
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A[x][y][z] ^= D[x][z];
		}
	}
}


void algorithm2(bitset<1>*** A) {
	bitset<1>*** A1;
	A1 = new bitset<1>**[5];
	
	for (int i = 0; i < 5; i++) {
		A1[i]= new bitset<1>*[5];
		for (int j = 0; j < 5; j++) 
			A1[i][j] = new bitset<1>[w];		
	}
	
	for (int x = 0; x < 5 ; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A1[x][y][z] = A[x][y][z];
		}
	}
	
	int x = 1, y = 0, temp;
	for (int t = 0; t < 24; t++) {
		for (int z = 0; z < w; z++) {
			A1[x][y][z] = A[x][y][(z-(t+1)*(t+2)/2+w*10)%w];
		} 
		temp = x;
		x = y;
		y = (2*temp + 3*y)%5;
	}
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A[x][y][z] = A1[x][y][z];
		}
	}
} 

void algorithm3(bitset<1>*** A) {
	bitset<1>*** A1;
	A1 = new bitset<1>**[5];
	
	for (int i = 0; i < 5; i++) {
		A1[i]= new bitset<1>*[5];
		for (int j = 0; j < 5; j++) 
			A1[i][j] = new bitset<1>[w];		
	}
	
	for (int x = 0; x < 5 ; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A1[x][y][z] = A[(x+3*y)%5][x][z];
		}
	}
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A[x][y][z] = A1[x][y][z];
		}
	}	
}


void algorithm4(bitset<1>*** A) {
	bitset<1>*** A1;
	A1 = new bitset<1>**[5];
	
	for (int i = 0; i < 5; i++) {
		A1[i]= new bitset<1>*[5];
		for (int j = 0; j < 5; j++) 
			A1[i][j] = new bitset<1>[w];		
	}
	
	for (int x = 0; x < 5 ; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A1[x][y][z] = A[x][y][z] ^ ( (A[(x+1)%5][y][z] ^ bitset<1>(1)) & A[(x+2)%5][y][z] );
		}
	}
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A[x][y][z] = A1[x][y][z];
		}
	}
	
}

bitset<1> rc(int t) {
	if (t%255 == 0)
		return bitset<1>(1);
	string R="10000000";
	for (int i = 1; i <= (t%255); i++) {
		R = "0"+ R;
		R[0] = (bitset<1>(R.at(0)) ^ bitset<1>(R.at(8))).to_string().c_str()[0];
		R[4] = (bitset<1>(R.at(4)) ^ bitset<1>(R.at(8))).to_string().c_str()[0];
		R[5] = (bitset<1>(R.at(5)) ^ bitset<1>(R.at(8))).to_string().c_str()[0];
		R[6] = (bitset<1>(R.at(6)) ^ bitset<1>(R.at(8))).to_string().c_str()[0];
		R = R.substr(0,8);
	}
	return bitset<1>(R[0]);	
	
}
void algorithm5(bitset<1>*** A, int Ir) {
	bitset<1>*** A1;
	A1 = new bitset<1>**[5];
	
	for (int i = 0; i < 5; i++) {
		A1[i]= new bitset<1>*[5];
		for (int j = 0; j < 5; j++) 
			A1[i][j] = new bitset<1>[w];		
	}
	
	for (int x = 0; x < 5 ; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A1[x][y][z] = A[x][y][z];
		}
	}
	
	string RC="";
	for (int i = 0; i < w; i++) {
		RC += "0";
	}
	
	for (int j = 0; j <= l; j++) {
		RC[pow(2.0, j)-1] = rc(j+7*Ir).to_string().c_str()[0];
	}
	
	for (int z = 0; z < w; z++)
		A1[0][0][z] ^= bitset<1>(RC[z]);
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				A[x][y][z] = A1[x][y][z];
		}
	}
}

// keccak_p(1600,12+2log2(1600/25))
string keccak_f1600(string S) {
	bitset<1>*** A;
	A = new bitset<1>**[5];
	
	for (int i = 0; i < 5; i++) {
		A[i]= new bitset<1>*[5];
		for (int j = 0; j < 5; j++) 
			A[i][j] = new bitset<1>[w];		
	}
	for (int x = 0; x < 5 ; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++) {
			 A[x][y][z] = bitset<1>(S[w*(5*y+x)+z]);
			}
		}
	}
	
	for (int i = 0; i <= (12+2*l-1); i++) {
		algorithm1(A);
		algorithm2(A);
		algorithm3(A);
		algorithm4(A);
		algorithm5(A, i);
	} 
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			for (int z = 0; z < w; z++)
				S[w*(5*y+x)+z] = A[x][y][z].to_string().c_str()[0];
		}
	}
	return S;
} 

// r+c=1600
string sponge1600(int r, string N, int d) {
	string P = N + pad101(r, N.length());
	int n = P.length()/r;
	int c = b - r;
	string Pi[n];
	for (int i = 0; i < n; i++)
		Pi[i] = P.substr(i*r, r);
	string S = "";
	for (int i = 0; i< b; i++)
		S += "0";
	string temp, Z = "";
	for (int i = 0; i < n; i++) {
		temp = Pi[i];
		for(int j = 0; j < c; j++) {
			temp += "0";
		}
		S = keccak_f1600( (bitset<b>(S) ^ bitset<b>(temp)).to_string() );
		Z += S.substr(0, r);
		if (d <= Z.length())
			return Z.substr(0, d);
		else
			S = keccak_f1600(S);
	}
}



void keccak(int c, string msg, int d) {
	int len = msg.length();
	bitset<8> temp;
	string N; 
	for (int i = 0; i < msg.length(); i++) {
		N += bitset<8>((unsigned long)msg.at(i)).to_string();
	}
	N+="01";
	printStr(sponge1600(1600-c,N,d), "result");
}




int main() {
	string msg;
	int cylen;
	clock_t start,finish;
    
	while (true) {
		cout << "请输入要加密的字符串:" << endl; 
		cin >> msg;
		cout << "请输入要加密的SHA-3的密文长度，支持224,256,384,512：" << endl;
		while (true) {
			cin >> cylen;
			if (cylen == 256 or cylen == 224 or cylen == 384 or cylen == 512)
			{
				start=clock();
				keccak(cylen*2, msg, cylen);
				cout << "运行时间:" << clock()-start << "ms" << endl; 
				break;
			} else {
				cout << "请重新输入要加密的SHA-3的密文长度，支持224,256,384,512：" << endl; 
			}
		}
	}
	return 0;
} 
