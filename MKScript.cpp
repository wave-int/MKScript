#include <iostream>
#include <string>
#include <cmath>
#include <locale>
#include <windows.h>
#include <fstream>
#include <chrono>
#include <vector>
#include  <stdio.h> 
#include  <stdlib.h> 

using namespace std::chrono;
using namespace std;

bool autoround = true, autospace = true, test = false; 

time_point<high_resolution_clock> timer = high_resolution_clock::now();

char comparison; struct condition {string left; char comparison; string right;};

int repeatstr, repeatblock, ignorestr; bool repeatstrwhile, repeatblockwhile; bool back;
	
bool skip, cancel; int block = 0; string errors[1024], str, function; int i, symnum, strnum; vector<string> code;

string print[1024], args[1024]; int argnum, endargs; char ops[1024];

string argstr[1024]; int argint[1024]; float argfloat[1024]; bool argbool[1024];

string varnames[1024]; string vartypes[1024]; int vars = 0, selectedvar;

string varstr[1024]; int varint[1024]; float varfloat[1024]; bool varbool[1024];


string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ\0";//118
string symbols = "-+*/^!=<>~{}&?:\'\";\0";
string keywords[15] = {"skip", "str", "int", "float", "bool", "unknown", "assign", "строка", "число", "дробь", "булево", "тру", "фейк", "изрекаю", "обозначим"};

void error(string reason) {
	if (test == true)
		cout << " (ошибка) ";
	errors[0] = to_string(stoi(errors[0]) + 1);
	if (reason.length() == 2)
		reason += " - неожиданный символ";
	errors[stoi(errors[0])] = "строка " + to_string(strnum) + ": " + reason;
	cout << errors[1] << endl << "программа прервана" << endl;
	cout << "время выполнения программы: " << duration_cast<seconds>(high_resolution_clock::now() - timer).count() << " сек." << endl;
	Sleep(10000);
	exit(1);
}

void addvar(string name, string type) {
	vars++;
	varnames[vars] = name;
	if (type == "строка") {
		vartypes[vars] = "str";
		varstr[vars] = "";
		if (test == true)
			cout << endl << "переменная " << name << " записана" << endl << endl;
	}
	if (type == "число") {
		vartypes[vars] = "int";
		varint[vars] = 0;
		if (test == true)
			cout << endl << "переменная " << name << " записана" << endl << endl;
	}
	if (type == "дробь") {
		vartypes[vars] = "float";
		varfloat[vars] = 0;
		if (test == true)
			cout << endl << "переменная " << name << " записана" << endl << endl;
	}
	if (type == "булево") {
		vartypes[vars] = "bool";
		varbool[vars] = false;
		if (test == true)
			cout << endl << "переменная " << name << " записана" << endl << endl;
	}
	if (argnum > 2)
		error("функция обозначения переменной принимает только 2 аргумента (идентификатор, тип данных)");
}
void assignvar(int var) {
	for (i = 1; i < argnum + 1; i++)
		if (ops[i] == ',')
			error("переменная может принять множество аргументов, но не значений");
	if (ops[1] != ':')
		error("после идентификатора переменной нет символа присваивания ':'");
	if (vartypes[var] != args[argnum])
		error("типы данных не совпадают");
	else {
		if (vartypes[var] == "str")
			varstr[var] = argstr[argnum];
		if (vartypes[var] == "int")
			varint[var] = argint[argnum];
		if (vartypes[var] == "float")
			varfloat[var] = argfloat[argnum];
		if (vartypes[var] == "bool")
			varbool[var] = argbool[argnum];
	}
}

