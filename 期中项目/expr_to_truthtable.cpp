//<表达式> ∷=  <项> & <项>
//          |  <项> | <项>
//          |  <项>
//          |  !<项>
//<项>    ∷=  <因子> & <因子>
//          |  <因子> | <因子>
//          |  <因子>
//          |  !<因子>
//<因子>  ∷=  <变量>
//          | （ <表达式> ）
//<变量>  ∷=  A | B | C | ...

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
using namespace std;
//#include <math.h>

//全局依赖：异常
namespace exceptions {
    struct SyntaxError{string msg;};
    void syntax_error(const string& msg){
        printf("%s\n", msg.c_str());
        throw SyntaxError{msg};
    }
    void syntax_error(const string& expr, int position){
        printf("Syntax Error at col %d\n", position);
        printf("%s\n", expr.c_str());
        for(int i=0; i<position; ++i) {printf("_");}
        printf("^ Error Here");

        stringstream errstr;
        errstr << "Syntax Error at col " << position;
        throw SyntaxError{errstr.str()};
    }
    void syntax_error(const string& expr, int position, const string& errmsg){
        printf("%s\n", errmsg.c_str());
        printf("Syntax Error at col %d\n", position);
        printf("%s\n", expr.c_str());
        for(int i=0; i<position; ++i) {printf("_");}
        printf("^ Error Here");
        throw SyntaxError{errmsg};
    }

    struct StackError{string msg;};
    struct ValueError{string msg;};
}
using namespace exceptions;

//全局依赖：运算符定义
namespace operators{
    struct OperatorInfo{
        char operator_itself;  //运算符本身
        int priority;   //优先级
        int ary;    //运算符目数
        OperatorInfo operator = (OperatorInfo b) {
            operator_itself = b.operator_itself;
            priority = b.priority;
            ary = b.ary;
            return *this;
        }
    };
    inline OperatorInfo get_operator_info(const char& operator_itself){
        //检查一个字符是否为允许的运算符，
        //并返回运算符相对优先级（用整数表示）和运算符目数
        switch(operator_itself){
            case '(': return OperatorInfo{operator_itself,-1073741824,0};
            case ')': return OperatorInfo{operator_itself,-1073741824,0};
            case '~': return OperatorInfo{operator_itself,4000,1};  // NOT(prefix)
            case '!': return OperatorInfo{operator_itself,4000,1};  // 额外运算符支持：!即为~
            case '&': return OperatorInfo{operator_itself,3000,2};  // AND
            case '*': return OperatorInfo{operator_itself,3000,2};  // 额外运算符支持：*即为&
            case '|': return OperatorInfo{operator_itself,2000,2};  // OR
            case '+': return OperatorInfo{operator_itself,2000,2};  // 额外运算符支持：+即为|
            case '^': return OperatorInfo{operator_itself,1000,2};  // XOR
            default:{
                stringstream fmt;
                fmt << "unexpected char: " << operator_itself;
                throw SyntaxError{fmt.str()};
            }
        }
    }
}
using namespace operators;


//以下关于输入表达式字符串的解析
//依赖namespace operators和namespace exceptions
class Parser{
public:
    static inline bool is_var(const char& letter){//是变量名
        if( (65<=letter && letter<=90) || (97<=letter && letter<=122) ) {
            // if(65<=letter && letter<=90){//是大写字母
            return 1;
        }else{
            return 0;
        }
    }

