#include <iostream>
#include <string>
#include <cmath>
#include <locale>
#include <windows.h>
using namespace std;

int i, symnum, argnum, strnum = 0;
bool start, finish, text, comma;
int argint[9999];
float argfloat[9999];
char ops[9999];
string args[9999];
string argstr[9999];
string str, answer, name, error;



void readstr(){	
	if (comma == false)
		error = "нет запятой";
	comma = false;
	text = true;
	argnum++;
	args[argnum] = "str";
	while (text == true) {
		cout << "readstr - " << text << " symnum: " << symnum << " - " << str[symnum] << endl;
		if (symnum == str.length()) {
			error = "текст не внутри ковычек";
			break;
		}
		else
			symnum++;
		if (str[symnum] == '"')
			text = false;
		else
			argstr[argnum] = argstr[argnum] + str[symnum];
	}
	symnum++;
}
void readfloat(int first) {
	//int round;
	float precision = 10;
	cout << "FloatReading..." << endl;
	if (comma == false)
		error = "нет запятой";
	comma = false;
	//argfloat[0] = 1;
	//argnum++;
	argfloat[argnum] = first;
	args[argnum] = "float";
	//precision = 1;
	argfloat[0] = 1;
	symnum++;
	do {
		cout << "reading float..." << " symnum: " << symnum << " " << "str[symnum]: " << str[symnum] << " argfloat[argnum]: " << argfloat[argnum] << endl;
		//if (argfloat[0] == 1)
		symnum++;
		//argfloat[argnum] *= 10;
		//if (precision == 0)
		switch (str[symnum]) {
		case '1': argfloat[argnum] += 1 / precision; break;
		case '2': argfloat[argnum] += 2 / precision; break;
		case '3': argfloat[argnum] += 3 / precision; break;
		case '4': argfloat[argnum] += 4 / precision; break;
		case '5': argfloat[argnum] += 5 / precision; break;
		case '6': argfloat[argnum] += 6 / precision; break;
		case '7': argfloat[argnum] += 7 / precision; break;
		case '8': argfloat[argnum] += 8 / precision; break;
		case '9': argfloat[argnum] += 9 / precision; break;
		case '0': break;
		default: argfloat[0] = 0; break;
		}

		//cout << "=========== 3 / 0 = " << 3 / 0 << endl;
		if (precision > 1)
			precision *= 10;
	} while (argfloat[0] == 1);
	symnum--;
	precision /= 10;
	//round = argfloat[argnum] * precision;
	//argfloat[argnum] = round / precision;
	//argfloat[argnum] = roundf(argfloat[argnum] * precision) / precision; // округлит до 3 знаков

}
void readint(int first) {
	bool notint = false;
	if (comma == false)
		error = "нет запятой";
	//comma = false;
	argint[0] = 1;
	argnum++;
	argint[argnum] = first;
		args[argnum] = "int";
	do {
		cout << "reading integer..." << symnum << " " << str[symnum] << " " << argint[argnum] << endl;
		if (argint[0] == 1)
			symnum++;

		argint[argnum] *= 10;
		switch (str[symnum]) {
			case '1': argint[argnum] += 1; break;
			case '2': argint[argnum] += 2; break;
			case '3': argint[argnum] += 3; break;
			case '4': argint[argnum] += 4; break;
			case '5': argint[argnum] += 5; break;
			case '6': argint[argnum] += 6; break;
			case '7': argint[argnum] += 7; break;
			case '8': argint[argnum] += 8; break;
			case '9': argint[argnum] += 9; break;
			case '.': notint = true; break;
			case '0': break;
			default: 
				argint[argnum] /= 10;
				argint[0] = 0;
				break;			
		}
	}
	while (argint[0] == 1 && notint == false);
	if (notint == true){
		symnum--;
		readfloat(argint[argnum] / 10);		
		argint[argnum] = 0;
	}
	else
		symnum--;
	comma = false;
		
	//cout << "---------" << argint[argnum] << endl;
}
void getunknown() {
	string word;
}
void perform() {
	cout << "perform()" << endl;
	for (i = 2; i < argnum + 1; i++){
		if (ops[i] != 's'){
			cout << args[i - 1] << ops[i] << args[i] << endl;
			if (args[i] == "str"){
				if (args[i - 1] == "str")
					if (ops[i] == '+') {
						cout << argstr[i - 1] << " + " << argstr[i] << " = " << argstr[i - 1] + argstr[i] << endl;
						argstr[i] = argstr[i - 1] + argstr[i];
						args[i - 1] = "skip";
						argstr[i - 1] = "";
					}
					else
						error = "строки можно только складывать";
				if (args[i - 1] == "int")
					if (ops[i] == '+') {
						cout << argint[i - 1] << " + " << argstr[i] << " = " << to_string(argint[i - 1]) + argstr[i] << endl;
						argstr[i] = to_string(argint[i - 1]) + argstr[i];
						args[i - 1] = "skip";
						argint[i - 1] = NULL;
					}
					else
						error = "числа со строками можно только складывать";
				if (args[i - 1] == "float")
					if (ops[i] == '+') {
						cout << argfloat[i - 1] << " + " << argstr[i] << " = " << to_string(argfloat[i - 1]) + argstr[i] << endl;
						argstr[i] = to_string(argfloat[i - 1]) + argstr[i];
					
						args[i - 1] = "skip";
						argfloat[i - 1] = NULL;
					}
					else
						error = "числа со строками можно только складывать";
			}
			if (args[i] == "int") {
				if (args[i - 1] == "str")
					if (ops[i] == '+') {
						cout << argint[i - 1] << " + " << argstr[i] << " = " << argstr[i - 1] + to_string(argint[i]) << endl;
						argstr[i] = argstr[i - 1] + to_string(argint[i]);
						args[i - 1] = "skip";
						args[i] = "str";
						argstr[i - 1] = "";
					}
					else
						error = "числа со строками можно только складывать";
				

				if (args[i - 1] == "int")
					if (ops[i] == '-') {
						cout << argint[i - 1] << " - " << argint[i] << " = " << argint[i - 1] - argint[i] << endl;
						argint[i] = argint[i - 1] - argint[i];
						args[i - 1] = "skip";
						argint[i - 1] = NULL;
					}
					if (ops[i] == '+') {
						cout << argint[i - 1] << " + " << argint[i] << " = " << argint[i - 1] + argint[i] << endl;
						argint[i] = argint[i - 1] + argint[i];
						args[i - 1] = "skip";
						argint[i - 1] = NULL;
					}
					if (ops[i] == '*') {
						cout << argint[i - 1] << " * " << argint[i] << " = " << argint[i - 1] * argint[i] << endl;
						argint[i] = argint[i - 1] * argint[i];
						args[i - 1] = "skip";
						argint[i - 1] = NULL;
					}
					if (ops[i] == '/') {
						cout << argint[i - 1] << " / " << argint[i] << " = " << argint[i - 1] / argint[i] << endl;
						argint[i] = argint[i - 1] / argint[i];
						args[i - 1] = "skip";
						argint[i - 1] = NULL;
					}
			}
		}
	}
}

