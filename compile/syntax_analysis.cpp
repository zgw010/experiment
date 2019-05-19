#include <iostream>
#include <cctype>

using namespace std;

// <程序> → <分程序>
void program();
// <分程序> → begin <说明语句表>；<执行语句表> end
void sub_program();
// <说明语句表> -> <说明语句><说明语句表_s>
void declaration_statement_table();
// <说明语句表_s> -> ;<说明语句><说明语句表_s>|<空>
void declaration_statement_table_s();
// <说明语句> → <变量说明>│<函数说明>
void declaration_statement();
// <变量说明> → integer <变量>
void variable_declaration();
// <变量> → <标识符>
void variable();
// <标识符> -> <字母><标识符_s>
void identifier();
// <标识符_s> -> <字母><标识符_s>|<数字><标识符_s>|<空>
void identifier_s();
// │r│s│t│u│v│w│x│y│z
void letter();
// <数字> → 0│1│2│3│4│5│6│7│8│9
void number();
// <函数说明> → integer function <标识符>（<参数>）；<函数体>
void function_declaration();
// <参数> → <变量>
void arguments();
// <函数体> → begin <说明语句表>；<执行语句表> end
void function_body();
// <执行语句表> → <执行语句><执行语句表_s>
void execute_statement_table();
// <执行语句表_s> -> ;<执行语句><执行语句表_s>|<空>
void execute_statement_table_s();
// <执行语句> → <读语句>│<写语句>│<赋值语句>│<条件语句>
void execute_statement();
// <读语句> → read(<变量>)
void read_statement();
// <写语句> → write(<变量>)
void write_statement();
// <赋值语句> → <变量>:=<算术表达式>
void assignment_statement();
// <算术表达式> → <项><算术表达式_s>
void arithmetic_expression();
// <算术表达式_s> -> -<项><算术表达式_s>|<空>
void arithmetic_expression_s();
// <项> → <因子><项_s>
void term();
// <项_s> -> *<因子><项_s>|<空>
void term_s();
// <因子> → <变量>│<常数>│<函数调用>
void factor();
// <常数> → <无符号整数>
void constant();
// <无符号整数> → <数字><无符号整数_s>
void unsigned_integer();
// <无符号整数_s> -> <数字><无符号整数_s>|<空>
void unsigned_integer_s();
// <函数调用> -> <标识符>(<算术表达式>)
void function_call();
// <条件语句> → if<条件表达式>then<执行语句>else <执行语句>
void condition_statement();
// <条件表达式> → <算术表达式><关系运算符><算术表达式>
void conditional_expression();
// <关系运算符>  → <│<=│>│>=│=│<>
void relational_operator();
void advance();
void error(string error_message);

string sym;
char character;

