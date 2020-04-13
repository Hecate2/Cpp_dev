// 用C++语言编写一个程序，实现猜数字游戏。
// 程序随机产生一个没有重复数字的的4位数字，由人来猜。
// 用户每猜一次，程序就根据这个数字给出形式为mAnB的结果，其中m表示位置正确的数的个数，而n表示数字正确而位置不对的数的个数。
// 如正确答案为5234，而人猜的是5346，则结果是1A2B，其中有一个5的位置对了，记为1A，而3和4这两个数字对了，而位置没对，因此记为2B，合起来就是1A2B。
// 接着用户再根据mAnB的结果继续猜，直到猜中为止。

// 注1：用户的输入可以有重复数字。例如正确答案为1234，而人猜的是3333，则结果是1A3B。
// 注2：C++随机数库的用法可参看C++ Primer第5版（17.4节），或参考网站http://en.cppreference.com/w/cpp/numeric/random

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
        while(!(cin>>input_num)){cin.clear();cin.sync();    printf("请输入%d位整数！输入格式错误时不保证结果正确\n>>> ", generated_digits);}
        if(input_num >= pow(10, generated_digits)){printf("您输入的位数有点多。请输入四位数。现用您输入的最后四位为您判断结果\n");}
        if(input_num <  pow(10, generated_digits-1)){printf("将您输入的数的高位补0，为您判断结果\n");}
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
                printf("恭喜，猜对了！\n");
                return;
            }
        }
    }

public:
    GuessNumGame(int digits = 4){
        srand(unsigned(time(0))); //随机数种子初始化

        generated_digits = digits;
        vector<int> base_vec = {0,1,2,3,4,5,6,7,8,9};
        random_shuffle(base_vec.begin(), base_vec.end(), [](int d){ return rand() % d; });
        generated_num.assign(base_vec.begin(),base_vec.begin() + digits);
        for(int i=0; i<digits;++i){
            num_to_index.insert(pair<int,int>(generated_num[i],i));
        }

        printf("生成的数为：\n");
        for_each(generated_num.cbegin(), generated_num.cend(), [](const int& val)->void{printf("%d", val);});
        printf("\n");

        start_game();
    }
};


int main(){
    GuessNumGame game = GuessNumGame();

}