#include <iostream>
#include <cctype>

using namespace std;

void program();                     //程序
void sub_program();                 //分程序
void declaration_statement_table(); //说明语句表
void declaration_statement();       //说明语句
void variable_declaration();        //变量说明
void variable();                    //变量
void identifier();                  //标识符
void letter();                      //字母
void number();                      //数字
void function_declaration();        //函数说明
void arguments();                   //参数
void function_body();               //函数体
void execute_statement_table();     //执行语句表
void execute_statement();           //执行语句
void read_statement();              //读语句
void write_statement();             //写语句
void assignment_statement();        //赋值语句
void arithmetic_expression();       //算数表达式
void term();                        //项
void factor();                      //因子
void constant();                    //常数
void unsigned_integer();            //无符号整数
void condition_statement();         //条件语句
void conditional_expression();      //条件表达式
void relational_operator();         //关系运算符

void advance();
void error();

string sym;
char character;

// <程序> → <分程序>
void program()
{
  sub_program();
}

// <分程序> → begin <说明语句表>；<执行语句表> end
void sub_program()
{
  if (sym == "begin")
  {
    advance();
    declaration_statement_table();
    if (sym != ";")
    {
      error();
    }
    execute_statement_table();
  }
  if (sym != "end")
  {
    error();
  }
}

// <说明语句表> → <说明语句>│<说明语句表> ；<说明语句>
void declaration_statement_table()
{
}

// <说明语句> → <变量说明>│<函数说明>

// <变量说明> → integer <变量>

// <变量> → <标识符>

// <标识符> → <字母>│<标识符><字母>│ <标识符><数字>

// <字母> → a│b│c│d│e│f│g│h│i│j│k│l│m│n│o │p│q │r│s│t│u│v│w│x│y│z
void letter()
{
  if (!isalpha(character))
  {
    error();
  }
}

// <数字> → 0│1│2│3│4│5│6│7│8│9
void number()
{
  if (!isdigit(character))
  {
    error();
  }
}

// <函数说明> → integer function <标识符>（<参数>）；<函数体>

// <参数> → <变量>

// <函数体> → begin <说明语句表>；<执行语句表> end

// <执行语句表> → <执行语句>│<执行语句表>；<执行语句>

// <执行语句> → <读语句>│<写语句>│<赋值语句>│<条件语句>

// <读语句> → read(<变量>)

// <写语句> → write(<变量>)

// <赋值语句> → <变量>:=<算术表达式>

// <算术表达式> → <算术表达式>-<项>│<项>

// <项> → <项>*<因子>│<因子>

// <因子> → <变量>│<常数>│<函数调用>

// <常数> → <无符号整数>

// <无符号整数> → <数字>│<无符号整数><数字>

// <条件语句> → if<条件表达式>then<执行语句>else <执行语句>

// <条件表达式> → <算术表达式><关系运算符><算术表达式>

// <关系运算符>  → <│<=│>│>=│=│<>
void relational_operator()
{
  if (sym != "<" && sym != "<=" && sym != ">" && sym != ">=" && sym != "=" && sym != "<>")
  {
    error();
  }
}
int main()
{
  return 0;
}