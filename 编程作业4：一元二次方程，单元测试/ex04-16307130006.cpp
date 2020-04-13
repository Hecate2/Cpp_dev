//��дһ�����������һԪ���η���ax2+bx+c=0.
//��ϵ������a=0���򷽳��޽⣨b2-4ac<0��ʱ�׳��쳣��
//���ñ��γ̵ļ򵥲��Կ�ܶԽⷽ�̺������е�Ԫ���ԡ�
//���������ɰ��պںз���׺з���ơ�
//�����úںз����ڳ�������ע��˵���ȼ���Ļ��֣������ð׺з���˵�����Դﵽ�ĸ��Ǳ�׼��

#include <iostream>
#include <sstream> // using std::stringstream;
#include <stdexcept>
#include <vector>
#include "simple_test.h"
using namespace std;
#include <math.h>
#include <stdio.h>

struct DivZero{};
struct NoSolution{};

vector<double> solve_quadratic(double a,double b,double c){
    if(a==0){
        throw DivZero{};
        //����Ĳ�����ִ��
        if(b==0){
            throw DivZero{};
        }
        return vector<double> {-c/b};
    }
    if(b*b-4*a*c<0){
        throw NoSolution{};
    }
    return vector<double> {
        (-b-sqrt(b*b-4*a*c))/(2*a),
        (-b+sqrt(b*b-4*a*c))/(2*a),
    };
}

namespace SimpleTest {
    template<typename T>
    bool operator == (vector<T> lhs, vector<T> rhs){
        // ��Ҫ��������������vector�ȴ�С
        // �Գ���1e-6Ϊ���ȱ�׼
        int s = lhs.size();
        if(rhs.size()!=s){return false;}
        else{
            for(int i=0;i<s;++i){
                if(not(-1e-6<lhs[i]-rhs[i] and lhs[i]-rhs[i]<1e-6)){return false;}
            }
        }
        return true;
    }
    template<typename T>
    ostream& operator << (ostream& lhs, vector<T> rhs){
        // ���ڸ�����vector�������Ϊ�ַ���
        int s=rhs.size();
        if(s==0){return lhs;}
        stringstream fmt;
        fmt << rhs[0];
        for(int i=1;i<s;++i){
            fmt << "," << rhs[i];
        }
        return fmt;
    }
}

int test_main(){
    // printf("------------�ںв���------------\n");
    //�ȼ���1��a==0��Ԥ�ڽ����DivZero
    CHECK_THROW(solve_quadratic(0,0,0),DivZero);    //�ȼ���1��a==0
    //�ȼ���2��a!=0 && b*b-4*a*c<0��Ԥ�ڽ����NoSolution
    CHECK_THROW(solve_quadratic(10,19.999,10),NoSolution);    //�ȼ���2��a!=0 && b*b-4*a*c<0���߽�ֵ
    CHECK_THROW(solve_quadratic(1,1,100),NoSolution);    //�ȼ���2��a!=0 && b*b-4*a*c<0
    //�ȼ���3��a!=0 && b*b-4*a*c>=0��Ԥ�ڽ�����������
    vector<double> ans;
    ans={-1,-1};
    CHECK_EQUAL(solve_quadratic(10,20,10),ans);  //�ȼ���3��a!=0 && b*b-4*a*c>=0���߽�ֵ����Ϊ����2��-1��vector
    ans={3,5};
    CHECK_EQUAL(solve_quadratic(1,-8,15),ans);  //�ȼ���3��a!=0 && b*b-4*a*c>=0
    ans={-2.414213562,0.4142135624};
    CHECK_EQUAL(solve_quadratic(1,2,-1),ans);  //�ȼ���3��a!=0 && b*b-4*a*c>=0��С���������
    // printf("------------�׺в���------------\n");
    CHECK_THROW(solve_quadratic(0,0,0),DivZero);    //���Ǵ��룺a==0
    CHECK_THROW(solve_quadratic(10,19.999,10),NoSolution);    //b*b-4*a*c<0
    CHECK_THROW(solve_quadratic(1,1,100),NoSolution);    //b*b-4*a*c<0
    ans={-1,-1};
    CHECK_EQUAL(solve_quadratic(10,20,10),ans);  //�������
    ans={3,5};
    CHECK_EQUAL(solve_quadratic(1,-8,15),ans);  //�������
    ans={-2.414213562,0.4142135624};
    CHECK_EQUAL(solve_quadratic(1,2,-1),ans);  //������⣻С���������
    return 0;
}

// int main()//int argc,char *argv[])
// {
//     //main
// }