void error(string error_message)
{
  cout << error_message << endl;
}

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
    if (sym == ";")
    {
      advance();
      execute_statement_table();
      if (sym == "end")
      {
        advance();
      }
      else
      {
        error("sub_program end");
      }
    }
    else
    {
      error("sub_program ;");
    }
  }
}
// <说明语句表> -> <说明语句><说明语句表_s>
void declaration_statement_table()
{
  declaration_statement();
  declaration_statement_table_s();
}
// <说明语句表_s> -> ;<说明语句><说明语句表_s>|<空>
void declaration_statement_table_s()
{
  if (sym == ";")
  {
    advance();
    declaration_statement();
    declaration_statement_table_s();
  }
}
// <说明语句> → <变量说明>│<函数说明>
// <变量说明> → integer <变量>
// <函数说明> → integer function <标识符>（<参数>）；<函数体>
void declaration_statement()
{
  if (sym == "integer")
  {
    advance();
    if (sym == "function")
    {
      advance();
      identifier();
      if (sym == "(")
      {
        advance();
        arguments();
        if (sym == ")")
        {
          advance();
          if (sym == ";")
          {
            advance();
            function_body();
          }
        }
      }
    }
    variable();
  }
  else
  {
    error("declaration_statement integer");
  }
}
// <变量说明> → integer <变量>
void variable_declaration()
{
  // 已在 declaration_statement 实现
}
// <变量> → <标识符>
void variable()
{
  identifier();
}
// <标识符> -> <字母><标识符_s>
void identifier()
{
  letter();
  identifier_s();
}
// <标识符_s> -> <字母><标识符_s>|<数字><标识符_s>|<空>
// TODO
void identifier_s()
{
  if (isalpha(character))
  {
    letter();
    identifier_s();
  }
  else if (isdigit(character))
  {
    number();
    identifier_s();
  }
}
// <字母> → a│b│c│d│e│f│g│h│i│j│k│l│m│n│o │p│q │r│s│t│u│v│w│x│y│z
void letter()
{
  if (!isalpha(character))
  {
    error("letter");
  }
  advance();
}
// <数字> → 0│1│2│3│4│5│6│7│8│9
void number()
{
  if (!isdigit(character))
  {
    error("number");
  }
  advance();
}
// <函数说明> → integer function <标识符>（<参数>）；<函数体>
void function_declaration()
{
  // 已在 declaration_statement 实现
}
// <参数> → <变量>
void arguments()
{
  variable();
}
// <函数体> → begin <说明语句表>；<执行语句表> end
void function_body()
{
  if (sym == "begin")
  {
    advance();
    declaration_statement_table();
    if (sym == ";")
    {
      advance();
      execute_statement_table();
      if (sym == "end")
      {
        advance();
      }
      else
      {
        error("function_body end");
      }
    }
    else
    {
      error("function_body :");
    }
  }
}
// <执行语句表> → <执行语句><执行语句表_s>
void execute_statement_table()
{
  execute_statement();
  execute_statement_table_s();
}
// <执行语句表_s> -> ;<执行语句><执行语句表_s>|<空>
void execute_statement_table_s()
{
  if (sym == ";")
  {
    advance();
    execute_statement();
    execute_statement_table_s();
  }
}
// <执行语句> → <读语句>│<写语句>│<赋值语句>│<条件语句>
// <常数> → <无符号整数>
// <读语句> → read(<变量>)
// <写语句> → write(<变量>)
// <赋值语句> → <变量>:=<算术表达式>
// <条件语句> → if<条件表达式>then<执行语句>else <执行语句>
void execute_statement()
{
  // <读语句> → read(<变量>)
  if (sym == "read")
  {
    advance();
    if (sym == "(")
    {
      advance();
      variable();
      if (sym == ")")
      {
        advance();
      }
      else
      {
        error("execute_statement )");
      }
    }
    else
    {
      error("execute_statement (");
    }
  }
  // <写语句> → write(<变量>)
  else if (sym == "write")
  {
    advance();
    if (sym == "(")
    {
      advance();
      variable();
      if (sym == ")")
      {
        advance();
      }
      else
      {
        error("execute_statement )");
      }
    }
    else
    {
      error("execute_statement (");
    }
  }
  // <条件语句> → if<条件表达式>then<执行语句>else <执行语句>
  else if (sym == "if")
  {
    advance();
    conditional_expression();
    if (sym == "then")
    {
      advance();
      execute_statement();
      if (sym == "else")
      {
        advance();
        execute_statement();
      }
      else
      {
        error("execute_statement else");
      }
    }
    else
    {
      error("execute_statement if");
    }
  }
  // <赋值语句> → <变量>:=<算术表达式>
  // TODO 此处需要判断事都是字母
  else if (isalpha(character))
  {
    assignment_statement();
    if (sym == ":=")
    {
      advance();
      arithmetic_expression();
    }
  }
  else
  {
    error("execute_statement error");
  }
}
// <读语句> → read(<变量>)
// void read_statement()
// {
// }
// <写语句> → write(<变量>)
// void write_statement()
// {
// }
// <赋值语句> → <变量>:=<算术表达式>
// void assignment_statement()
// {
// }
// <算术表达式> → <项><算术表达式_s>
// <常数> → <无符号整数>
void arithmetic_expression()
{
  term();
  arithmetic_expression_s();
}
// <算术表达式_s> -> -<项><算术表达式_s>|<空>
void arithmetic_expression_s()
{
  if (sym == "-")
  {
    advance();
    term();
    arithmetic_expression_s();
  }
}
// <项> → <因子><项_s>
void term()
{
  factor();
  term_s();
}
// <项_s> -> *<因子><项_s>|<空>
void term_s()
{
  if (sym == "*")
  {
    advance();
    factor();
    term_s();
  }
}
// <因子> → <变量>│<常数>│<函数调用>
// <变量> → <标识符>
// <常数> → <无符号整数>
// <函数调用> -> <标识符>(<算术表达式>)
void factor()
{
  if (isalpha(character))
  {
    identifier();
    if (sym == "(")
    {
      advance();
      arithmetic_expression();
      if (sym == ")")
      {
        advance();
      }
      else
      {
        error("factor )");
      }
    }
  }
  else if (isdigit(character))
  {
    constant();
  }
  else
  {
    error("factor error");
  }
}
// <常数> → <无符号整数>
void constant()
{
  unsigned_integer();
}
// <无符号整数> → <数字><无符号整数_s>
void unsigned_integer()
{
  number();
  unsigned_integer_s();
}
// <无符号整数_s> -> <数字><无符号整数_s>|<空>
void unsigned_integer_s()
{
  number();
  unsigned_integer_s();
}
// <函数调用> -> <标识符>(<算术表达式>)
void function_call()
{
  // 在 factor 中被实现
}
// <条件语句> → if<条件表达式>then<执行语句>else <执行语句>
// void condition_statement()
// {
// }
// <条件表达式> → <算术表达式><关系运算符><算术表达式>
void conditional_expression()
{
  assignment_statement();
  conditional_expression();
  assignment_statement();
}
// <关系运算符>  → <│<=│>│>=│=│<>
void relational_operator()
{
  if (sym != "<" && sym != "<=" && sym != ">" && sym != ">=" && sym != "=" && sym != "<>")
  {
    error("relational_operator");
  }
  advance();
}

int main()
{
  return 0;
}