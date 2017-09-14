#include<iostream>
#include<bitset>
#include<ctime> 
using namespace std;

bitset<8>  RCon[11]={0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};  //左移24位得到32进行异或 

static unsigned int s_box_1[64] = {
1,0,0,0,1,1,1,1,
1,1,0,0,0,1,1,1,
1,1,1,0,0,0,1,1,
1,1,1,1,0,0,0,1,
1,1,1,1,1,0,0,0,
0,1,1,1,1,1,0,0,
0,0,1,1,1,1,1,0,
0,0,0,1,1,1,1,1};


static bitset<8> mix_columns[16] = {
0x02, 0x03, 0x01, 0x01,
0x01, 0x02, 0x03, 0x01,
0x01, 0x01, 0x02, 0x03,
0x03, 0x01, 0x01, 0x02};
 
static bitset<1> s_box_2[8] = {
1,1,0,0,0,1,1,0}; 


bitset<8> aes_multiply(bitset<8> input_v1, bitset<8> input_v2) {
	bitset<8> midres[8];
	bitset<8> res;
	bitset<8> temp;
	midres[0] = input_v2;
	for (int i = 1 ;i < 8; i++)
	{
		midres[i] = midres[i-1] << 1;
		if (midres[i-1].to_ulong()&0x80)     //0x80=8*16=128
		{
			midres[i]^=0x1b;    //因为二进制除法： 被除数/除数，因为被除数只有8位，那除数也取后8位 
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if ((input_v1>>i).to_ulong()&0x01)
		{
			res^=midres[i];
		}
	}
	return res;
}

   

int find_first_one_bit(bitset<16> input_v)
{
	for(int i = 0; i < 16; i++)
	{
		if ((bitset<1>(input_v.to_string(), i, 1)) == 1) 
			return 15-i;
	}
	return -1;	
}


void add_one(bitset<128>&  input_v)
{
	bool flag = false;
	for (int i = 0; i < 128; i++)
	{
		if (input_v.test(i)==0)
		{
			input_v.set(i,1);
			flag = true;
			for (int j = 0; j < i; j++)
				input_v.set(j,0);
			break;
		}
	}
	if (!flag)
	input_v = 0;
}
//Galoias domain
void gd_division(bitset<16> disivor, bitset<16> dividend, bitset<16>& quotients, bitset<16>& residue)
{
	quotients = 0;  //商 
	residue = disivor; //余数
	while (find_first_one_bit(residue) >= find_first_one_bit(dividend))
	{
		//除数比被除数大
		quotients.set((find_first_one_bit(residue) -find_first_one_bit(dividend)), 1);
		residue = residue ^ (dividend << (find_first_one_bit(residue) -find_first_one_bit(dividend)));
	}
}

bitset<16> gd_multiply(bitset<16> a, bitset<16> b)
{
	bitset<16> res;
	while (find_first_one_bit(a) >= 0)
	{
		res = res ^ (b << find_first_one_bit(a));
		a. set(find_first_one_bit(a), 0);
	} 
	return res;
}

//http://book.2cto.com/201209/5239.html 
bitset<8> aes_inversion(bitset<16> input_v)
{
	bitset<16> s0 = 1, s1 = 0, t0 = 0, t1 = 1, s = 0, t = 1;
	bitset<16> a = input_v, b = 0x11b;
	bitset<16> r, q;
	gd_division(a, b, q, r);  	

	while (r!=0)
	{
		s = s0 ^ (gd_multiply(q, s1));
		t = t0 ^ (gd_multiply(q, t1));
		s0 = s1, s1 = s;
		t0 = t1; t1 = t;
		a = b;
		b = r;
		gd_division(a, b, q, r);  
	}	

	return bitset<8>(s1.to_string(), 8, 8);	
}

bitset<8> s_box_algorithm2(bitset<8> input_v) {
	bitset<8> input_inversion = bitset<8>(aes_inversion(input_v.to_ulong()));
	bitset<1> temp[8] = {0};
	bitset<1> res[8] = {0};
	bitset<8> ans;
	unsigned int mid_res;
	for (int i = 0; i < 8; i++)
	{
		mid_res = 0;
		for (int j = 0; j < 8 ; j++) {
			mid_res ^= s_box_1[i*8+j]*((input_inversion>>j).to_ulong()&0x01);
		}
		if (( bitset<1>(mid_res)^s_box_2[i] )== 1)
			ans|=0x01<<i;
		
	}
	return ans;
}

void rotWord(bitset<8>* state, int num) 
{
	bitset<8> backup[4];
	for (int i = 0; i < 4; i++) {
		backup[i] = state[i];
	}
	for(int i = 0; i < 4; i++)
	{
		state[i] = backup[(i+num)%4];
	}
}

void keyExpansion(bitset<8> key[16], bitset<8>* res)
{
	bitset<8>  mid_res[4];
	for (int i = 0; i < 16; i++) {
		res[i] = key[i];
	}		
	for (int i = 4; i < 44; i++) {
		if (i%4 == 0)
		{
			for (int j = 0; j < 4; j++)
				mid_res[j] = res[i*4-13+j*4];
			rotWord(mid_res,1);
			for (int j = 0; j < 4; j++)
				res[i*4+4*j] = s_box_algorithm2(mid_res[j]);
			res[i*4] ^= RCon[i/4];
			for (int j = 0; j < 4; j++)
				res[i*4+4*j] ^= res[i*4+4*j-16];	
		} else
		{
			for (int j = 0; j < 4; j++)
				res[(i/4)*16+(i%4)+j*4] = res[(i/4)*16+(i%4)+j*4-1] ^ res[(i/4-1)*16+(i%4)+j*4];
		}
	}
}

void subBytes(bitset<8>* state)
{
	for (int i = 0; i < 16; i++)
	{
		state[i] = s_box_algorithm2(state[i]);
	}
}

void shiftRows(bitset<8>* state)
{
	bitset<8> temp[4];
	for (int i = 1; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			temp[j] = state [i*4+j];
		rotWord(temp, i);
		for (int j = 0; j < 4; j++)
			state[i*4+j] = temp[j];
	}
}

void mixColumns(bitset<8>* state)
{
	bitset<8> temp[4];
	for (int i = 0; i < 4; i++)
	{
		//每一列
		for (int j = 0; j < 4; j++)
		{
			temp[j] = state[i+j*4];
		}
		for (int j = 0; j < 4; j++)
		{
			state[i+j*4] = 0;
			
			for (int k = 0; k < 4; k++) 
			{
				state[i+j*4] ^= aes_multiply(mix_columns[j*4+k], temp[k]);
			}
		}
			
	}
}

void addRoundKey(bitset<8>* state, bitset<8>* key)
{
	for (int i = 0; i < 16; i++)
		state[i]^=key[i];
}

void get_key(bitset<8>* round_keys, bitset<8>* now_round_key, int round_num) 
{
	for (int i = 0; i < 16; i++ )
		now_round_key[i] = round_keys[i+round_num*16]; 
}

void print_v(bitset<8>* v, int num, string s)
{
	cout << "***************   " << s << "   ***************" << endl;
	for (int i = 0; i < num; i++)
		cout << i << "  " << v[i] << endl; 
}

void print_byte_value(bitset<8>* v, int num, string s)
{
	cout << "***************   " << s << "   ***************" << endl;
	for (int i = 0; i < num; i++)
		cout << i << "  " << v[i].to_ulong() << endl; 
}

void printres(bitset<8>* v, int num, string s)
{
	for (int i = 0; i < num/16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			cout << v[(j%4)*4+j/4] << " ";
 		}
 	}
	cout << endl;
}
/**
http://coolshell.cn/wp-content/uploads/2010/10/rijndael_ingles2004.swf
http://www.mamicode.com/info-detail-514466.html  
**/