    static string infix_to_suffix(string expr)
    //中缀表达式转后缀表达式    //后缀表达式作为中间代码
    //遇到变量则直接往输出字符串末尾写入这个变量
    //遇到运算符则出栈，直到栈为空，或者栈顶的运算符优先级高于本运算符
    //被出栈的运算符写入输出字符串
    //然后把本运算符入栈
    {
        //TODO: string[i]不能作为char传入别的函数
        stack<OperatorInfo> operator_stack;  //运算符栈
        stringstream suffix;  //返回的后缀表达式

        int s=expr.size();
        OperatorInfo operator_current, operator_in_stack;
        //std::string标准中未规定需要'\0'作为字符串结尾
        for(int i=0;i<s;++i)
        {
            try{ // 不要把try移到for外面去。try会捕获语法错误的位置。
                if(expr[i]==' '){continue;} //无视空格
                if(is_var(expr[i])){//是变量名
                    suffix << expr[i];
                    continue;
                }
                operator_current = get_operator_info(expr[i]); //是运算符
                if(expr[i]=='('){
                    // printf("%s\n", "准备入栈");
                    // printf("%c %d %d\n", operator_current.operator_itself, operator_current.priority, operator_current.ary);
                    operator_stack.push(operator_current);
                    continue;
                }
                if(expr[i]==')'){
                    bool got_left = false;  // 在运算符栈里找到了左括号
                    while( not operator_stack.empty()) {
                        operator_in_stack = operator_stack.top();
                        if(operator_in_stack.operator_itself == '('){
                            operator_stack.pop();
                            got_left = true;
                            break;
                        }else{
                            operator_stack.pop();
                            if(operator_in_stack.ary > 0){
                                //后缀表达式里仅需要有实际运算功能的运算符，不需要括号
                                suffix << operator_in_stack.operator_itself;
                                // printf("%s\n", "写入后缀表达式");
                                // printf("%s\n", suffix.str().c_str());
                            }
                        }
                    }
                    if(got_left == false){
                        printf("括号有误。已忽略错误，继续解析表达式。不保证答案正确\n");
                    }
                    continue;
                }
                // 是运算符但不是括号
                while( not operator_stack.empty() ) {
                    //准备出栈。被出栈的运算符写入中缀表达式
                    operator_in_stack = operator_stack.top();
                    if(operator_in_stack.priority <= operator_current.priority){
                        //栈顶运算符优先级低于等于当前运算符。停止出栈
                        break;
                    }else{
                        // printf("%s\n", "准备出栈");
                        // printf("%c %d %d\n", operator_in_stack.operator_itself, operator_in_stack.priority, operator_in_stack.ary);
                        operator_stack.pop();
                        if(operator_in_stack.ary > 0){
                            //后缀表达式里仅需要有实际运算功能的运算符，不需要括号
                            suffix << operator_in_stack.operator_itself;
                            // printf("%s\n", "写入后缀表达式");
                            // printf("%s\n", suffix.str().c_str());
                        }
                    }
                }
                // printf("%s\n", "准备入栈");
                // printf("%c %d %d\n", operator_current.operator_itself, operator_current.priority, operator_current.ary);
                operator_stack.push(operator_current);
                continue;
            }
            catch(SyntaxError){
                syntax_error(expr, i, "unexpected char");
            }
        }
        //全部出栈
        // printf("全部出栈\n");
        while( not operator_stack.empty() ) {
            //准备出栈。被出栈的运算符写入中缀表达式
            operator_in_stack = operator_stack.top();
            // printf("%c\n", operator_in_stack.operator_itself);
            operator_stack.pop();
            if(operator_in_stack.ary > 0){
                //后缀表达式里仅需要有实际运算功能的运算符
                suffix << operator_in_stack.operator_itself;
            }
            else if(operator_in_stack.operator_itself == '(' or operator_in_stack.operator_itself == ')'){
                //全部出栈时不应该有括号
                printf("括号有误。已忽略错误，继续解析表达式。不保证答案正确\n");
            }
        }
        return suffix.str();
    }
};
//using namespace parser;

class Calculator{
public:
    int effective_operands;  //记录operands中前多少位是有效的
    string postfix_expr;  //外部输入的后缀表达式
    vector<bool> operands;
    //'A'的值为operands[0]。char some_letter的值为operands[some_letter-'A']

    //构造函数
    Calculator(int input_effective_operands, const string& input_postfix_expr) {
        postfix_expr = input_postfix_expr;
        effective_operands = input_effective_operands;
    }

    inline bool get_value(const char& name){
        //由变量名查询操作数当前值
        if(name-'A'>=effective_operands){
            string msg = "Error in parsing variable name.\nCheck VARIABLE NAMEs in your input expression.\n";
            printf(msg.c_str());
            printf("Unexpected variable name: \"%c\" from your input\n", name);
            printf("For input effective_operands==%d, only variable names from 'A' to 'A'+%d=='%c' is supported.\n",
                effective_operands,effective_operands, 'A'+effective_operands-1);
            throw StackError{msg};
        }
        return operands[name-'A'];
    }

    inline void inc_operands(){
        // 将operands视为一个二进制数。operands[0]为最低位。整个数+1
        bool carry = 1;  //来自上一级的进位
        bool next_carry;  //本级产生的进位，用于下一级
        for(int i=0;i<effective_operands;++i){
            if(operands[i]==1 and carry==1){
                next_carry = 1;
            } else {
                next_carry = 0;
            }
            operands[i] = operands[i] ^ carry; //异或"^"可实现0与1的加法
            if(next_carry) {  //本级产生了进位。需要下一级继续处理
                carry = next_carry;
                continue;
            }
            //本级没有产生进位
            break;
        }
    }