int main() {
	setlocale(LC_ALL, "");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	//strnum = 0;
	while (true) {
		strnum++;
		getline(cin, str);
		//for (symnum = 0; symnum < str.length(); symnum++)
			//cout << endl << symnum << " - " << str[symnum] << endl;
		//str = str + ';';
		start = false;
		finish = false;
		text = false;
		comma = true;
		error = "нет ошибок";
		argint[0] = 0;
		argstr[0] = "";
		argnum = 0;
		answer = "";
		
		/////////////////////////////////////////////////////////////////////////проверка на ';'
		//if (str.rfind("изрекаю", 0) == 0)
			for (symnum = 0; symnum < str.length(); symnum++) {
				//cout << "---" << argnum << " " << str[symnum] << " " << argint[argnum - 1] << " " << argint[argnum] << endl;
				//cout << "symnum: " << symnum << ", " << str[symnum] << ", start: " << start << ", finish: " << finish << ", error " << error << endl;
				
				switch (str[symnum]) {
					case ' ': break;
					case '0':
						argnum++;
						//comma = false;
						switch (str[symnum + 1]){
							case '.': 
								readfloat(0);
								break;
							case ' ':
								args[argnum] = "zero";
								break;
							case ',':
								args[argnum] = "zero";
								comma = true; 
								break;
							case ')':
								args[argnum] = "zero";
								finish = true; 
								break;
							default: 
								error = "неожиданный символ '" + str[symnum + 1] + '\'';
								cout << "default: " << str[symnum + 1] << endl;
								break;
						}
						break;
					case ',':
						if (argnum > 0 && comma == false)
							comma = true;
						else
							error = "лишняя запятая";
						break;	
						
					case '-': comma = true; ops[argnum + 1] = '-'; ops[argnum + 2] = 's'; break;
					case '+': comma = true; ops[argnum + 1] = '+'; ops[argnum + 2] = 's'; break;
					case '*': comma = true; ops[argnum + 1] = '*'; ops[argnum + 2] = 's'; break;
					case '/': comma = true; ops[argnum + 1] = '/'; ops[argnum + 2] = 's'; break;
					/*
					(56, 4556)
					(435 + 45)
					(35, 4 + 9)
					(54,56 * 4335 - 2)
					("erg" + 23)!!!
					("ger" + "free")
					(0 - 4, 56 + 4)
					("erf", 43 / 5 + 21, "e34", 0, 545)
				    */

					case '1': readint(1); break;
					case '2': readint(2); break;
					case '3': readint(3); break;
					case '4': readint(4); break;
					case '5': readint(5); break;
					case '6': readint(6); break;
					case '7': readint(7); break;
					case '8': readint(8); break;
					case '9': readint(9); break;					
					case '(': start = true; break;							
					case ')': finish = true; break;
					case '"': readstr(); symnum--; break;					
					default: error = "неожиданный символ '" + str[symnum] + '\''; cout << "default : '" << str[symnum] << "' " << str.length() << " " << symnum << endl; break;
					}				
				
			}
			
			perform();
			
			
			
			if (start == false or finish == false) 
				error = symnum;
			if (comma == true)
				error = symnum;
			answer = "";
			if (error == "нет ошибок") {
				//=====================ANSWER 
				cout << endl << "args: " << args[0] << endl;
				args[0] = to_string(argnum);
				for (argnum = 0; argnum < stoi(args[0]) + 1; argnum++) {
					if (args[argnum] == "int")
						answer += to_string(argint[argnum]);
					if (args[argnum] == "float")
						answer += to_string(argfloat[argnum]);
					if (args[argnum] == "str")
						answer += argstr[argnum];
					if (args[argnum] == "zero")
						answer += '0';
					//if skip
					cout << "arg " << argnum << ": " << args[argnum] << endl;
					//default: answer += "NULL"; break;
				}
			}
			else
				//cout << "-----------------------------" << "В " << strnum << " строке обнаружена ошибка!" << endl;
				//cout << "====================ошибка ~ на " << error << " символе '" << str[error] << "'!" << endl;
				cout << "ошибка: " << error << endl;
			cout << "=============================" << answer << endl;
			///*
			cout << "comma: " << comma << endl;// << "аргументы:";
			cout << "start: " << start << endl;
			cout << "finish: " << finish << endl;
			cout << "argnum: " << argnum << endl;
			cout << "argint[0]" << argint[0] << endl;
			//cout << "error: " << str[error] << endl;
			//cout << "stoi(argnum[argint]): " << stoi(argnum[argint]) << endl;
			//cout << "args[0]: " << args[0] << endl;
			//*/



			//////////////////////////////////////////////////////////////// ЗАЧИСТКА
			args[0] = to_string(argnum);
			for (argnum = 0; argnum < stoi(args[0]); argnum++) {
				argstr[argnum] = "";
				argint[argnum] = NULL;
				argfloat[argnum] = NULL;
			}
			argnum = 0;
			args[0] = "0";
	}
}