void readstr() {
	if (test == true)
		cout << symnum << " - ' - начало строки" << endl;
	symnum++;
	argnum++;
	args[argnum] = "str";
	if (str[symnum] != '\'')
		do {
			if (test == true)
				cout << symnum << " - " << str[symnum] << " - чтение строки..." << endl;
			argstr[argnum] = argstr[argnum] + str[symnum];
			symnum++;
		} while (str[symnum] != '\'' and symnum < str.length());
	if (test == true){
		cout << symnum << " - ' - конец строки, " << argnum << " аргумент записан";
		if (argstr[argnum].length() == 0)
			cout << " (пустая строка)";
		cout << endl;
	}
	if (symnum == str.length())
		error("текст не внутри ковычек");
	else
		symnum++;
	symnum--;
}
void readfloat(int first) {
	float precision = 10;	
	argint[argnum] = 0;
	argfloat[argnum] = first;
	args[argnum] = "float";
	argfloat[0] = 1;
	symnum++;
	if (test == true)
		cout << symnum - 1 << " - " << str[symnum - 1] << " - начало дробной части" << endl;
	do {		
		switch (str[symnum]) {//можно превратить в условие по коду символа
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
		if (argfloat[0] == 1)
		switch (str[symnum + 1]) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if (test == true)
					cout << symnum << " - " << str[symnum] << " - чтение дробной части..." << endl; break;
			default: 
				if (test == true)
					cout << symnum << " - " << str[symnum] << " - конец числа, " << argnum << " аргумент записан" << endl; 
				break;
		}
		symnum++;
		if (precision > 1)
			precision *= 10;		
	} while (argfloat[0] == 1);
	symnum--;
	precision /= 10;
}
void readint(int first){
	bool notint = false, zero = false;
	argint[0] = 1;
	argnum++;
	argint[argnum] = first;
	args[argnum] = "int";
	switch (str[symnum + 1]) {
		case '.': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			if (test == true)
				cout << symnum << " - " << first << " - начало числа" << endl;
			if (first == 0)
				switch (str[symnum + 1]) {
					case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': 
						error ("неверная запись числа"); break;//сократить фрагмент
					case '.': notint = true; symnum++; break;
					default:
						zero = true;
						symnum++;//+-
						break;
				}
			if (notint == false and zero == false)
				do {
					if (argint[0] == 1)
						symnum++;
					argint[argnum] *= 10;
					switch (str[symnum]) {
						case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
							switch (str[symnum + 1]) {
								case '.': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
									if (test == true)
										cout << symnum << " - " << str[symnum] << " - чтение числа... " << endl; break;						
							}
							argint[argnum] += (str[symnum]) - '0';
							break;
						case '.': notint = true; break;
						case '0': break;
						default: 
							argint[argnum] /= 10;
							argint[0] = 0;
							break;			
					}				
				}while (argint[0] == 1 && notint == false);
			if (notint == true)
				readfloat(argint[argnum] / 10);
			else
				if (test == true)
					cout << symnum - 1 << " - " << str[symnum - 1] << " - конец числа, " << argnum << " аргумент записан" << endl;
			symnum--;//+-
			break;
		default: 
			if (test == true)
				cout << symnum << " - " << str[symnum] << " - число, " << argnum << " аргумент записан" << endl;
			break;
	}
	if (argnum > 1 and ops[argnum] == 's')//'n'
		if (notint == true)
			error("нет запятой перед " + to_string(argfloat[argnum]));
		else
			error("нет запятой перед " + to_string(argint[argnum]));
}
void readidentifier() {
	string identifier;
	bool text, free = true;
	char valid[10] = { ' ', ';', ')', '-', '+', '*', '/' };
	argnum++;
	args[argnum] = "unknown";
	symnum--;
	do {
		symnum++;
		text = true;
		if (symnum == str.length()) {
			text = false;
			error("неожиданное окончание строки");
			symnum--;
		}
		if (text == false)
			break;
		text = false;
		for (i = 0; i < 118; i++)
			if (str[symnum] == alphabet[i]) {
				text = true;
				if (test == true)
					cout << symnum << " - " << str[symnum] << " - чтение идентификатора..." << endl;
			}
		if (text == true)
			identifier += str[symnum];
		else {
			for (i = 1; i < 8; i++)//!
				if (str[symnum] == valid[i])
					valid[0] == '1';
			if (valid[0] == '0')
				error("некорректная запись идентификатора");
		}
	} while (text == true);
	//переделать
	if (function == "обозначим") {
		if (argnum == 1)
			if (identifier != "тру" && identifier != "фейк" && identifier != "str" && identifier != "int" && identifier != "float" && identifier != "bool") {
				for (i = 1; i < 15; i++)
					if (identifier == keywords[i])
						free = false;
				for (i = 1; i < vars + 1; i++)
					if (identifier == varnames[i])
						free = false;
				if (free == false)
					error("идентификатор зарезервирован");
				else
					args[1] = identifier;
			}
			else
				error("идентификатор зарезервирован");
		if (argnum == 2) {
			if (identifier == "строка" or identifier == "число" or identifier == "дробь" or identifier == "булево")
				args[2] = identifier;
			else
				error("неизвестный тип");
		}
		if (argnum > 2)
			error("неверный формат обозначения переменной");
	}
	else {
		if (identifier != "тру" && identifier != "фейк")
			for (i = 1; i < 10; i++)
				if (identifier == keywords[i])
					error("идентификатор зарезервирован");
		if (identifier == "тру") {
			if (test == true)
				cout << "значение выявлено (тру), аргумент записан" << endl;
			argbool[argnum] = true;
			args[argnum] = "bool";
		}
		if (identifier == "фейк") {
			if (test == true)
				cout << "значение выявлено (фейк), аргумент записан" << endl;
			argbool[argnum] = false;
			args[argnum] = "bool";
		}
		for (i = 1; i < vars + 1; i++)
			if (identifier == varnames[i])
				if (function == "спросить" or function == "петля")
					args[argnum] = identifier;
				else{
					args[argnum] = vartypes[i];
					if (vartypes[i] == "str")
						argstr[argnum] = varstr[i];
					if (vartypes[i] == "int")
						argint[argnum] = varint[i];
					if (vartypes[i] == "float")
						argfloat[argnum] = varfloat[i];
					if (vartypes[i] == "bool")
						argbool[argnum] = varbool[i];
				}
	}
	if (args[argnum] == "unknown") {
		error("неизвестное имя: " + identifier);
		argnum--;
		if (test == true)
			cout << "идентификатор \"" << identifier << "\" неизвестен, отмена записи аргумента" << endl;
	}
	symnum--;
	if (argnum > 1 and ops[argnum] == 's')
		error("нет запятой");
}

