// 循环小数
// 读入2个自然数个自然数n和d，以十进制小数形式输出n除以d的结果。
// 对于循环小数，用括号将循环节括起来
// 输入0代表输入结束。例：
// >>> 9 3
// 3
// >>> 15 7
// 2.(142857)
// >>> 5 42
// 0.1(190476)
// >>> 123456 500
// 246.912
// >>> 0


#include <iostream>
#include <map>
#include <vector>
using namespace std;
#include <stdio.h>

void repetend(int remainder, int d){//求循环节
    if(remainder < 0) {remainder = -remainder;}  //外层已经转为正数了，但为了这个函数可以用在其他地方，还是转一次正数
    if(d < 0) {d = -d;}
    if (d == 0) {printf("value error: d==0\n"); throw "value error: d==0";}
    if(remainder >= d) {printf("value error: remainder>=d"); throw "value error: remainder>=d";}

    //从输出结果为小数开始，记录每次除法的{余数:位数}
    map<int,int> remainder_to_index; // {remainder:index}   1/7 -> .142857 -> {1:0, 4:1, 2:2, 8:3, 5:4, 7:5}
    map<int,int>::iterator it_remainder; // for(it_remainder=Map.begin();it!=Map.end();it++){cout<<it->first<<"    "<<it->second<<endl;}
    vector<int> result; //每一位小数结果 [1,4,2,8,5,7]

    int index=0;
    do{
        remainder_to_index.insert(pair<int,int>(remainder, index++));
        remainder = remainder*10;
        result.insert(result.end(), 1, remainder/d);
        remainder = remainder%d;

        if(remainder == 0){  //除法结束，没有循环小数
            goto NoRemainder;
        }

        it_remainder = remainder_to_index.find(remainder);
        if(it_remainder != remainder_to_index.end()){  //remainder在remainder_to_index中出现过
            goto HasRemainder;
        }

    }while(1);

NoRemainder:
    for (int i=0; i<index;++i) {
        printf("%d", result[i]);
    }
    return;

HasRemainder:
    //小于一百万的最大质数是999983，输出到terminal需要print好久
    //可以计算1/1296341，更高的没测试
    // if(index > 65535){
    //     printf("小数部分太长了。输出到repretend.txt\n");
    //     FILE*fp=fopen("repretend.txt","w");
    //     NotImplemented
    // }
    for (int i=0; i<index;++i) {
        if(i == (it_remainder->second)){
            printf("(");
        }

        printf("%d", result[i]);

        if (i == index-1) {
            printf(")");
        }
    }
    //printf("\n结果是循环小数！");
    return;
}

void print_repretend(int n, int d){
    if(n<0 xor d<0){  //最终结果是负数
        printf("-");
    }

    if(n < 0) {n = -n;}
    if(d < 0) {d = -d;}
    int int_part = n/d;
    printf("%d", int_part);

    int remainder = n%d;
    if (remainder == 0){ //整数部分就除完了
        printf("\n");
        return;
    }
    else{ //求小数部分
        printf(".");
        repetend(remainder,d);
        printf("\n");
    }
}

int main()//int argc,char *argv[])
{
    printf("读入2个自然数n和d，以十进制小数形式输出n除以d的结果。\n");
    printf("循环节在括号()中，例如10/3输出3.(3)\n");
    printf("输入0或小数或字符代表输入结束\n");
    printf("可以计算1/1296341，更高的没测试\n");
    int n,d;
    while(1) {
        printf(">>> ");
        cin >> n;
        if(n==0) {break;} // also breaks when a string is input
        cin >> d;
        if(d==0) {printf("除数d输入异常\n"); break;}

        print_repretend(n,d);
    }
    printf("输入结束。\n");
}