void aes(bitset<8>* state, bitset<8> key[16])
{
	bitset<8>* round_keys = new bitset<8>[4*44];
	keyExpansion(key, round_keys);
	bitset<8>* now_round_key = new bitset<8>[16];
	get_key(round_keys, now_round_key, 0);
    addRoundKey(state, now_round_key);
	
    for (int i = 1; i < 10; i++)
	{
		subBytes(state);
		shiftRows(state);
		mixColumns(state);
		get_key(round_keys, now_round_key, i);
    	addRoundKey(state, now_round_key);
	}
	
	subBytes(state);
	shiftRows(state);
	get_key(round_keys, now_round_key, 10);
    addRoundKey(state, now_round_key);
}

void cbc_encry_one_round(bitset<8> key[16], bitset<8>* state, bitset<8> xor_value[16])
{
	for (int i = 0; i < 16; i++)
		state[i] ^= xor_value[i];
	aes(state, key);

}


void ctr_encry_one_round(bitset<8> key[16], bitset<8>* counter_value, bitset<8> plain[16])
{
	aes(counter_value, key);
	for (int i = 0; i < 16; i++)
		counter_value[i] ^= plain[i];
}

void pkcs7(string& s, int num)
{
	int len = s.length();
	int need_num = num - len%num;
	if (len/num < 2)
		need_num = 2*num - len;
	
	s += string(need_num, need_num);
}