void getvalues() {
	if (test == true)
		cout << "получение значений..." << endl;
	int num;
	for (num = 1; num < argnum + 1; num++) {
		if (args[num] == "str" or args[num] == "int" or args[num] == "float" or args[num] == "bool" or args[num] == "action")
			continue;
		varnames[vars + 1] == "0";//!
		for (i = 1; i < vars + 2; i++)
			if (args[num] == varnames[i])
				break;
		if (i == vars + 1)
			error("неизвестное имя: " + args[num]);
		else {
			args[num] = vartypes[i];
			if (args[num] == "str")
				argstr[num] = varstr[i];
			if (args[num] == "int")
				argint[num] = varint[i];
			if (args[num] == "float")
				argfloat[num] = varfloat[i];
			if (args[num] == "bool")
				argbool[num] = varbool[i];
			//if (args[num] == "action")
				//argact[num] = varargt[i];
		}
	}
}
void perform() {
	string operands;
	int first = 1;
	bool firstable = false;
	if (test == true){
		cout << endl << "применение математических операций к аргументам (";
		for (i = 1; i < argnum + 1; i++) {
			if (i > 1)
				cout << ' ' << ops[i] << ' ';
			if (args[i] == "str")
				cout << argstr[i];
			if (args[i] == "int")
				cout << argint[i];
			if (args[i] == "float")
				cout << argfloat[i];
			if (args[i] == "bool")
				if (argbool[i] == true)
					cout << "тру";
				else
					cout << "фейк";
		}
		cout << ')' << endl << endl;
	}
	for (i = 2; i < argnum + 1; i++) {
		operands = args[i - 1] + args[i];
		if (ops[i] == '^') {
			if (operands == "strstr")
				error("возведение в степень не применимо к строкам");
			if (operands == "strint") 
				error("возведение в степень не применимо к строкам");
			if (operands == "strfloat")
				error("возведение в степень не применимо к строкам");
			if (operands == "strbool")
				error("возведение в степень не применимо к строкам");

			if (operands == "intstr")
				error("строка не может быть степенью");
			if (operands == "intint") {
				if (argint[i] < 0) {
					args[i] = "float";
					argfloat[i] = pow(argint[i - 1], argint[i]);
				}
				else
					if (argint[i] == 0)
						argint[i] = 1;
					else
						//if (argint[i] > 0)
							argint[i] = pow(argint[i - 1], argint[i]);
			}
				
			if (operands == "intfloat")
				argfloat[i] = pow(argint[i - 1], argfloat[i]);
			if (operands == "intbool")
				error("возведение в степень не применимо к булевым ");

			if (operands == "floatstr")
				error("строка не может быть степенью");
			if (operands == "floatint") {
				args[i] = "float";
				argfloat[i] = pow(argfloat[i - 1], argint[i]);
			}
			if (operands == "floatfloat")
				argfloat[i] = pow(argfloat[i - 1], argfloat[i]);
			if (operands == "floatbool")
				error("возведение в степень не применимо к булевым ");

			if (operands == "boolstr")
				error("возведение в степень не применимо к булевым ");
			if (operands == "boolint")
				error("возведение в степень не применимо к булевым ");
			if (operands == "boolfloat")
				error("возведение в степень не применимо к булевым ");
			if (operands == "boolbool")
				error("возведение в степень не применимо к булевым ");

			if (ops[i] == '^') {
				first++;
				while (i > first) {
					ops[i] = ops[i - 1];
					i--;
					args[i] = args[i - 1];
					argstr[i] = argstr[i - 1];
					argint[i] = argint[i - 1];
					argfloat[i] = argfloat[i - 1];
					argbool[i] = argbool[i - 1];
				}
				args[first - 1] = "skip";
				ops[first] = 's';
				i = first;
			}
		}
	}
	do {
		for (i = 2; i < argnum + 1; i++) {
			operands = args[i - 1] + args[i];
			if (ops[i] == '*') {	
				if (operands == "strstr")
					error("строки нельзя перемножать");
				if (operands == "strint") {
					args[i] = "str";
					argstr[i] = "";
					for (argint[i]; argint[i] > 0; argint[i]--)
						argstr[i] += argstr[i - 1];
				}
				if (operands == "strfloat")
					error("строку нельзя умножить на дробь");
				if (operands == "strbool") {
					args[i] = "str";
					argstr[i] = "";
					if (argbool[i] == true)
						argstr[i] = argstr[i - 1];
				}
				if (operands == "intstr") {
					argstr[i - 1] = "";
					for (argint[i - 1]; argint[i - 1] > 0; argint[i - 1]--)
						argstr[i - 1] += argstr[i];
					argstr[i] = argstr[i - 1];
				}
				if (operands == "intint")
					argint[i] *= argint[i - 1];
				if (operands == "intfloat")
					argfloat[i] *= argint[i - 1];
				if (operands == "intbool") {
					args[i] = "int";
					argint[i] = argint[i - 1] * argbool[i];
				}
				if (operands == "floatstr")
					error("дробь нельзя умножить на строку");
				if (operands == "floatint") {
					args[i] = "float";
					argfloat[i] = argfloat[i - 1] * argint[i];
				}
				if (operands == "floatfloat")
					argfloat[i] *= argfloat[i - 1];
				if (operands == "floatbool") {
					args[i] = "float";
					argfloat[i] = argfloat[i - 1] * argbool[i];
				}
				if (operands == "boolstr")
					if (argbool[i - 1] == false)
						argstr[i] = "";
				if (operands == "boolint")
					argint[i] *= argbool[i - 1];
				if (operands == "boolfloat")
					argfloat[i] *= argbool[i - 1];
				if (operands == "boolbool")
					argbool[i] *= argbool[i - 1];
			}
			if (ops[i] == '/') {
				if (operands == "strstr")
					error("строки нельзя делить");
				if (operands == "strint") {
					if (argint[i] > argstr[i - 1].length())
						error(to_string(argint[i]) + " превышает количество символов в '" + argstr[i - 1] + '\'');
					else
						if (argint[i] == 0)
							error("на ноль делить нельзя");
						else {
							args[i] = "str";
							argstr[i] = "";
							for (symnum = 0; symnum < floor(argstr[i - 1].length() / argint[i]); symnum++)
								argstr[i] += argstr[i - 1][symnum];
						}	
				}
				if (operands == "strfloat")
					error("строку нельзя делить на дробь");
				if (operands == "strbool")
					error("строку нельзя делить на булево");
				if (operands == "intstr")
					error("нельзя делить на строку");
					if (operands == "intint")
						if (argint[i] != 0)
							if (autoround == true)
								argint[i] = argint[i - 1] / argint[i];
							else {
								args[i - 1] = "float";
								args[i] = "float";
								argfloat[i - 1] = argint[i - 1];
								argfloat[i] = argint[i];
								argfloat[i] = argfloat[i - 1] / argfloat[i];
							}
						else
							error("на ноль делить нельзя");
				if (operands == "intfloat"){
					if (argfloat[i] != 0)
						argfloat[i] = argint[i - 1] / argfloat[i];
					else
						error("на ноль делить нельзя");
				}
				if (operands == "intbool")
					error("нельзя делить на булево");
				if (operands == "floatstr")
					error("нельзя делить на строку");
				if (operands == "floatint")
					if (argint[i] != 0) {
						args[i] = "float";
						argfloat[i] = argfloat[i - 1] / argint[i];
					}
					else
						error("на ноль делить нельзя");
				if (operands == "floatfloat")
					if (argfloat[i] != 0)
						argfloat[i] = argfloat[i - 1] / argfloat[i];
					else
						error("на ноль делить нельзя");
				if (operands == "floatbool")
					error("нельзя делить на булево");
				if (operands == "boolstr")
					error("нельзя делить на строку");
				if (operands == "boolint")
					error("булево неделимо");
				if (operands == "boolfloat")
					error("булево неделимо");
				if (operands == "boolbool")
					error("булево неделимо");					
			}
			if (ops[i] == '*' or ops[i] == '/') {
				first++;
				while (i > first) {
					ops[i] = ops[i - 1];
					i--;
					args[i] = args[i - 1];
					argstr[i] = argstr[i - 1];
					argint[i] = argint[i - 1];
					argfloat[i] = argfloat[i - 1];
					argbool[i] = argbool[i - 1];
				}
				args[first - 1] = "skip";
				ops[first] = 's';
			}
		}
		firstable = false;
		for (i = 2; i < argnum + 1; i++)
			if (ops[i] == '*' or ops[i] == '/')
				firstable = true;
	} while (firstable == true);
	for (i = first + 1; i < argnum + 1; i++){
		operands = args[i - 1] + args[i];
		switch (ops[i]) {
			case '-':{
				if (operands == "strstr")
					if (argstr[i - 1].find(argstr[i]) == string::npos)
						error('\'' + argstr[i - 1] + "' не содержит '" + argstr[i] + '\'');
					else
						argstr[i] = argstr[i - 1].erase(argstr[i - 1].find(argstr[i]), argstr[i].length());
				if (operands == "strint")
					if (argint[i] > argstr[i - 1].length())
						error("число не должно привышать длину строки");
					else{
						args[i] = "str";
						argstr[i] = "";
						for (symnum = 0; symnum < argstr[i - 1].length() - argint[i]; symnum++)
							argstr[i] += argstr[i - 1][symnum];
					}
				if (operands == "strfloat")
					error("из строки нельзя вычесть дробное число символов");
				if (operands == "strbool")
					if (argstr[i - 1].find("тру") == string::npos && argstr[i - 1].find("фейк") == string::npos)
						error("в строке '" + argstr[i - 1] + "' нет булевых значений");
					else {
						args[i] = "str";
						argstr[i] = argstr[i - 1];
						if (argbool[i] == true)
							if (argstr[i - 1].find("тру") != string::npos)
								argstr[i] = argstr[i - 1].erase(argstr[i - 1].find("тру"), 3);							
						if (argbool[i] == false)
							if (argstr[i - 1].find("фейк") != string::npos)
								argstr[i] = argstr[i - 1].erase(argstr[i - 1].find("фейк"), 4);							
					}					
				if (operands == "intstr")
					error("нельзя вычесть строку из числа");
				if (operands == "intint")
					argint[i] = argint[i - 1] - argint[i];
				if (operands == "intfloat")
					argfloat[i] = argint[i - 1] - argfloat[i];
				if (operands == "intbool") {
					args[i] = "int";
					argint[i] = argint[i - 1] - argbool[i];
				}
				if (operands == "floatstr")
					error("нельзя вычесть строку из дроби");
				if (operands == "floatint") {
					args[i] = "float";
					argfloat[i] = argfloat[i - 1] - argint[i];
				}
				if (operands == "floatfloat")
					argfloat[i] = argfloat[i - 1] - argfloat[i];
				if (operands == "floatbool") {
					args[i] = "float";
					argfloat[i] = argfloat[i - 1] - argbool[i];
				}
				if (operands == "boolstr")
					error("нельзя вычесть строку из булева");
				if (operands == "boolint")
					argint[i] = argbool[i - 1] - argint[i];
				if (operands == "boolfloat")
					argfloat[i] = argbool[i - 1] - argfloat[i];
				if (operands == "boolbool") 
					if (argbool[i - 1] == true and argbool[i] == false)
						argbool[i] = true;
					else
						argbool[i] = false;
				break;
			}
			case '+': {
				if (operands == "strstr")
					argstr[i] = argstr[i - 1] + argstr[i];
				if (operands == "strint") {
					args[i] = "str";
					argstr[i] = argstr[i - 1] + to_string(argint[i]);
				}					
				if (operands == "strfloat") {
					args[i] = "str";
					argstr[i] = argstr[i - 1] + to_string(argfloat[i]);
				}
				if (operands == "strbool") {
					args[i] = "str";
					if (argbool[i] == true)
						argstr[i] = argstr[i - 1] + "тру";
					if (argbool[i] == false)
						argstr[i] = argstr[i - 1] + "фейк";
				}	
				if (operands == "intstr")
					argstr[i] = to_string(argint[i - 1]) + argstr[i];
				if (operands == "intint")
					argint[i] += argint[i - 1];
				if (operands == "intfloat")
					argfloat[i] += argint[i - 1];
				if (operands == "intbool") {
					args[i] = "int";
					argint[i] = argint[i - 1] + argbool[i];
				}
				if (operands == "floatstr")
					argstr[i] = to_string(argfloat[i - 1]) + argstr[i];
				if (operands == "floatint") {
					args[i] = "float";
					argfloat[i] = argfloat[i - 1] + argint[i];
				}
				if (operands == "floatfloat")
					argfloat[i] += argfloat[i - 1];
				if (operands == "floatbool") {
					args[i] = "float";
					argfloat[i] = argfloat[i - 1] + argbool[i];
				}
				if (operands == "boolstr")
					if (argbool[i - 1] == true)
						argstr[i] = "тру" + argstr[i];
					else
						argstr[i] = "фейк" + argstr[i];				
				if (operands == "boolint")
					argint[i] += argbool[i - 1];
				if (operands == "boolfloat")
					argfloat[i] += argbool[i - 1];
				if (operands == "boolbool")
					argbool[i] += argbool[i - 1];
				break;
			}	
			case 's': case ',': case ':': break;
			case '!': case '=': case '<': case '>': 
			case '{': case '}': case '"': case '~':  case '?':
				if (comparison == '\0')
					comparison = ops[i];
				else
					error("вид сравнения уже задан");
				break;
			default: error("действие не задано"); break;
		}
		if (ops[i] == '+' or ops[i] == '-') {
			first++;
			while (i > first) {
				ops[i] = ops[i - 1];
				i--;
				args[i] = args[i - 1];
				argstr[i] = argstr[i - 1];
				argint[i] = argint[i - 1];
				argfloat[i] = argfloat[i - 1];
				argbool[i] = argbool[i - 1];
			}
			args[first - 1] = "skip";
			ops[first] = 's';
			i = first;
		}		
	}	
}
bool compare(string left, char type, string right){//перегрузить
	if (test == true) {
		//cout << "левое значение:" << left << endl;
		cout << "вид сравнения: " << type << endl;
		//cout << "правое значение:" << right << endl;
		cout << "результат: ";
	}
	//getvalues();
	if (left != right) {
		if (test == true)
			cout << "(конфликт типов) " << endl;
		return false;
	}
	if (left == "str" and right == "str")
		switch (type) {
			case '!':
				if (argstr[1] != argstr[2])
					return true;
				break;
			case '=': 
				if (argstr[1] == argstr[2])
					return true;
				break;
			case '<':
				if (argstr[1].length() < argstr[2].length())
					return true;
				break;
			case '>':
				if (argstr[1].length() > argstr[2].length())
					return true;
				break;
		}	
	if (left == "int" and right == "int")
		switch (type) {
			case '!':
				if (argint[1] != argint[2])
					return true;
				break;
			case '=':
				if (argint[1] == argint[2])
					return true;
				break;
			case '<':
				if (argint[1] < argint[2])
					return true;
				break;
			case '>':
				if (argint[1] > argint[2])
					return true;
				break;
		}
	if (left == "float" and right == "float")
		switch (type) {
			case '!':
				if (argfloat[1] != argfloat[2])
					return true;
				break;
			case '=':
				if (argfloat[1] == argfloat[2])
					return true;
				break;
			case '<':
				if (argfloat[1] < argfloat[2])
					return true;
				break;
			case '>':
				if (argfloat[1] > argfloat[2])
					return true;
				break;
		}
	if (left == "bool" and right == "bool")
		switch (type) {
			case '!':
				if (argbool[1] != argbool[2])
					return true;
				break;
			case '=':
				if (argbool[1] == argbool[2])
					return true;
				break;
			case '<':
				if (argbool[1] < argbool[2])
					return true;
				break;
			case '>':
				if (argbool[1] > argbool[2])
					return true;
				break;
		}
	return false;
}


