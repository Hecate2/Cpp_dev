// 伯努利数
// 输入一个自然数m（0 <= m <= 18），输出从B0到Bm的伯努利数序列，以分数的格式输出。

// B0 = 1
// B1 = -1/2
// B2 = 1/6
// B3 = 0
// B4 = -1/30
// B5 = 0
// B6 = 1/42
// B7 = 0
// B8 = -1/30
// B9 = 0
// B10 = 5/66
// B11 = 0
// B12 = -691/2730
// B13 = 0
// B14 = 7/6
// B15 = 0
// B16 = -3617/510
// B17 = 0
// B18 = 43867/798
// B20 = -174611/330
// B22 = 854513/138
// B24 = -236364091/2730 //本程序使用int时在B24出错

#include <iostream>
using namespace std;

typedef long long ll;

namespace frac{
// 实际干活的话这个namespace放在别的文件……
// 实现最大公因数，最小公倍数和分数的计算。重载了+ - * / ~(取倒数)，还可以有+=和=等。
// 推荐T==long long 或 int
	template <typename T>
	T gcd(T x,T y){
		//最大公因数；非负
		if(x==0 || y==0) {return 0;}
		if(x<0){x=-x;}
		if(y<0){y=-y;}
	   	while(x!=y){
			if(x>y){
				x=x-y;
			}else{
				y=y-x;
			}
	   }
	   return x;
	} 

	template <typename T>
	T lcm(T x,T y){
		//最小公倍数；可为负
		if(x==0 || y==0){return 0;}
		return x/gcd(x,y)*y;
	}
	
	template<typename T = int>
	class fraction{
	public:
		T up,down; //分子分母

		//以下是构造函数
		fraction<T>(T u,T d){ up=u,down=d;	reduction();}
		fraction<T>(T u){ up=u,down=1; }
		fraction<T>(){ up=0,down=1;}

		//以下是基本方法
		fraction<T> reduction(){  //约分，并保持负号在分子上
			if(down==0){ throw "Division by zero!"; }
			if(up==0){up=0, down=1; return *this;}
			if(down<0){ up=-up,down=-down; }
			T gcd_result=gcd(up,down); //处理了负数情况
			up/=gcd_result,down/=gcd_result;
			return *this;
		}

		//以下是运算符重载
		fraction<T> operator + (fraction<T> another_frac){	//分数与分数
			fraction<T> ans=fraction(this->up * another_frac.down + this->down * another_frac.up,
				this->down * another_frac.down);
			return ans.reduction();
		}
		fraction<T> operator - (fraction<T> another_frac){
			fraction<T> ans=fraction(this->up * another_frac.down - this->down * another_frac.up,
				this->down * another_frac.down);
			return ans.reduction();
		}
		fraction<T> operator - (){ //自身取负号
			fraction<T> ans=fraction(-(this->up),this->down);
			return ans;
		}
		fraction<T> operator * (fraction<T> another_frac){	//分数与分数
			//避免溢出：交叉约分
			fraction<T> ans1=fraction(this->up, another_frac.down);
			fraction<T> ans2=fraction(another_frac.up, this->down);
			fraction<T> ans=fraction(ans1.up * ans2.up, ans1.down * ans2.down);
			return ans;
		}
		fraction<T> operator ~ (){ //自身取倒数
			fraction<T> ans=fraction(this->down,this->up);
			return ans;
		}
		fraction<T> operator / (fraction<T> another_frac){	//分数与分数
			//乘以倒数
			return (*this) * (~another_frac);
		}
	};

	template<typename T>
	fraction<T> operator + (T adder,fraction<T> frac_adder){	//分数与一般数
		fraction<T> ans;
		ans.down=frac_adder.down;
		ans.up=adder * frac_adder.down + frac_adder.up;
		return ans.reduction();
	}
	template<typename T>
	fraction<T> operator + (fraction<T> frac_adder,T adder){	//分数与一般数
		fraction<T> ans;
		ans.down=frac_adder.down;
		ans.up=adder * frac_adder.down + frac_adder.up;
		return ans.reduction();
	}
	template<typename T>
	fraction<T> operator - (T adder,fraction<T> frac_adder){
		fraction<T> ans;
		ans.down=frac_adder.down;
		ans.up=adder * frac_adder.down - frac_adder.up;
		return ans.reduction();
	}
	template<typename T>
	fraction<T> operator - (fraction<T> frac_adder,T adder){
		fraction<T> ans;
		ans.down=frac_adder.down;
		ans.up=frac_adder.up - adder * frac_adder.down;
		return ans.reduction();
	}
	template<typename T>
	fraction<T> operator * (T adder,fraction<T> frac_adder){	//分数与一般数
		fraction<T> ans=fraction<T>(adder, frac_adder.down);
		ans.up = ans.up * frac_adder.up;
		return ans;
	}
	template<typename T>
	fraction<T> operator * (fraction<T> frac_adder,T adder){	//分数与一般数
		fraction<T> ans=fraction<T>(adder, frac_adder.down);
		ans.up = ans.up * frac_adder.up;
		return ans;
	}
	template<typename T>
	fraction<T> operator / (T adder,fraction<T> frac_adder){	//分数与一般数
		fraction<T> reciprocal=fraction<T>(frac_adder.down, frac_adder.up);
		return adder * reciprocal;
	}
	template<typename T>
	fraction<T> operator / (fraction<T> frac_adder,T adder){	//分数与一般数
		fraction<T> reciprocal=fraction<T>(1, adder);
		return frac_adder * reciprocal;
	}
	//以下是输出
	template<typename T>
	ostream& operator << (ostream& output,fraction<T> the_frac){
		if(the_frac.up==0){
			output<<0;
			return output;
		}
		if(the_frac.down==1){
			output<<the_frac.up;
			return output;
		}
		output<<the_frac.up<<'/'<<the_frac.down;
		return output;
	}
}



