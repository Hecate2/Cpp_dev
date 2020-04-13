// 使用C或C++语言编写一个程序，输出1到100的数字。但是遇到数字为3的倍数时，
// 输出“Fizz”替代数字，5的倍数用“Buzz”代替，既是3的倍数又是5的倍数时输出“FizzBuzz”：
// 1
// 2
// Fizz
// 4
// Buzz
// Fizz
// ... ...

#include <iostream>
using namespace std;

int main(int argc,char *argv[])
{
	unsigned char mod5,mod3;
	for(int i=1; i<101; ++i)
	{
		mod3=!(i%3);	//是3的倍数
		mod5=!(i%5);	//是5的倍数
		if(mod3 && mod5)
		{
			cout<<"FizzBuzz"<<endl;
			continue;
		}
		if(mod3)
		{
			cout<<"Fizz"<<endl;
			continue;
		}
		if(mod5)
		{
			cout<<"Buzz"<<endl;
			continue;
		}
		cout<<i<<endl;
	}
}