int varcheck(string arg) {
	for (i = 1; i < vars + 1; i++)
		if (arg == varnames[i])
			return i;
	return 0;
}
bool checkmatch(string literal, string type) {//getargs()
	bool point = false;
	int symnum = 0;//!
	if (type == "str") {
		for (symnum = 0; symnum < literal.length(); symnum++)
			if (literal[symnum] == '\t' or literal[symnum] == '\n' or literal[symnum] == '\0' or literal[symnum] == '|')
				return false;
		return true;
	}
	if (type == "int") {
		for (symnum = 0; symnum < literal.length(); symnum++)
			switch (literal[symnum]) {
				case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
					break;
				case '-':
					if (symnum > 0)
						return false;
					break;
				default:
					return false;
			}
		return true;
	}
	if (type == "float") {
		for (symnum = 0; symnum < literal.length(); symnum++)
			switch (literal[symnum]) {
				case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
					break;
				case '.': case ',':
					if (point == false)
						point = true;
					else
						return false;
					break;
				case '-':
					if (symnum > 0)
						return false;
					break;
				default:
					return false;
			}
		//if (point == true)
			return true;
		//else
			return false;
	}
	return false;
}
void askvar(int var){	
	string value;
	getline(cin, value);

	//string valuestr;
	//int valueint;
	//float valuefloat;
	//if (vartypes[var] == "str")
		//getline(cin, valuestr);
	//if (vartypes[var] == "int")
		//cin >> valueint;
	//if (vartypes[var] == "float")
		//cin >> valuefloat;

	if (checkmatch(value, vartypes[var]) == true) {
		if (vartypes[var] == "str")
			varstr[var] = value;
		if (vartypes[var] == "int")
			varint[var] = stoi(value);
		if (vartypes[var] == "float")
			varfloat[var] = stof(value);
	}
	else
		cout << "ошибка ввода: конфликт типов данных" << endl;
}
void askvar(int var, string mistake) {
	string value;
	getline(cin, value);
	if (checkmatch(value, vartypes[var]) == true) {
		if (vartypes[var] == "str")
			varstr[var] = value;
		if (vartypes[var] == "int")
			varint[var] = stoi(value);
		if (vartypes[var] == "float")
			varfloat[var] = stof(value);
	}
	else
		cout << mistake << endl;
}

