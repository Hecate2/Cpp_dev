// ѭ��С��
// ����2����Ȼ������Ȼ��n��d����ʮ����С����ʽ���n����d�Ľ����
// ����ѭ��С���������Ž�ѭ����������
// ����0�����������������
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

void repetend(int remainder, int d){//��ѭ����
    if(remainder < 0) {remainder = -remainder;}  //����Ѿ�תΪ�����ˣ���Ϊ����������������������ط�������תһ������
    if(d < 0) {d = -d;}
    if (d == 0) {printf("value error: d==0\n"); throw "value error: d==0";}
    if(remainder >= d) {printf("value error: remainder>=d"); throw "value error: remainder>=d";}

    //��������ΪС����ʼ����¼ÿ�γ�����{����:λ��}
    map<int,int> remainder_to_index; // {remainder:index}   1/7 -> .142857 -> {1:0, 4:1, 2:2, 8:3, 5:4, 7:5}
    map<int,int>::iterator it_remainder; // for(it_remainder=Map.begin();it!=Map.end();it++){cout<<it->first<<"    "<<it->second<<endl;}
    vector<int> result; //ÿһλС����� [1,4,2,8,5,7]

    int index=0;
    do{
        remainder_to_index.insert(pair<int,int>(remainder, index++));
        remainder = remainder*10;
        result.insert(result.end(), 1, remainder/d);
        remainder = remainder%d;

        if(remainder == 0){  //����������û��ѭ��С��
            goto NoRemainder;
        }

        it_remainder = remainder_to_index.find(remainder);
        if(it_remainder != remainder_to_index.end()){  //remainder��remainder_to_index�г��ֹ�
            goto HasRemainder;
        }

    }while(1);

NoRemainder:
    for (int i=0; i<index;++i) {
        printf("%d", result[i]);
    }
    return;

HasRemainder:
    //С��һ��������������999983�������terminal��Ҫprint�þ�
    //���Լ���1/1296341�����ߵ�û����
    // if(index > 65535){
    //     printf("С������̫���ˡ������repretend.txt\n");
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
    //printf("\n�����ѭ��С����");
    return;
}

void print_repretend(int n, int d){
    if(n<0 xor d<0){  //���ս���Ǹ���
        printf("-");
    }

    if(n < 0) {n = -n;}
    if(d < 0) {d = -d;}
    int int_part = n/d;
    printf("%d", int_part);

    int remainder = n%d;
    if (remainder == 0){ //�������־ͳ�����
        printf("\n");
        return;
    }
    else{ //��С������
        printf(".");
        repetend(remainder,d);
        printf("\n");
    }
}

int main()//int argc,char *argv[])
{
    printf("����2����Ȼ��n��d����ʮ����С����ʽ���n����d�Ľ����\n");
    printf("ѭ����������()�У�����10/3���3.(3)\n");
    printf("����0��С�����ַ������������\n");
    printf("���Լ���1/1296341�����ߵ�û����\n");
    int n,d;
    while(1) {
        printf(">>> ");
        cin >> n;
        if(n==0) {break;} // also breaks when a string is input
        cin >> d;
        if(d==0) {printf("����d�����쳣\n"); break;}

        print_repretend(n,d);
    }
    printf("���������\n");
}