    inline void dec_operands(){
        // 将operands视为一个二进制数。operands[0]为最低位。整个数-1
        bool carry = 1;  //来自上一级的退位
        bool next_carry;  //本级产生的退位，用于下一级
        for(int i=0;i<effective_operands;++i){
            if(operands[i]==0 and carry==1){
                next_carry = 1;
            } else {
                next_carry = 0;
            }
            operands[i] = operands[i] ^ carry; //异或"^"可实现0与1的减法
            if(next_carry) {  //本级产生了退位。需要下一级继续处理
                carry = next_carry;
                continue;
            }
            //本级没有产生退位
            break;
        }
    }

    inline bool calc_one_operator(const vector<bool>& operands, const char& operator_itself){
        //对于单个运算符计算结果
        switch(operator_itself){
            case '~': return (!operands[0]);  // NOT(prefix)
            case '!': return (!operands[0]);  // 额外运算符支持：!即为~
            case '&': return (operands[0]&&operands[1]);  // AND
            case '*': return (operands[0]&&operands[1]);  // 额外运算符支持：*即为&
            case '|': return (operands[0]||operands[1]);  // OR
            case '+': return (operands[0]||operands[1]);  // 额外运算符支持：+即为|
            case '^': return (operands[0]^operands[1]);  // XOR
            default:{
                stringstream fmt;
                fmt << "unexpected char in postfix expression: " << operator_itself;
                throw SyntaxError{fmt.str()};
            }
        }
    }

    inline bool calc_postfix_expr(){
        int s=postfix_expr.size();
        if(s==0){
            syntax_error("It seems no effective expression was input.");
        }
        stack<bool> operand_stack;
        vector<bool> tmp_operands;
        vector<bool>::iterator tmp_operands_begin = tmp_operands.begin();
        int ary;  //当前运算符目数

        for(int i=0; i<s; ++i){
            if (Parser::is_var(postfix_expr[i])){
                operand_stack.push(get_value(postfix_expr[i]));
                continue;
            }
            ary = operators::get_operator_info(postfix_expr[i]).ary;  //如果不是运算符会直接throw exception
            tmp_operands.assign(ary,0);
            for(ary-=1; ary>=0; --ary){
                if(operand_stack.empty()){
                    string msg = "Error in calculating postfix expression.\nCheck redundant OPERATORs in your input expression.\n";
                    printf(msg.c_str());
                    printf("The postfix expression generated from your input:\n");
                    printf("%s\n",postfix_expr.c_str());
                    throw StackError{msg};
                }
                tmp_operands[ary] = operand_stack.top();
                operand_stack.pop();
            }
            operand_stack.push(calc_one_operator(tmp_operands, postfix_expr[i]));
        }
        bool result = operand_stack.top();
        operand_stack.pop();
        if(not operand_stack.empty()){
            string msg = "Error in calculating postfix expression.\nCheck redundant VARIABLEs in your input expression.\n";
            printf(msg.c_str());
            printf("The postfix expression generated from your input:\n");
            printf("%s\n",postfix_expr.c_str());
            throw StackError{msg};
        }
        return result;
    }

    inline string calc_truth_table(){
        operands.assign(effective_operands, 1);
        stringstream fmt;
        int calc_count = 1 << effective_operands;// pow(2,effective_operands);  // may skip the loop below if effective_operands is very large, for example, 31 in 32-bit programs
        for(int i=0; i<calc_count; ++i){
            // print directly or store results in fmt?
            fmt << calc_postfix_expr();
            dec_operands();
        }
        return fmt.str();
    }
};

std::string expr_to_truthtable(int n, const std::string& expr){
    try{
        return Calculator(n, Parser::infix_to_suffix(expr)).calc_truth_table();
    }catch(SyntaxError){
        // do nothing
    }catch(StackError){
        //do nothing
    }
}