string output() {
	string outstr = "";
	for (i = 1; i < argnum + 1; i++) {
		if (autospace == true && i > 1 && args[i - 1] != "skip")
			outstr += ' ';
		if (args[i] == "int")
			outstr += to_string(argint[i]);
		if (args[i] == "float")
			outstr += to_string(argfloat[i]);
		if (args[i] == "str")
			outstr += argstr[i];
		if (args[i] == "bool")
			if (argbool[i] == true)
				outstr += "тру";
			else
				outstr += "фейк";
	}
	return outstr;
}

void getargs(bool parenthesized, int min, int max) {
	bool open = false;
	bool closed = false;
	bool negative = false;
	if (test == true)
		cout << endl << "аргументация:" << endl;
	for (symnum = (function == "assign" ? varnames[selectedvar].length() : function.length()); symnum < str.length() and errors[0] == "0"; symnum++) {
		if (ops[argnum + 1] == '\0')
			ops[argnum + 1] = 's';
		endargs = symnum;
		switch (str[symnum]) {
			case ' ': 
				if (test == true)
					cout << symnum << " -   - пробел" << endl; 
				break;
			case ',':
				if (argnum == 0)
					error("перечисления не начинаются с запятой");
				if (ops[argnum + 1] == ',')
					error("лишняя запятая");
				ops[argnum + 1] = ',';
				if (test == true)
					cout << symnum << " - , - запятая" << endl;
				break;
			case ';':
				if (test == true)
					cout << symnum << " - ; - конец команды" << endl << endl;
				if (parenthesized == true and closed == true)
					symnum--;
				return;
				break;
			case '!': case '=': case '<': case '>': case '~':
			case '{': case '}': case '"': case '&': case '?':
			case '+': case '*': case '^': case '/': case ':':
				ops[argnum + 1] = str[symnum];//-1
				ops[0] = 'T';
				if (test == true)
					cout << symnum << " - " << str[symnum] << " - действие задано" << endl;
				break;
			case '-':
				switch (str[symnum + 1]) {
					case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
						negative = true;
						if (test == true)
							cout << symnum << " - " << str[symnum] << " - установлен отрицательный знак" << endl;
						break;
					default:
						ops[argnum + 1] = '-';
						ops[0] = 'T';
						if (test == true)
							cout << symnum << " - " << str[symnum] << " - действие задано" << endl;
						break;
				}
				break;
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				readint(str[symnum] - '0'); 
				if (negative == true) {
					if (args[argnum] == "int")
						argint[argnum] *= -1;
					if (args[argnum] == "float")
						argfloat[argnum] *= -1;
					//if (args[argnum] == "bool")
					negative = false;
				}
				break;
			case '(':
				if (test == true)
					cout << symnum << " - ( - начало аргументации" << endl;
				if (open == true)
					error("повторное начало аргументации");
				open = true;
				break;
			case ')':
				if (test == true)
					cout << symnum << " - ) - конец аргументации" << endl;
				//str[symnum] = '\0';
				if (closed == true)
					error("повторное закрытие скобки");
				closed = true;				
				return;
				break;
			case '\'': readstr(); break;
			default:
				if (alphabet.find(str[symnum]) != alphabet.npos)
					readidentifier();
				else
					error(to_string(str[symnum]));
				break;
		}
	}

	if (parenthesized == true){
		if (open == false)
			error("нет открывающей скобки");
		if (closed == false)
			error("нет закрывающей скобки");		
	}
	if (argnum == 0)
		error("аргументы отсутствуют");	
}
string removespaces(string strget) {
	if (strget == "")
		return "";
	for (symnum = 0; symnum < strget.length(); symnum++) {
		if (strget[symnum] == '\t' or strget[symnum] == ' ') {
			strget.erase(symnum, 1);
			symnum--;
		}
	}
	return strget;	
}//в одну функцию
string clearstr(string strget) {
	string cleanstr = "";
	if (strget == "")
		return "";
	if (test == true)
		cout << "удаление пробелов..." << endl;
	for (symnum = 0; symnum < strget.length(); symnum++) {
		for (i = 0; i < 119; i++)
			if (strget[symnum] == alphabet[i])
				break;
		if (i < 119)
			break;
		for (i = 0; i < 19; i++)
			if (strget[symnum] == symbols[i])
				break;
		if (i < 21)
			break;
		strget.erase(symnum, 1);
		symnum--;
	}
	for (symnum = 0; symnum < strget.length() + 1; symnum++)
		if (strget[symnum] == '\t' or strget[symnum] == '\n') {
			strget.erase(symnum, 1);
			symnum--;
		}
	for (symnum = 0; symnum < strget.length() + 1; symnum++)
		if (strget[symnum] != '\t' and strget[symnum] != '\n' and strget[symnum] != ' ')
			break;
	cleanstr = strget;
	if (symnum == cleanstr.length())
		return "";
	else
		return cleanstr;
	if (test == true)
		cout << "обработанная строка: " << cleanstr << endl;
}
string afterargs(bool expect) {
	string end = "";
	symnum = 0;
	//while (symnum < str.length() and str[symnum] != ')')
		//symnum++;
	symnum = endargs;
	if (str[symnum] == ')')
		symnum++;
	for (symnum; symnum < str.length(); symnum++)
		end += str[symnum];
	end = clearstr(end);
	if (function != "вдруг" && function != "петля" && end != ";")
	//if (expect == false && end != ";")		
		return "error";		
	if (function == "вдруг" or function == "петля")
		if (removespaces(end) == "поехали") {
			return "error";
			/*
			if (block > 0 and block < strnum)
				error("начало блока кода уже задано");
			//block = strnum;
			if (test == true)
				cout << "начало блока кода" << endl;
			//block = strnum;
			return "start";
			*/
		}
		else
			if (end == "")
				return "";
			else
				return "error";
	if (end == ";")
		return "semicolon";
	return "error";		
}
void checkendblock(int start) {
	bool end = false;
	bool newblock = false;
	for (strnum = start; strnum < code.size() - 1; strnum++) {
		if (clearstr(code[strnum]) == "хватит;")
			end = true;
		if (clearstr(code[strnum]) == "поехали;" and end == false)
			error("вложенные конструкции запрещены");
	}
	if (end == false)
		error("окончание блока не задано");
	strnum = start;
	str = clearstr(code[strnum]);
}

