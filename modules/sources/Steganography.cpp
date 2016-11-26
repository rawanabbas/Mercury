#include <stdio.h>
#include <stdlib.h>
#include<string>
using namespace std;
void encode()
{
	//first parameter is cover file, second is the file to be embedded/hidden and the third is the password
	system("steghide embed -cf input.jpg -ef input.txt -p test");
	system("steghide embed -cf default.jpg -ef input.jpg -p test");
}
void decode()
{
	//first parameter is image that we want to decrypt and the second is the key
	system("steghide extract -sf default.jpg -p test");
	system("steghide extract -sf input.jpg -p test");
}
int main()
{
	encode();
	decode();
	return 0;
}
