// ��Ŭ����
// ����һ����Ȼ��m��0 <= m <= 18���������B0��Bm�Ĳ�Ŭ�������У��Է����ĸ�ʽ�����

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
// B24 = -236364091/2730 //������ʹ��intʱ��B24����

#include <iostream>
using namespace std;

typedef long long ll;

namespace frac{
// ʵ�ʸɻ�Ļ����namespace���ڱ���ļ�����
// ʵ�������������С�������ͷ����ļ��㡣������+ - * / ~(ȡ����)����������+=��=�ȡ�
// �Ƽ�T==long long �� int
	template <typename T>
	T gcd(T x,T y){
		//����������Ǹ�
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
		//��С����������Ϊ��
		if(x==0 || y==0){return 0;}
		return x/gcd(x,y)*y;
	}
	
	template<typename T = int>
	class fraction{
	public:
		T up,down; //���ӷ�ĸ

		//�����ǹ��캯��
		fraction<T>(T u,T d){ up=u,down=d;	reduction();}
		fraction<T>(T u){ up=u,down=1; }
		fraction<T>(){ up=0,down=1;}

		//�����ǻ�������
		fraction<T> reduction(){  //Լ�֣������ָ����ڷ�����
			if(down==0){ throw "Division by zero!"; }
			if(up==0){up=0, down=1; return *this;}
			if(down<0){ up=-up,down=-down; }
			T gcd_result=gcd(up,down); //�����˸������
			up/=gcd_result,down/=gcd_result;
			return *this;
		}

		//���������������
		fraction<T> operator + (fraction<T> another_frac){	//���������
			fraction<T> ans=fraction(this->up * another_frac.down + this->down * another_frac.up,
				this->down * another_frac.down);
			return ans.reduction();
		}
		fraction<T> operator - (fraction<T> another_frac){
			fraction<T> ans=fraction(this->up * another_frac.down - this->down * another_frac.up,
				this->down * another_frac.down);
			return ans.reduction();
		}
		fraction<T> operator - (){ //����ȡ����
			fraction<T> ans=fraction(-(this->up),this->down);
			return ans;
		}
		fraction<T> operator * (fraction<T> another_frac){	//���������
			//�������������Լ��
			fraction<T> ans1=fraction(this->up, another_frac.down);
			fraction<T> ans2=fraction(another_frac.up, this->down);
			fraction<T> ans=fraction(ans1.up * ans2.up, ans1.down * ans2.down);
			return ans;
		}
		fraction<T> operator ~ (){ //����ȡ����
			fraction<T> ans=fraction(this->down,this->up);
			return ans;
		}
		fraction<T> operator / (fraction<T> another_frac){	//���������
			//���Ե���
			return (*this) * (~another_frac);
		}
	};

	template<typename T>
	fraction<T> operator + (T adder,fraction<T> frac_adder){	//������һ����
		fraction<T> ans;
		ans.down=frac_adder.down;
		ans.up=adder * frac_adder.down + frac_adder.up;
		return ans.reduction();
	}
	template<typename T>
	fraction<T> operator + (fraction<T> frac_adder,T adder){	//������һ����
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
	fraction<T> operator * (T adder,fraction<T> frac_adder){	//������һ����
		fraction<T> ans=fraction<T>(adder, frac_adder.down);
		ans.up = ans.up * frac_adder.up;
		return ans;
	}
	template<typename T>
	fraction<T> operator * (fraction<T> frac_adder,T adder){	//������һ����
		fraction<T> ans=fraction<T>(adder, frac_adder.down);
		ans.up = ans.up * frac_adder.up;
		return ans;
	}
	template<typename T>
	fraction<T> operator / (T adder,fraction<T> frac_adder){	//������һ����
		fraction<T> reciprocal=fraction<T>(frac_adder.down, frac_adder.up);
		return adder * reciprocal;
	}
	template<typename T>
	fraction<T> operator / (fraction<T> frac_adder,T adder){	//������һ����
		fraction<T> reciprocal=fraction<T>(1, adder);
		return frac_adder * reciprocal;
	}
	//���������
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
// ����ֻ��int���㡣����ģ�����long long����
private: 
	bool _calculated = 0; //�Ƿ�������Ŭ����
	int _expand_binomial_coefficient(int index)
	//����index��binomial_coefficient�����һ����Ч����±��Ƕ���
	//��������������һ�����ʽϵ��(index+=1��Ķ���ʽϵ��)
	//����index==2; 1 2 1 -> 1 3 3 1
	{
		// binomial_coefficient[0]����仯
		int i;
		int temp_prev=binomial_coefficient[0], temp=binomial_coefficient[1];
		for(i=1; i<=index; ++i)
		{
			binomial_coefficient[i] = temp_prev+temp;
			temp_prev=temp;
			temp=binomial_coefficient[i+1];
		}
		// ����i==index+1
		binomial_coefficient[i] = temp_prev;

		return i;
	}

public:
	int *binomial_coefficient; //����ʽϵ������
	fraction<int> *bernoulli_num; //�洢֮ǰ����������в�Ŭ����
	int max_index; //��class����Ĳ�Ŭ��ϵ��������(B[0]��B[max_index])

	//�����ǹ��캯��
	Bernoulli(int m)
	{
		binomial_coefficient = new int[m+2];
		bernoulli_num = new fraction<int>[m+1];
		max_index = m;
	}

	fraction<int> calc()
	{
		//��ʼ������ʽϵ��
		binomial_coefficient[0] = binomial_coefficient[1] = 1;
		//��ʼ����Ŭ������1��
		bernoulli_num[0] = fraction<int>(1);

		int i=0;
		fraction<int> sum;
		while(i<max_index)
		{//ֱ��ʹ�õ���ʽ���㡣�㷨�Ƚϱ�������
			i+=1;
			_expand_binomial_coefficient(i);
			sum=fraction<int>(0); //������ֵ�����'='��δ����
			for(int prev_index=0; prev_index<i; ++prev_index)
			{
				sum = sum+binomial_coefficient[prev_index]*bernoulli_num[prev_index];
				//�����+=��δ����
			}
			bernoulli_num[i] = -sum/binomial_coefficient[i];
		}
		_calculated = 1;
		return bernoulli_num[i];
	}

	fraction<int> *print_all_bernoulli()
	//���ȫ����Ŭ����
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

	cout<<"��������Ŭ�����е�ǰm��Ƽ�mС�ڵ���26��"<<endl;
	cout<<"���벮Ŭ�����е�����m��"<<endl;
	// m=18;
	cin>>m;
	while(m<0)
	{
		cout<<"����������������Ŭ�����е�����m��"<<endl;
		cin>>m;
	}

	Bernoulli bernoulli_calculator = Bernoulli(m);
	// cout<<bernoulli_calculator.calc()<<endl;
	// bernoulli_calculator.calc();
	bernoulli_calculator.print_all_bernoulli();  //���������
}