/*
http://www1.tc711.com/tool/BASE64.htm
http://tool.chacuo.net/cryptaes
*/


void base64(bitset<8>* output, int size)
{
	char base64Char[64];
	for (int i = 0; i < 26; i++)
		base64Char[i]= char(65+ i);
	for (int i = 0; i < 26; i++)
		base64Char[i+26]= char(97+ i);
	for (int i = 0; i < 10; i++)
		base64Char[i+52]= char(48+ i);
	base64Char[62] ='+';
	base64Char[63] = '/';
		 
	bitset<8> temp[4];
	for (int i = 0; i+2 < size; i= i+3)
	{
		cout << output[i] << output[i+1] << output[i+2] << endl;
		temp[0] = output[i] >> 2;
		temp[1] = (output[i]&bitset<8>(0x03) << 4 ) ^(output[i+1] >> 4);
		temp[2] = (output[i+1]&bitset<8>(0x0f) << 2 ) ^(output[i+2] >> 6);
		temp[3] = output[i+2]&bitset<8>(0x3f);
		for(int j = 0;j < 4; j++)
			cout <<base64Char[temp[j].to_ulong()&0xff] ; 
	}
	if (size%3==1)
		cout << "==";  //百度百科有解释，为了让其编码数是4的倍数 
	if (size%3==2)
		cout << "=";
}

void cbc_encry(string s, bitset<8> key[16], bitset<8> xor_value[16], int num)
{
	pkcs7(s,num);
	int group_num = s.length()/num;
	bitset<8> plain_text[16];
    bitset<8> res[group_num*16];
	for (int i = 0; i < 16; i++)
		xor_value[i] = 0x00;
	for (int i = 0; i < group_num; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			plain_text[(j%4)*4+j/4] = bitset<8>((unsigned long)(s.at(i*16+j)));
 		}
		cbc_encry_one_round(key, plain_text, xor_value);
		for (int j = 0; j < 16; j++)
		{
			xor_value[j] = plain_text[j];
			res[i*16+j] = plain_text[(j%4)*4+j/4];
		}
	}
	//print_byte_value(res, group_num*num, "cbc cipher");
	printres(res, group_num*num, "cbc cipher");
}

void ctr_encry(string s, bitset<8> key[16], int num)  //num字节数 
{
	pkcs7(s,num);
	int group_num = s.length()/num;
	bitset<8> plain_text[16];
    bitset<8> res[group_num*16];
    bitset<8> counter_value[16];
    bitset<128> counter_values = 0x00, temp;
    unsigned long long int temp_counter;
	for (int i = 0; i < group_num; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			plain_text[(j%4)*4+j/4] = bitset<8>((unsigned long)(s.at(i*16+j)));
 		}
 		
 		for (int j = 0; j < 16; j++)
 		{
 			temp = counter_values>>(120-j*8);
 			counter_value[(j%4)*4+j/4] = bitset<8>(temp.to_ulong());
 		}
		ctr_encry_one_round(key, counter_value, plain_text); 
		add_one(counter_values);
		for (int j = 0; j < 16; j++)
		{
			res[i*16+j] = counter_value[(j%4)*4+j/4];
		}
	}
	printres(res, group_num*16, "ctr cipher");
	//base64(res, group_num*16);
}


int main()
{
	string s;
	cout << "请输入需要加密的字符串：" << endl;
	cin >> s;
	bitset<8> xor_value[16];
	for (int i = 0 ; i < 16; i++) {
		xor_value[i] = 0x00;
	};
	bitset<8> key[16] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0x01,0x01,0x01,0x01};
	clock_t start=clock();
	cbc_encry(s, key, xor_value, 16); //16bytes表示128bits 
	cout << "cbc time, 包括输出cbc密文的时间:" << clock()-start << "ms" << endl;
	start = clock();
	ctr_encry(s, key,16);
	cout << "ctr time, 包括输出ctr密文的时间:" << clock()-start << "ms" << endl;
	return 0;
} 
