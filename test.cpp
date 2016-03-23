
/*
vector<bytes> h;

Allocator():

allocate ():


deallocate():

valid():

intToBytes():
*/
/*
	Little Endian = least significant digit last

*/


#include <iostream>
#include <vector>  
#include <cstdio>

using namespace std;


vector<char> intToBytes(int paramInt)
{
     vector<char> arrayOfByte(4);
     for (int i = 0; i < 4; i++)
         arrayOfByte[3 - i] = (paramInt >> (i * 8));
     return arrayOfByte;
}

int main(){

	int n = 5;
	cout << "n = " << n[0] << endl;



	char b[] = {0,0,0,1};

	vector <int> x;
	x.push_back(1);
	x.push_back(0);
	x.push_back(0);
	x.push_back(0);

	vector<char> y = intToBytes(1);

	int i = (x[3] << 24) | (x[2] << 16) | (x[1] << 8) | (x[0]);
	//i = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | (b[0]);

	cout << "y = " << y[0] << endl;
	cout << "y = " << y[1] << endl;
	cout << "y = " << y[2] << endl;
	cout << "y = " << y[3] << endl;
	cout << x[0] << endl;
	cout << i << endl;

	int num = 256;

	char s = (num >> (8*1)) & 0xff;

	printf("%d\n", s);

	return 0;
}
