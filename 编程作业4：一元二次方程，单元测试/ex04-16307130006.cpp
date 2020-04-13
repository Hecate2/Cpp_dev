//编写一个函数，求解一元二次方程ax2+bx+c=0.
//当系数错误（a=0）或方程无解（b2-4ac<0）时抛出异常。
//采用本课程的简单测试框架对解方程函数进行单元测试。
//测试用例可按照黑盒法或白盒法设计。
//若采用黑盒法，在程序中用注释说明等价类的划分；若采用白盒法，说明测试达到的覆盖标准。

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
        //下面的并不会执行
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
        // 主要用于两个浮点数vector比大小
        // 以常数1e-6为精度标准
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
        // 用于浮点数vector向量输出为字符串
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
    // printf("------------黑盒测试------------\n");
    //等价类1：a==0；预期结果：DivZero
    CHECK_THROW(solve_quadratic(0,0,0),DivZero);    //等价类1：a==0
    //等价类2：a!=0 && b*b-4*a*c<0；预期结果：NoSolution
    CHECK_THROW(solve_quadratic(10,19.999,10),NoSolution);    //等价类2：a!=0 && b*b-4*a*c<0，边界值
    CHECK_THROW(solve_quadratic(1,1,100),NoSolution);    //等价类2：a!=0 && b*b-4*a*c<0
    //等价类3：a!=0 && b*b-4*a*c>=0；预期结果：正常求解
    vector<double> ans;
    ans={-1,-1};
    CHECK_EQUAL(solve_quadratic(10,20,10),ans);  //等价类3：a!=0 && b*b-4*a*c>=0；边界值：解为包含2个-1的vector
    ans={3,5};
    CHECK_EQUAL(solve_quadratic(1,-8,15),ans);  //等价类3：a!=0 && b*b-4*a*c>=0
    ans={-2.414213562,0.4142135624};
    CHECK_EQUAL(solve_quadratic(1,2,-1),ans);  //等价类3：a!=0 && b*b-4*a*c>=0；小数舍入误差
    // printf("------------白盒测试------------\n");
    CHECK_THROW(solve_quadratic(0,0,0),DivZero);    //覆盖代码：a==0
    CHECK_THROW(solve_quadratic(10,19.999,10),NoSolution);    //b*b-4*a*c<0
    CHECK_THROW(solve_quadratic(1,1,100),NoSolution);    //b*b-4*a*c<0
    ans={-1,-1};
    CHECK_EQUAL(solve_quadratic(10,20,10),ans);  //正常求解
    ans={3,5};
    CHECK_EQUAL(solve_quadratic(1,-8,15),ans);  //正常求解
    ans={-2.414213562,0.4142135624};
    CHECK_EQUAL(solve_quadratic(1,2,-1),ans);  //正常求解；小数舍入误差
    return 0;
}

// int main()//int argc,char *argv[])
// {
//     //main
// }