void clean() {
	for (i = 0; i < argnum + 1; i++) {
		ops[i] = 's';
		args[i] = "";
		argstr[i] = "";
		argint[i] = NULL;
		argfloat[i] = NULL;
		argbool[i] = NULL;
	}
	ops[0] = 'F';
	argnum = 0;
	//result = "";
}
void fullclean() {}

void skipblock(){
	if (test == true)
		cout << "пропуск блока" << endl;
	do
		strnum++;
	while (strnum < code.size() - 1 and clearstr(code[strnum]) != "хватит;");
	block = 0;
}

int main() {
	setlocale(LC_ALL, "");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	//string file;
	//getline(cin, file);
	print[0] = "0";
	errors[0] = "0";
	repeatblock = 0;
	repeatstr = 0;
	ignorestr = 0;
	code.push_back("");
	ifstream file("код.txt");	
	while (getline(file, str))
		code.push_back(str);
	endargs = 0;
	for (strnum = 1; strnum < code.size(); strnum++) {
		argint[0] = 0;
		argstr[0] = "";
		argnum = 0;
		symnum = 0;
		ops[0] = 'F';
		comparison = '\0';
		function = "none";

		begin:		
		if (errors[0] != "0")
			break;
		
		//strnum++;
		str = clearstr(code[strnum]);
		if (str == "")
			continue;
		if (strnum == ignorestr)
			continue;
		if (skip == true) {
			if (block > 0)
				skipblock();
			skip = false;
			continue;
		}
		
		
		if (str == "хватит;") {
			if (test == true)
				cout << "окончание блока кода" << endl;
			if (block == 0 and repeatblock == 0)
				error("начало блока кода не задано");
			else
				if (repeatblock == 0 and repeatblockwhile == false) {
					block = 0;
					continue;
					//str = clearstr(code[strnum]);
				}
				else {
					if (repeatblock > 0) {
						//repeatblock--;
						if (test == true)
							cout << "итераций осталось : " << repeatblock << endl << endl;
						strnum = block - 1;
						str = clearstr(code[strnum]);
						continue;
					}
					if (repeatblockwhile == true) {
						strnum = block - 1;
						str = clearstr(code[strnum]);
						continue;
					}
						
				}				
					
		}
		if (str == "поехали") {
			checkendblock(strnum);
			function = "поехали";
			//if (block > 0)// and block != strnum)
				//error("вложенный блок кода");
			//else
				if (strnum > 1) {
					//проверить прошлые строки на условия и циклы	
					ignorestr = strnum;
					block = strnum - 1;
				}
				else
					error("блок кода не может быть началом программы");
			continue;
		}
			
		if (str == "отбой;") {
			if (test == true)
				cout << "прерывание блока кода" << endl;
			if (block == 0)
				error("прерывание вне блока кода");
			else
				skipblock();
			continue;			
		}

		if (str.rfind("вдруг", 0) == 0) {
			
			function = "вдруг";
			if (test == true)
				cout << endl << "функция: сравнивание" << endl;
			getargs(true, 2, 0);
			if (afterargs(false) == "error")
				error("не предусмотренное продолжение команды");
			if (ops[0] == 'T')
				perform();
			for (i = 1; argnum; i++) {
				if (ops[i + 1] == ',')
					error("перечисления недопустимы в условиях");
				if (args[i] == "str") {
					args[1] = "str";
					argstr[1] = argstr[i];
					break;
				}
				if (args[i] == "int") {
					args[1] = "int";
					argint[1] = argint[i];
					break;
				}
				if (args[i] == "float") {
					args[1] = "float";
					argfloat[1] = argfloat[i];
					break;
				}
				if (args[i] == "bool") {
					args[1] = "bool";
					argbool[1] = argbool[i];
					break;
				}
			}
			args[2] = args[argnum];
			if (args[2] == "str")
				argstr[2] == argstr[argnum];
			if (args[2] == "int")
				argint[2] == argint[argnum];
			if (args[2] == "float")
				argfloat[2] == argfloat[argnum];
			if (args[2] == "bool")
				argbool[2] == argbool[argnum];
			if (errors[0] != "0")
				cout << "ошибка" << endl;
			else
				if (compare(args[1], comparison, args[2]) == false) {
					if (test == true)
						cout << "фейк" << endl;
					//if (afterargs(true) == "start" or clearstr(code[symnum + 1]) == "поехали")
					if (clearstr(code[strnum + 1]) == "поехали")
						skipblock();
					else
						skip = true;
				}
				else {
					if (test == true)
						cout << "тру" << endl;
					//if (afterargs(true) == "start" or clearstr(code[symnum + 1]) == "поехали")
					if (clearstr(code[strnum + 1]) == "поехали")
						block = strnum;
				}
					
		}
		if (str.rfind("петля", 0) == 0) {
			
			if (repeatstr > 0) {
				repeatstr--;
				continue;
			}
			if (repeatblock > 0) {
				repeatblock--;
				if (repeatblock == 0)
					skip = true;
				continue;
			}
			function = "петля";
			if (test == true)
				cout << endl << "функция: петля" << endl;
			getargs(true, 1, 2);
			if (afterargs(false) == "error")
				error("не предусмотренное продолжение команды");
			if (ops[0] == 'T')
				perform();

			if (repeatstr > 0 or repeatblock > 0)
				if (strnum > block)
					error("вложенная петля не может быть вызвана (пока)");



			if (strnum == code.size() - 1)
				error("непредвиденное прерывание программы");
			else {
				//if (clearstr(code[strnum + 1]) != "поехали" and afterargs(true) != "start") {
				if (clearstr(code[strnum + 1]) != "поехали"){
					if (comparison == '\0')//repeatstr
						if (argnum == 1) {
							if (test == true)
								cout << "задан цикл с счетчиком" << endl;
							if (varcheck(args[1]) > 0)
								if (vartypes[i] == "int")//сделать нормально
									repeatstr = varint[i];
								else
									error("счетчик принимает только числовое значение");
							else {
								if (args[1] == "int")
									repeatstr = argint[1];
								else
									if (args[i] == "float")
										error("счетчик принимает только целое число");
									else
										error("счетчик принимает только числовое значение");
							}							
							if (repeatstr < 0)
								repeatstr *= -1;
							if (repeatstr == 0)
								skip = true;
						}
						else
							error("неверная аргументация цикла");
					else {//repeatstrwhile
						if (test == true)
							cout << "задан цикл с условием" << endl;

						if (test == true)
							cout << endl << "функция: сравнивание" << endl;
						getvalues();
						if (compare(args[1], comparison, args[2]) == false)
							skip = true;
						else
							back = true;



						//repeatstrwhile.left = args[1];
						//repeatstrwhile.comparison = ops[2];
						//repeatstrwhile.right = args[2];
					}

				}
				else {
					if (comparison == '\0'){//repeatblock							
						if (argnum == 1) {
							if (test == true)
								cout << "задан цикл с счетчиком" << endl;

							getvalues();
							if (args[1] == "int") 
								if (argint[1] != 0) {
									block = strnum;
									repeatblock = argint[1];
								}
								else
									error("нулевой цикл");
							else
								if (args[i] == "float")
									error("счетчик принимает только целое число");
								else
									error("счетчик принимает только числовое значение");

							if (repeatblock < 0)
								repeatblock *= -1;
						}
						else
							error("неверная аргументация цикла");

						//if (clearstr(code[strnum + 1]) == "поехали;")
							//block = strnum + 1;
						//if (afterargs(true) == "start")
							//block = strnum;
					}
					else {//repeatblockwhile
						if (test == true)
							cout << "задан цикл с условием" << endl;
						repeatblockwhile = false;
						if (argnum == 2) {
							getvalues();
							//if (clearstr(code[strnum + 1]) == "поехали" or afterargs(true) == "start") {
							if (clearstr(code[strnum + 1]) == "поехали"){
								block = strnum;
								if (test == true)
									cout << endl << "функция: сравнивание" << endl;
								if (compare(args[1], comparison, args[2]) == false)
									//skip = true;
									skipblock();
								else {
									//back = true;
									repeatblockwhile = true;//varint[1]
								}
									
							}
							//repeatblockwhile.left = args[1];
							//repeatblockwhile.comparison = ops[2];
							//repeatblockwhile.right = args[2];

						}
						else
							error("неверная аргументация цикла");
						//if (clearstr(code[strnum + 1]) == "поехали;")
								//block = strnum + 1;
						//if (afterargs(true) == "start")
							//block = strnum;
					}
				}
			}
			continue;
		}

		if (str.rfind("изрекаю", 0) == 0) {
			function = "изрекаю";
			if (test == true)
				cout << endl << "функция: изрекаю" << endl;
			getargs(true, 0, 0);
			if (ops[0] == 'T')
				perform();
			if (test == true)
				cout << endl << "вывод: ";
			cout << output() << endl;
			print[0] = to_string(stoi(print[0]) + 1);
			print[stoi(print[0])] = output();				
		}		

		if (str.rfind("обозначим", 0) == 0) {
			function = "обозначим";
			if (test == true)
				cout << endl << "функция: обозначим" << endl;
			getargs(true, 2, 2);
			if (args[1] != "unknown")
				if (args[2] == "строка" or args[2] == "число" or args[2] == "дробь" or args[2] == "булево")
					addvar(args[1], args[2]);
		}			
		if (str.rfind("спросить", 0) == 0) {
			function = "спросить";
			getargs(false, 1, 2);
			for (i = 1; i < vars + 2; i++)
				if (args[1] == varnames[i])
					break;
			if (i < vars + 1) {
				if (argnum == 1)
					askvar(i);
				if (argnum == 2)
					askvar(i, argstr[2]);
				//error("нельзя вводить двоичные данные");
			}

		}
		for (i = 1; i < vars + 1; i++)
			if (str.rfind(varnames[i], 0) == 0) {
				function = "assign";
				selectedvar = i;
				getargs(false, 1, 0);
				if (ops[0] == 'T')
					perform();
				assignvar(selectedvar);
			}		

		if (function == "none")
			error("функция не задана");	

		//if (function != "вдруг" and function != "петля")
			if (afterargs(false) == "error")
				error("не предусмотренное продолжение команды");

		if (test == true) {
			cout << endl << "переменные:" << endl;
			for (i = 1; i < vars + 1; i++) {
				cout << i << ' ' << varnames[i] << " (" << vartypes[i] << ") = ";
				if (vartypes[i] == "str")
					cout << '\'' << varstr[i] << '\'' << endl;
				if (vartypes[i] == "int")
					cout << varint[i] << endl;
				if (vartypes[i] == "float")
					cout << varfloat[i] << endl;
				if (vartypes[i] == "bool")
					if (varbool[i] == true)
						cout << "тру" << endl;
					else
						cout << "фейк" << endl;
			}
			cout << endl;
		}		
		if (repeatstr > 1) {
			repeatstr--;
			strnum--;
		}
		if (back == true) {
			back = false;
			strnum -= 2;
		}
		


		clean();
		//if (repeat > 0)
			//goto begin;
	}
	cout << endl << endl << endl << "программа завершена без ошибок" << endl;
	cout << "время выполнения программы: " << duration_cast<seconds>(high_resolution_clock::now() - timer).count() << " сек." << endl;
	//fullclean();
	Sleep(100000);
	/*
	for (i = 10; i >= 0; i--) {
		cout << "автоматическое закрытие консоли через " << i << endl;
		Sleep(1000);
		cout << "\033[F\033[K";		
	}	
	exit(0);	
	*/
}