int main()//int argc, char const *argv[])
{
    printf("输入 变量个数 和 逻辑表达式字符串 ，输出真值表字符串\n\n");
    printf("支持 单个大写字母 作为变量名\n");
    printf("支持运算符：\n~前缀NOT !前缀NOT\n&中缀AND *中缀AND\n|中缀OR  +中缀OR\n^中缀XOR\n和括号()\n\n");
    printf("示例：2 A&B -> AB& -> 1000\n");
    printf("示例：4 (A&B)|~C&D -> AB&C~D&| -> 1000111110001000\n");
    int effective_operands;
    string expr_in;
    while(1){
        printf(">>> ");
        cin >> effective_operands >> expr_in;
        while(cin.fail())
        {
            cin.clear();
            printf("输入错误。请输入一个正整数和一个逻辑表达式字符串。\n正整数表示逻辑表达式字符串中不同变量的个数\n>>> ");
            cin.ignore(1000000, '\n');
            cin >> effective_operands >> expr_in;
        }
        try{
            string postfix_expr = Parser::infix_to_suffix(expr_in);
            // string infix_str = Parser::infix_to_suffix("(A&B)|~C&D");
            printf("postfix expression：%s\n", postfix_expr.c_str());
            printf("truthtable string：%s\n", Calculator(effective_operands, postfix_expr).calc_truth_table().c_str());
        }catch(SyntaxError){
            // do nothing
        }catch(StackError){
            //do nothing
        }
        printf("\n");
    }
    return 0;
}


// namespace ast{ //抽象语法树
// //以下关于在语法树中查询变量的值
//     vector<bool> operands(8,0);  //记录所有变量的值。初始为8个0
//     int effective_operands;  //记录operands中前多少位是有效的
//     //'A'的值为operands[0]。char some_letter的值为operands[some_letter-'A']
//     void inc_operands(vector<bool> operands=operands, int effective_operands=effective_operands){
//         // 将operands视为一个二进制数。operands[0]为最低位。整个数+1
//         bool carry = 1;  //来自上一级的进位
//         bool next_carry;  //本级产生的进位，用于下一级
//         for(int i=0;i<effective_operands;++i){
//             if(operands[i]==1 and carry==1){
//                 next_carry = 1;
//             } else {
//                 next_carry = 0;
//             }
//             operands[i] = operands[i] ^ carry; //异或"^"可实现0与1的加法
//             if(next_carry) {  //本级产生了进位。需要下一级继续处理
//                 carry = next_carry;
//                 continue;
//             }
//             //本级没有产生进位
//             break;
//         }
//     }

// //以下关于表达式的计算
//     inline bool calc(char op, bool operand){
//         switch(op){
//             case '~': return !operand;
//             default:  throw "unexpected operator";
//         }
//     }
//     inline bool calc(char op, bool left_operand, bool right_operand){
//         switch(op){
//             case '&': return left_operand & right_operand;
//             case '|': return left_operand | right_operand;
//             case '^': return left_operand ^ right_operand;
//             default:  throw "unexpected operator";
//         }
//     }

// //以下关于语法树节点
//     class Operand{
//     public:
//         //本节点可以塞操作数
//         char name;  //操作数变量名
//         inline bool get_value(vector<bool> operands=operands){
//             //由变量名查询操作数当前值
//             return operands[name-'A'];
//         }
//     };

//     class Operator{//本节点可以塞一个运算符，可以下挂运算符也可以下挂操作数
//     private:
//         char this_operator;  //本级运算符
//         Operand *left_operand=NULL;  //下级操作数
//         Operand *right_operand=NULL;  //下级操作数

//         //常规情况下一个运算符下面应该为一两个操作数
//         //但也可以为一两个运算符，每个下级运算符下面又有数或者运算符
//         Operator *left_operator=NULL;  //下级运算符
//         Operator *right_operator=NULL;  //下级运算符

//     public:
//         // Operator(char this_op, char l_operand, char r_operand){
//         //     this_operator=this_op;
//         //     left_operand=l_operand;
//         //     right_operand=r_operand;
//         // }

//         bool calc_tree(){
//             if(left_operand){//有下级操作数
//                 if(right_operand){ //双目
//                     return calc(this_operator,left_operand->get_value(),right_operand->get_value());
//                 }else{ //单目
//                     return calc(this_operator,left_operand->get_value());
//                 }
//             }
//             else if(left_operator){ //有下级运算符
//                 if(right_operator){ //本运算符为双目
//                     return calc(this_operator,left_operator->calc_tree(),right_operator->calc_tree());
//                 }else{//本运算符为单目
//                     return calc(this_operator,left_operator->calc_tree());
//                 }
//             }
//         }

//         bool ast_to_value(Operator root){
//             return root.calc_tree();
//         }
//     };

//     // string ast_to_truthtable(int n, const string& expr)
//     // //n是变量个数，expr是逻辑表达式
//     // {
//     //     //_
//     // }
// }
//using namespace ast;
