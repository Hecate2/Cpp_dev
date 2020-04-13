//<���ʽ> ��=  <��> & <��>
//          |  <��> | <��>
//          |  <��>
//          |  !<��>
//<��>    ��=  <����> & <����>
//          |  <����> | <����>
//          |  <����>
//          |  !<����>
//<����>  ��=  <����>
//          | �� <���ʽ> ��
//<����>  ��=  A | B | C | ...

#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
using namespace std;
//#include <math.h>

//ȫ���������쳣
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

//ȫ�����������������
namespace operators{
    struct OperatorInfo{
        char operator_itself;  //���������
        int priority;   //���ȼ�
        int ary;    //�����Ŀ��
        OperatorInfo operator = (OperatorInfo b) {
            operator_itself = b.operator_itself;
            priority = b.priority;
            ary = b.ary;
            return *this;
        }
    };
    inline OperatorInfo get_operator_info(const char& operator_itself){
        //���һ���ַ��Ƿ�Ϊ������������
        //�����������������ȼ�����������ʾ���������Ŀ��
        switch(operator_itself){
            case '(': return OperatorInfo{operator_itself,-1073741824,0};
            case ')': return OperatorInfo{operator_itself,-1073741824,0};
            case '~': return OperatorInfo{operator_itself,4000,1};  // NOT(prefix)
            case '!': return OperatorInfo{operator_itself,4000,1};  // ���������֧�֣�!��Ϊ~
            case '&': return OperatorInfo{operator_itself,3000,2};  // AND
            case '*': return OperatorInfo{operator_itself,3000,2};  // ���������֧�֣�*��Ϊ&
            case '|': return OperatorInfo{operator_itself,2000,2};  // OR
            case '+': return OperatorInfo{operator_itself,2000,2};  // ���������֧�֣�+��Ϊ|
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


//���¹���������ʽ�ַ����Ľ���
//����namespace operators��namespace exceptions
class Parser{
public:
    static inline bool is_var(const char& letter){//�Ǳ�����
        if( (65<=letter && letter<=90) || (97<=letter && letter<=122) ) {
            // if(65<=letter && letter<=90){//�Ǵ�д��ĸ
            return 1;
        }else{
            return 0;
        }
    }

    static string infix_to_suffix(string expr)
    //��׺���ʽת��׺���ʽ    //��׺���ʽ��Ϊ�м����
    //����������ֱ��������ַ���ĩβд���������
    //������������ջ��ֱ��ջΪ�գ�����ջ������������ȼ����ڱ������
    //����ջ�������д������ַ���
    //Ȼ��ѱ��������ջ
    {
        //TODO: string[i]������Ϊchar�����ĺ���
        stack<OperatorInfo> operator_stack;  //�����ջ
        stringstream suffix;  //���صĺ�׺���ʽ

        int s=expr.size();
        OperatorInfo operator_current, operator_in_stack;
        //std::string��׼��δ�涨��Ҫ'\0'��Ϊ�ַ�����β
        for(int i=0;i<s;++i)
        {
            try{ // ��Ҫ��try�Ƶ�for����ȥ��try�Ჶ���﷨�����λ�á�
                if(expr[i]==' '){continue;} //���ӿո�
                if(is_var(expr[i])){//�Ǳ�����
                    suffix << expr[i];
                    continue;
                }
                operator_current = get_operator_info(expr[i]); //�������
                if(expr[i]=='('){
                    // printf("%s\n", "׼����ջ");
                    // printf("%c %d %d\n", operator_current.operator_itself, operator_current.priority, operator_current.ary);
                    operator_stack.push(operator_current);
                    continue;
                }
                if(expr[i]==')'){
                    bool got_left = false;  // �������ջ���ҵ���������
                    while( not operator_stack.empty()) {
                        operator_in_stack = operator_stack.top();
                        if(operator_in_stack.operator_itself == '('){
                            operator_stack.pop();
                            got_left = true;
                            break;
                        }else{
                            operator_stack.pop();
                            if(operator_in_stack.ary > 0){
                                //��׺���ʽ�����Ҫ��ʵ�����㹦�ܵ������������Ҫ����
                                suffix << operator_in_stack.operator_itself;
                                // printf("%s\n", "д���׺���ʽ");
                                // printf("%s\n", suffix.str().c_str());
                            }
                        }
                    }
                    if(got_left == false){
                        printf("���������Ѻ��Դ��󣬼����������ʽ������֤����ȷ\n");
                    }
                    continue;
                }
                // �����������������
                while( not operator_stack.empty() ) {
                    //׼����ջ������ջ�������д����׺���ʽ
                    operator_in_stack = operator_stack.top();
                    if(operator_in_stack.priority <= operator_current.priority){
                        //ջ����������ȼ����ڵ��ڵ�ǰ�������ֹͣ��ջ
                        break;
                    }else{
                        // printf("%s\n", "׼����ջ");
                        // printf("%c %d %d\n", operator_in_stack.operator_itself, operator_in_stack.priority, operator_in_stack.ary);
                        operator_stack.pop();
                        if(operator_in_stack.ary > 0){
                            //��׺���ʽ�����Ҫ��ʵ�����㹦�ܵ������������Ҫ����
                            suffix << operator_in_stack.operator_itself;
                            // printf("%s\n", "д���׺���ʽ");
                            // printf("%s\n", suffix.str().c_str());
                        }
                    }
                }
                // printf("%s\n", "׼����ջ");
                // printf("%c %d %d\n", operator_current.operator_itself, operator_current.priority, operator_current.ary);
                operator_stack.push(operator_current);
                continue;
            }
            catch(SyntaxError){
                syntax_error(expr, i, "unexpected char");
            }
        }
        //ȫ����ջ
        // printf("ȫ����ջ\n");
        while( not operator_stack.empty() ) {
            //׼����ջ������ջ�������д����׺���ʽ
            operator_in_stack = operator_stack.top();
            // printf("%c\n", operator_in_stack.operator_itself);
            operator_stack.pop();
            if(operator_in_stack.ary > 0){
                //��׺���ʽ�����Ҫ��ʵ�����㹦�ܵ������
                suffix << operator_in_stack.operator_itself;
            }
            else if(operator_in_stack.operator_itself == '(' or operator_in_stack.operator_itself == ')'){
                //ȫ����ջʱ��Ӧ��������
                printf("���������Ѻ��Դ��󣬼����������ʽ������֤����ȷ\n");
            }
        }
        return suffix.str();
    }
};
//using namespace parser;

class Calculator{
public:
    int effective_operands;  //��¼operands��ǰ����λ����Ч��
    string postfix_expr;  //�ⲿ����ĺ�׺���ʽ
    vector<bool> operands;
    //'A'��ֵΪoperands[0]��char some_letter��ֵΪoperands[some_letter-'A']

    //���캯��
    Calculator(int input_effective_operands, const string& input_postfix_expr) {
        postfix_expr = input_postfix_expr;
        effective_operands = input_effective_operands;
    }

    inline bool get_value(const char& name){
        //�ɱ�������ѯ��������ǰֵ
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
        // ��operands��Ϊһ������������operands[0]Ϊ���λ��������+1
        bool carry = 1;  //������һ���Ľ�λ
        bool next_carry;  //���������Ľ�λ��������һ��
        for(int i=0;i<effective_operands;++i){
            if(operands[i]==1 and carry==1){
                next_carry = 1;
            } else {
                next_carry = 0;
            }
            operands[i] = operands[i] ^ carry; //���"^"��ʵ��0��1�ļӷ�
            if(next_carry) {  //���������˽�λ����Ҫ��һ����������
                carry = next_carry;
                continue;
            }
            //����û�в�����λ
            break;
        }
    }

    inline void dec_operands(){
        // ��operands��Ϊһ������������operands[0]Ϊ���λ��������-1
        bool carry = 1;  //������һ������λ
        bool next_carry;  //������������λ��������һ��
        for(int i=0;i<effective_operands;++i){
            if(operands[i]==0 and carry==1){
                next_carry = 1;
            } else {
                next_carry = 0;
            }
            operands[i] = operands[i] ^ carry; //���"^"��ʵ��0��1�ļ���
            if(next_carry) {  //������������λ����Ҫ��һ����������
                carry = next_carry;
                continue;
            }
            //����û�в�����λ
            break;
        }
    }

    inline bool calc_one_operator(const vector<bool>& operands, const char& operator_itself){
        //���ڵ��������������
        switch(operator_itself){
            case '~': return (!operands[0]);  // NOT(prefix)
            case '!': return (!operands[0]);  // ���������֧�֣�!��Ϊ~
            case '&': return (operands[0]&&operands[1]);  // AND
            case '*': return (operands[0]&&operands[1]);  // ���������֧�֣�*��Ϊ&
            case '|': return (operands[0]||operands[1]);  // OR
            case '+': return (operands[0]||operands[1]);  // ���������֧�֣�+��Ϊ|
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
        int ary;  //��ǰ�����Ŀ��

        for(int i=0; i<s; ++i){
            if (Parser::is_var(postfix_expr[i])){
                operand_stack.push(get_value(postfix_expr[i]));
                continue;
            }
            ary = operators::get_operator_info(postfix_expr[i]).ary;  //��������������ֱ��throw exception
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
    printf("���� �������� �� �߼����ʽ�ַ��� �������ֵ���ַ���\n\n");
    printf("֧�� ������д��ĸ ��Ϊ������\n");
    printf("֧���������\n~ǰ׺NOT !ǰ׺NOT\n&��׺AND *��׺AND\n|��׺OR  +��׺OR\n^��׺XOR\n������()\n\n");
    printf("ʾ����2 A&B -> AB& -> 1000\n");
    printf("ʾ����4 (A&B)|~C&D -> AB&C~D&| -> 1000111110001000\n");
    int effective_operands;
    string expr_in;
    while(1){
        printf(">>> ");
        cin >> effective_operands >> expr_in;
        while(cin.fail())
        {
            cin.clear();
            printf("�������������һ����������һ���߼����ʽ�ַ�����\n��������ʾ�߼����ʽ�ַ����в�ͬ�����ĸ���\n>>> ");
            cin.ignore(1000000, '\n');
            cin >> effective_operands >> expr_in;
        }
        try{
            string postfix_expr = Parser::infix_to_suffix(expr_in);
            // string infix_str = Parser::infix_to_suffix("(A&B)|~C&D");
            printf("postfix expression��%s\n", postfix_expr.c_str());
            printf("truthtable string��%s\n", Calculator(effective_operands, postfix_expr).calc_truth_table().c_str());
        }catch(SyntaxError){
            // do nothing
        }catch(StackError){
            //do nothing
        }
        printf("\n");
    }
    return 0;
}


// namespace ast{ //�����﷨��
// //���¹������﷨���в�ѯ������ֵ
//     vector<bool> operands(8,0);  //��¼���б�����ֵ����ʼΪ8��0
//     int effective_operands;  //��¼operands��ǰ����λ����Ч��
//     //'A'��ֵΪoperands[0]��char some_letter��ֵΪoperands[some_letter-'A']
//     void inc_operands(vector<bool> operands=operands, int effective_operands=effective_operands){
//         // ��operands��Ϊһ������������operands[0]Ϊ���λ��������+1
//         bool carry = 1;  //������һ���Ľ�λ
//         bool next_carry;  //���������Ľ�λ��������һ��
//         for(int i=0;i<effective_operands;++i){
//             if(operands[i]==1 and carry==1){
//                 next_carry = 1;
//             } else {
//                 next_carry = 0;
//             }
//             operands[i] = operands[i] ^ carry; //���"^"��ʵ��0��1�ļӷ�
//             if(next_carry) {  //���������˽�λ����Ҫ��һ����������
//                 carry = next_carry;
//                 continue;
//             }
//             //����û�в�����λ
//             break;
//         }
//     }

// //���¹��ڱ��ʽ�ļ���
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

// //���¹����﷨���ڵ�
//     class Operand{
//     public:
//         //���ڵ������������
//         char name;  //������������
//         inline bool get_value(vector<bool> operands=operands){
//             //�ɱ�������ѯ��������ǰֵ
//             return operands[name-'A'];
//         }
//     };

//     class Operator{//���ڵ������һ��������������¹������Ҳ�����¹Ҳ�����
//     private:
//         char this_operator;  //���������
//         Operand *left_operand=NULL;  //�¼�������
//         Operand *right_operand=NULL;  //�¼�������

//         //���������һ�����������Ӧ��Ϊһ����������
//         //��Ҳ����Ϊһ�����������ÿ���¼�������������������������
//         Operator *left_operator=NULL;  //�¼������
//         Operator *right_operator=NULL;  //�¼������

//     public:
//         // Operator(char this_op, char l_operand, char r_operand){
//         //     this_operator=this_op;
//         //     left_operand=l_operand;
//         //     right_operand=r_operand;
//         // }

//         bool calc_tree(){
//             if(left_operand){//���¼�������
//                 if(right_operand){ //˫Ŀ
//                     return calc(this_operator,left_operand->get_value(),right_operand->get_value());
//                 }else{ //��Ŀ
//                     return calc(this_operator,left_operand->get_value());
//                 }
//             }
//             else if(left_operator){ //���¼������
//                 if(right_operator){ //�������Ϊ˫Ŀ
//                     return calc(this_operator,left_operator->calc_tree(),right_operator->calc_tree());
//                 }else{//�������Ϊ��Ŀ
//                     return calc(this_operator,left_operator->calc_tree());
//                 }
//             }
//         }

//         bool ast_to_value(Operator root){
//             return root.calc_tree();
//         }
//     };

//     // string ast_to_truthtable(int n, const string& expr)
//     // //n�Ǳ���������expr���߼����ʽ
//     // {
//     //     //_
//     // }
// }
//using namespace ast;
