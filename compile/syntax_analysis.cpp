#include <iostream>
#include <fstream>

using namespace std;

string infilename = "outfile.dyd";   //语法分析输入文件
string outfilename = "outfile.dyd2"; //语法分析输出文件
ifstream infile;
ofstream outfile;
string sym;

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
// <标识符> ->
void identifier();
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

// 判断是否是关系运算符
bool is_relational_operator(string str);
// 判断是否是标识符
bool is_identifier(string str);
// 判断是否是无符号整数
bool is_unsigned_integer(string str);

void advance(ifstream *infile);
void error(string error_message);

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
    advance(&infile);
    declaration_statement_table();
    if (sym == ";")
    {
      advance(&infile);
      execute_statement_table();
      if (sym == "end")
      {
        advance(&infile);
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
    advance(&infile);
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
    advance(&infile);
    if (sym == "function")
    {
      advance(&infile);
      identifier();
      if (sym == "(")
      {
        advance(&infile);
        arguments();
        if (sym == ")")
        {
          advance(&infile);
          if (sym == ";")
          {
            advance(&infile);
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
// <标识符> ->
void identifier()
{
  if (is_identifier(sym))
  {
    advance(&infile);
  }
  else
  {
    error("identifier");
  }
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
    advance(&infile);
    declaration_statement_table();
    if (sym == ";")
    {
      advance(&infile);
      execute_statement_table();
      if (sym == "end")
      {
        advance(&infile);
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
    advance(&infile);
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
    advance(&infile);
    if (sym == "(")
    {
      advance(&infile);
      variable();
      if (sym == ")")
      {
        advance(&infile);
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
    advance(&infile);
    if (sym == "(")
    {
      advance(&infile);
      variable();
      if (sym == ")")
      {
        advance(&infile);
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
    advance(&infile);
    conditional_expression();
    if (sym == "then")
    {
      advance(&infile);
      execute_statement();
      if (sym == "else")
      {
        advance(&infile);
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
  else if (is_identifier(sym))
  {
    arithmetic_expression();
    if (sym == ":=")
    {
      advance(&infile);
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
    advance(&infile);
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
    advance(&infile);
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
  if (is_identifier(sym))
  {
    identifier();
    if (sym == "(")
    {
      advance(&infile);
      arithmetic_expression();
      if (sym == ")")
      {
        advance(&infile);
      }
      else
      {
        error("factor )");
      }
    }
  }
  else if (is_identifier(sym))
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
  if (is_unsigned_integer(sym))
  {
    advance(&infile);
  }
  else
  {
    error("constant");
  }
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
  arithmetic_expression();
  if (is_relational_operator(sym))
  {
    arithmetic_expression();
  }
}
// <关系运算符>  → <│<=│>│>=│=│<>
bool is_relational_operator(string str)
{
  if (sym != "<" && sym != "<=" && sym != ">" && sym != ">=" && sym != "=" && sym != "<>")
  {
    error("relational_operator");
  }
  advance(&infile);
}
bool is_unsigned_integer(string str)
{
  return true;
}
bool is_identifier(string str)
{
}

void advance(ifstream *infile)
{
  getline(*infile, sym);
}
int main()
{
  infile.open(infilename);
  // outfile.open(outfilename);
  infile >> noskipws;
  while (sym != "             EOF 25")
  {
    advance(&infile);
    program();
    // cout << sym << endl;
  }
  // program();
  return 0;
}