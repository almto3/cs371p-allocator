// --------
// includes
// --------

#include <iostream> // istream, ostream
#include <sstream>  // getline
#include <string>   // string
#include <cassert>  // assert
#include <cstring>  // stoi
#include <vector>   // string

#include "Allocator.h"

using namespace std;


vector<unsigned char> intToBytes(int paramInt)
{
     vector<unsigned char> arrayOfByte(4);
     for (int i = 0; i < 4; i++)
         arrayOfByte[3 - i] = (paramInt >> (i * 8));
     return arrayOfByte;
}