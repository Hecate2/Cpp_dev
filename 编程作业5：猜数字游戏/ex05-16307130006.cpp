// ��C++���Ա�дһ������ʵ�ֲ�������Ϸ��
// �����������һ��û���ظ����ֵĵ�4λ���֣��������¡�
// �û�ÿ��һ�Σ�����͸���������ָ�����ʽΪmAnB�Ľ��������m��ʾλ����ȷ�����ĸ�������n��ʾ������ȷ��λ�ò��Ե����ĸ�����
// ����ȷ��Ϊ5234�����˲µ���5346��������1A2B��������һ��5��λ�ö��ˣ���Ϊ1A����3��4���������ֶ��ˣ���λ��û�ԣ���˼�Ϊ2B������������1A2B��
// �����û��ٸ���mAnB�Ľ�������£�ֱ������Ϊֹ��

// ע1���û�������������ظ����֡�������ȷ��Ϊ1234�����˲µ���3333��������1A3B��
// ע2��C++���������÷��ɲο�C++ Primer��5�棨17.4�ڣ�����ο���վhttp://en.cppreference.com/w/cpp/numeric/random

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
using namespace std;
#include <math.h>       //pow

class GuessNumGame{
private:
    vector<int> generated_num;
    int generated_digits;
    vector<int> input_num;
    map<int,int> num_to_index;

    struct Result{
        // to generate "mAnB"
        int a,b;
    };

    vector<int> get_input(){
        printf(">>> ");
        int input_num;
        while(!(cin>>input_num)){cin.clear();cin.sync();    printf("������%dλ�����������ʽ����ʱ����֤�����ȷ\n>>> ", generated_digits);}
        if(input_num >= pow(10, generated_digits)){printf("�������λ���е�ࡣ��������λ��������������������λΪ���жϽ��\n");}
        if(input_num <  pow(10, generated_digits-1)){printf("������������ĸ�λ��0��Ϊ���жϽ��\n");}
        return vector<int> {input_num/1000 % 10, input_num/100 % 10, input_num/10 % 10, input_num/1 % 10};
    }

    Result judge(){
        int a=0,b=0;
        for(int i=0; i<generated_digits; ++i){
            if(input_num[i] == generated_num[i]){++a;}
            if(num_to_index.find(input_num[i]) != num_to_index.end() and num_to_index[input_num[i]] != i){++b;}
        }
        // printf("%d %d\n", a,b);
        return Result{a,b};
    }

    void start_game(){
        Result result;
        while(1){
            input_num = get_input();
            result = judge();
            printf("%dA%dB\n", result.a, result.b);
            if(result.a == generated_digits){
                printf("��ϲ���¶��ˣ�\n");
                return;
            }
        }
    }

public:
    GuessNumGame(int digits = 4){
        srand(unsigned(time(0))); //��������ӳ�ʼ��

        generated_digits = digits;
        vector<int> base_vec = {0,1,2,3,4,5,6,7,8,9};
        random_shuffle(base_vec.begin(), base_vec.end(), [](int d){ return rand() % d; });
        generated_num.assign(base_vec.begin(),base_vec.begin() + digits);
        for(int i=0; i<digits;++i){
            num_to_index.insert(pair<int,int>(generated_num[i],i));
        }

        printf("���ɵ���Ϊ��\n");
        for_each(generated_num.cbegin(), generated_num.cend(), [](const int& val)->void{printf("%d", val);});
        printf("\n");

        start_game();
    }
};


int main(){
    GuessNumGame game = GuessNumGame();

}