using namespace frac;

class Bernoulli{
// 现在只用int计算。加上模板可用long long……
private: 
	bool _calculated = 0; //是否计算过伯努利数
	int _expand_binomial_coefficient(int index)
	//输入index：binomial_coefficient中最后一个有效项的下标是多少
	//本函数将计算新一组二项式系数(index+=1后的二项式系数)
	//例如index==2; 1 2 1 -> 1 3 3 1
	{
		// binomial_coefficient[0]无需变化
		int i;
		int temp_prev=binomial_coefficient[0], temp=binomial_coefficient[1];
		for(i=1; i<=index; ++i)
		{
			binomial_coefficient[i] = temp_prev+temp;
			temp_prev=temp;
			temp=binomial_coefficient[i+1];
		}
		// 现在i==index+1
		binomial_coefficient[i] = temp_prev;

		return i;
	}

public:
	int *binomial_coefficient; //二项式系数数组
	fraction<int> *bernoulli_num; //存储之前计算出的所有伯努利数
	int max_index; //本class计算的伯努利系数的项数(B[0]到B[max_index])

	//以下是构造函数
	Bernoulli(int m)
	{
		binomial_coefficient = new int[m+2];
		bernoulli_num = new fraction<int>[m+1];
		max_index = m;
	}

	fraction<int> calc()
	{
		//初始化二项式系数
		binomial_coefficient[0] = binomial_coefficient[1] = 1;
		//初始化伯努利数第1项
		bernoulli_num[0] = fraction<int>(1);

		int i=0;
		fraction<int> sum;
		while(i<max_index)
		{//直接使用递推式计算。算法比较暴力……
			i+=1;
			_expand_binomial_coefficient(i);
			sum=fraction<int>(0); //整数赋值运算符'='尚未重载
			for(int prev_index=0; prev_index<i; ++prev_index)
			{
				sum = sum+binomial_coefficient[prev_index]*bernoulli_num[prev_index];
				//运算符+=尚未重载
			}
			bernoulli_num[i] = -sum/binomial_coefficient[i];
		}
		_calculated = 1;
		return bernoulli_num[i];
	}

	fraction<int> *print_all_bernoulli()
	//输出全部伯努利数
	{
		if(!(_calculated)){
			calc();
		}
		for(int i=0; i<=max_index; ++i)
		{
			cout<<"B"<<i<<" = "<<bernoulli_num[i]<<endl;
		}
		return bernoulli_num;
	}
};

int main()//int argc,char *argv[])
{
	// cout<<fraction<int>(-1,3)+1<<endl;
	// cout<<1-fraction<int>(-1,3)<<endl;
	// cout<<fraction<int>(2,3)-fraction<int>(-1,3)<<endl;
	// cout<<fraction<int>(2,3)*fraction<int>(-1,3)<<endl;
	// cout<<fraction<int>(2,3)/fraction<int>(-1,3)<<endl;
	// cout<<fraction<int>(1,2)/3<<endl;
	// cout<<-3/fraction<int>(1,2)<<endl;
	// cout<<4*fraction<int>(-1,2)<<endl;
	// cout<<fraction<int>(1)+4*fraction<int>(-1,2)<<endl;

	int m;

	cout<<"本程序求伯努利数列的前m项。推荐m小于等于26。"<<endl;
	cout<<"输入伯努利数列的项数m："<<endl;
	// m=18;
	cin>>m;
	while(m<0)
	{
		cout<<"请输入正整数！伯努利数列的项数m："<<endl;
		cin>>m;
	}

	Bernoulli bernoulli_calculator = Bernoulli(m);
	// cout<<bernoulli_calculator.calc()<<endl;
	// bernoulli_calculator.calc();
	bernoulli_calculator.print_all_bernoulli();  //输出所有项
}
