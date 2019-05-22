#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// #include <cctype>
using namespace std;
// vector<string> reserved_word_table = {"begin", "end", "integer", "if", "then", "else", "function", "read", "write"};
vector<int> constant_table;
vector<string> variable_table;
vector<string> reserve_table = {
	"",
	"begin",
	"end",
	"integer",
	"if",
	"then",
	"else",
	"function",
	"read",
	"write",
	"标识符",
	"常数",
	"=",
	"<>",
	"<=",
	"<",
	">=",
	">",
	"-",
	"*",
	":=",
	"(",
	")",
	";",
};
int reserver(string token);
int symbol(string token);
int constant(string token);
string outspace(int num);
int main1()
{
	string infilename = "infile.pas";    //词法分析输入文件
	string outfilename = "outfile.dyd"; //词法分析输出文件
	char character;               // 读取的字符串
	bool isretract = false;       // 标志位, 用来标记是否有字符回退
	string token;
	int num, val;
	ifstream infile;
	ofstream outfile;
	infile.open(infilename);
	outfile.open(outfilename);
	infile >> noskipws;
	while (!infile.eof()) //判断刚读取的字符是否是文件结束符
	{
		// 判断是否回退, 如果回退则不读取新的字符
		if (!isretract)
		{
			infile >> character;
		}
		else
		{
			isretract = false;
		}

		// 防止最后一个字符读两次
		if (infile.fail())
		{
			break;
		}
		switch (character)
		{
		case '\n':
			outfile << "            EOLN 24" << endl;
			break;
		case ' ':
			cout << "忽略一个空格" << endl;
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			while ((isalpha(character) || isdigit(character))&& !infile.fail())
			{
				token += character;
				infile >> character;
			}
			// 回退一个字符
			isretract = true;
			outfile << outspace(16 - token.size());
			num = reserver(token);
			if (num != 0)
			{
				outfile << token << ' ' << num << endl;
			}
			else
			{
				val = symbol(token);
				// outfile << token << ' ' << val << endl;
				outfile << token << ' ' << "10" << endl;
			}
			token = "";
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			while (isdigit(character))
			{
				token += character;
				infile >> character;
			}
			isretract = true;
			val = constant(token);
			outfile << outspace(16 - token.size());
			// outfile << token << ' ' << val << endl;
			outfile << token << ' ' << "11" << endl;
			token = "";
			break;
		case '=':
		case '-':
		case '*':
		case '(':
		case ')':
		case ';':
			token = character;
			num = reserver(token);
			outfile << "               " << character << " " << num << endl;
			token = "";
			break;
		case '<':
			infile >> character;
			if (character == '=')
			{
				outfile << "              <" << character << " "
					<< "14" << endl;
			}
			else if (character == '>')
			{
				outfile << "              <" << character << " "
					<< "13" << endl;
			}
			else
			{
				isretract = true;
			}
			break;
		case '>':
			infile >> character;
			if (character == '=')
			{
				outfile << "              >" << character << " "
					<< "16" << endl;
			}
			else
			{
				isretract = true;
			}
			break;
		case ':':
			infile >> character;
			if (character == '=')
			{
				outfile << "              :" << character << " "
					<< "20" << endl;
			}
			else
			{
				isretract = true;
			}
			break;
		default:
			cout << " error" << character << endl;
			outfile << character; //若不是结束符，将它写入out所指文件
			break;
		}

		// outfile << character;
	}
	outfile << "             EOF 25";
	infile.close();
	outfile.close();
	return 0;
}
// 对存放在 token 中的字符串查保留字表, 若查到则返回它的类别编码, 否则返回0
int reserver(string token)
{
	for (int i = 0; i < reserve_table.size(); i++)
	{
		if (reserve_table[i] == token)
		{
			return i;
		}
	}
	return 0;
}
// 对 token 中的字符串查符号表, 若查到, 返回它在符号表中的位置编号, 否则将该字符串存入符号表,并返回位置编号
int symbol(string token)
{
	for (int i = 0; i < variable_table.size(); i++)
	{
		if (variable_table[i] == token)
		{
			return i;
		}
	}
	variable_table.push_back(token);
	return variable_table.size() - 1;
}
// 常数存入常数表中, 返回位置编号 (string -> int, 简单起见, 只考虑整型常数)
int constant(string token)
{
	constant_table.push_back(stoi(token));
	return constant_table.size() - 1;
}
string outspace(int num)
{
	string res = "";
	for (int i = 0; i < num; i++)
	{
		res += " ";
	}
	return res;
}