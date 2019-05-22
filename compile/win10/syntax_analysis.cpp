#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

string infilename = "outfile.dyd";   //语法分析输入文件
string outfilename = "outfile.dys"; //语法分析输出文件
string errfilename = "outfile.err"; //语法分析错误信息文件
ifstream infile;
ofstream outfile;
ofstream errfile;
set<string> variable_table;
string sym;
string sym_type;
string variable_type = "define";
int code_line = 1;
int need_read_new_line = 0;
vector <string> word_vector;
unsigned int word_vector_size = 2;

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
bool is_unsigned_integer();

void advance(ifstream* infile);
void error(string error_message, int line);

void error(string error_message, int line)
{
	cout << "***LINE:" << line << "  " << error_message << endl;
	errfile << "***LINE:" << line << "  " << error_message << endl;
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
				error("sub_program end", code_line);
			}
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
		
	}
	else
	{
		error("缺少符号出错: 缺失 ;", code_line - 1);
	}
	declaration_statement();
	if (need_read_new_line == 0) {
		declaration_statement_table_s();
	}
	else
	{
		advance(&infile);
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
		variable_type = "define";
		if (sym == "function")
		{
			variable_type = "define";
			advance(&infile);
			identifier();
			if (sym == "(")
			{
				advance(&infile);
				variable_type = "arguments";
				arguments();
				if (sym == ")")
				{
					advance(&infile);
				}
				else
				{
					error("符号匹配出错: 缺失 )", code_line);
				}
				if (sym == ";")
				{
					advance(&infile);
					function_body();
				}
			}
		}
		else {
			variable();
		}
		variable_type = "use";
	}
	else
	{
		need_read_new_line = 2;
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
		error("identifier", code_line);
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
				error("function_body end", code_line);
			}
		}
		else
		{
			error("function_body :", code_line);
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
		if (need_read_new_line == 0) {
			execute_statement_table_s();
		}
		else
		{
			advance(&infile);
		}
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
				error("execute_statement )", code_line);
			}
		}
		else
		{
			error("execute_statement (", code_line);
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
				error("execute_statement )", code_line);
			}
		}
		else
		{
			error("execute_statement (", code_line);
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
				error("execute_statement else", code_line);
			}
		}
		else
		{
			error("execute_statement if", code_line);
		}
	}
	// <赋值语句> → <变量>:=<算术表达式>
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
		error("execute_statement error", code_line);
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
		//identifier();
		advance(&infile);
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
				error("factor )", code_line);
			}
		}
	}
	else if (is_unsigned_integer())
	{
		constant();
	}
	else
	{
		need_read_new_line = 2;
	}
}
// <常数> → <无符号整数>
void constant()
{
	if (is_unsigned_integer())
	{
		advance(&infile);
	}
	else
	{
		error("constant", code_line);
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
		error("relational_operator", code_line);
		return false;
	}
	advance(&infile);
	return true;
}
bool is_unsigned_integer()
{
	// cout << "is_unsigned_integer 11" << endl;
	if (sym_type == "11") {
		return true;
	}
	return false;
}
bool is_identifier(string str)
{
	if (sym_type == "10" && need_read_new_line ==0) {
		if (variable_type == "define")
		{
			if (variable_table.count(sym) == 0) {
				variable_table.insert(sym);
			}
			else
			{
				error("变量重复定义", code_line);
			}
		}
		else if (variable_type == "use"){
			if (variable_table.count(sym) == 0) {
				error("错误地使用了未定义的变量 " + sym, code_line);
			}
		}
		else if (variable_type == "arguments")
		{

		}
		return true;
	}
	return false;
}

void advance(ifstream* infile)
{
	int space_index;
	if (need_read_new_line == 0) {
		getline(*infile, sym);
		outfile << sym << endl;
		sym.erase(0, sym.find_first_not_of(" "));
		//cout << sym << endl;
		space_index = sym.find(" ");
		sym_type = sym.substr(space_index + 1);
		//cout << sym_type << endl;
		sym = sym.substr(0, space_index);
		if (sym == "EOLN") {
			++code_line;
			cout << "----this is " << code_line << " line----" << endl;
			advance(infile);
			return;
		}
		word_vector.push_back(sym);
		if (word_vector.size() > word_vector_size) {
			word_vector.erase(word_vector.begin());
		}
	}
	else {
		sym = word_vector[0];
		word_vector.erase(word_vector.begin());
		--need_read_new_line;
	}
}

int main()
{
	infile.open(infilename);
	outfile.open(outfilename);
	errfile.open(errfilename);
	infile >> noskipws;
	cout << "----this is 1 line----" << endl;
	advance(&infile);
	program();
	infile.close();
	outfile.close();
	errfile.close();
	return 0;
}