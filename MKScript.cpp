#include <iostream>
#include <string>
#include <cmath>
#include <locale>
#include <windows.h>
#include <fstream>
using namespace std;

bool autoround = true, autospace = true, test = false;

bool skip; string errors[1024], str, strget, function; int i, symnum, strnum;

string print[1024], args[1024]; int argnum; char ops[1024], comparison; bool start, finish;

string argstr[1024]; int argint[1024]; float argfloat[1024]; bool argbool[1024];

string varnames[1024]; string vartypes[1024]; int vars = 0, selectedvar;

string varstr[1024]; int varint[1024]; float varfloat[1024]; bool varbool[1024];

string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ\n";//118
string keywords[15] = {"skip", "str", "int", "float", "bool", "unknown", "assign", "строка", "число", "дробь", "булево", "тру", "фейк", "изрекаю", "обозначим"};

void error(string reason) {
	if (test == true)
		cout << " (ошибка) ";
	errors[0] = to_string(stoi(errors[0]) + 1);
	if (reason.length() == 2)
		reason += " - неожиданный символ";
	errors[stoi(errors[0])] = "строка " + to_string(strnum) + ": " + reason;
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
	if (function == "обозначим") {
		if (argnum == 1)
			if (identifier != "тру" && identifier != "фейк") {
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
				if (function == "спросить")
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
			if (operands == "intint")
				argint[i] = pow(argint[i - 1], argint[i]);
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
	if (left != right and test == true)
		cout << "(конфликт типов) " << endl;
	return false;
}

string askstr(int var) {
	string value;
	//recognize (value)
	cin >> value;
	return value;
}
int askint(int var) {
	int value;
	//recognize (value)
	cin >> value;
	return value;
}
float askfloat(int var) {
	float value;
	//recognize (value)
	cin >> value;
	//for (symnum = 0; symnum < value.length() + 1; symnum++)
		//if (value[symnum] == '.')
			//value[symnum] = ',';
	return value;
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
string recognize(string literal) {
	if (literal == "тру" or literal == "фейк")
		return "bool";
	for (i = 1; i < vars + 1; i++)
		if (literal == varnames[i])
			return "var";
	switch (literal[0]){
		case '\'':
			if (literal[literal.length()] == '\'')
				for (symnum = 0; symnum < literal.length(); symnum++)
					if (literal[symnum] == '\'' or literal[symnum] == '\t' or literal[symnum] == '\n' or literal[symnum] == '\0' or literal[symnum] == '|')
						return "incorrect";			
			return "str";
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			for (symnum = 0; symnum < literal.length(); symnum++)
				switch (literal[symnum]) {
					case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
						break;
					default: return "incorrect";
				}
			return "int";
		default: return "incorrect";
	}	
	return "incorrect";
}
bool afterargs() {
	string end = "";
	for (symnum += 1; symnum < str.length(); symnum++)
		if (str[symnum] != ' ')
			end += str[symnum];
	if (end == ";" and function != "сравнивание")
		return false;
	if (end == "значит" and function == "вдруг")
		return false;
	return true;
}
void getargs(bool parenthesized, int min, int max) {
	start = false;
	finish = false;
	if (test == true)
		cout << endl << "аргументация:" << endl;
	for (symnum = (function == "assign" ? varnames[selectedvar].length() : function.length()); symnum < str.length() and errors[0] == "0"; symnum++) {//вынести аргументацию в отдельную функцию
		if (ops[argnum + 1] == '\0')
			ops[argnum + 1] = 's';
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
				break;
			case '!': case '=': case '<': case '>': case '~':
			case '{': case '}': case '"': case '&': case '?':
			case '-': case '+': case '*': case '^': case '/': case ':':
				ops[argnum + 1] = str[symnum];//-1
				ops[0] = 'T';
				if (test == true)
					cout << symnum << " - " << str[symnum] << " - действие задано" << endl;
				break;
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				readint(str[symnum] - '0'); break;
			case '(':
				if (test == true)
					cout << symnum << " - ( - начало аргументации" << endl;
				if (start == true)
					error("повторное начало аргументации");
				start = true;
				break;
			case ')':
				if (test == true)
					cout << symnum << " - ) - конец аргументации" << endl;
				if (afterargs() == true)
					error("не предусмотренное продолжение команды");
				finish = true;

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
		if (start == false)
			error("нет открывающей скобки");
		if (finish == false)
			error("нет закрывающей скобки");		
	}
	if (argnum == 0)
		error("аргументы отсутствуют");	
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

int main() {
	setlocale(LC_ALL, "");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	string file;
	//getline(cin, file);
	ifstream code("код.txt");		
	print[0] = "0";
	errors[0] = "0";
	while (getline(code, strget)) {
		//cout << strget << endl;
		strnum++;
		if (skip == true) {
			skip = false;
			continue;
		}
		//getline(cin, strget);		
		argint[0] = 0;
		argstr[0] = "";
		argnum = 0;
		symnum = 0;
		ops[0] = 'F';
		comparison = '\0';		
		function = "notset";
		str = "";

		if (test == true)
			cout << "удаление пробелов..." << endl;
		for (symnum = 0; symnum < strget.length(); symnum++) {
			for (i = 0; i < 119; i++)
				if (strget[symnum] == alphabet[i])
					break;
			//if (strget[symnum] == ' ' or strget[symnum] == '\0' or strget[symnum] == '\t')
			if (i < 119)
				break;
			strget.erase(symnum, 1);
			symnum--;
		}
		str = strget;
		if (test == true)
			cout << "обработанная строка: " << str << endl;

		if (str[0] == 'w') {
			str = "изрекаю";
			for (symnum = 1; symnum < strget.length(); symnum++)
				str += strget[symnum];
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
			//cout << output() << endl;
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
		if (str.rfind("вдруг", 0) == 0) {
			function = "вдруг";
			if (test == true)
				cout << endl << "функция: сравнивание" << endl;			
			getargs(true, 2, 0);
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
					skip = true;
				}
				else
					if (test == true)
						cout << "тру" << endl;	
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
		if (str.rfind("спросить", 0) == 0) {
			function = "спросить";
			getargs(false, 1, 1);
			for (i = 1; i < vars + 1; i++)
				if (args[1] == varnames[i])
					selectedvar = i;
			if (vartypes[selectedvar] == "str")
				varstr[selectedvar] = askstr(selectedvar);
			if (vartypes[selectedvar] == "int")
				varint[selectedvar] = askint(selectedvar);
			if (vartypes[selectedvar] == "float")
				varfloat[selectedvar] = askfloat(selectedvar);
			if (vartypes[selectedvar] == "bool")
				error("нельзя вводить двоичные данные");
		}
		if (function == "notset")
			error("функция не задана");	
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
		}
		clean();
	}	

	if (errors[0] == "0")
		for (i = 1; i < stoi(print[0]) + 1; i++)
			cout << print[i] << endl;
	else {
		cout << endl << endl << "ошибки: " << errors[0] << endl;
		for (i = 1; i < stoi(errors[0]) + 1; i++) {
			cout << "ошибка " << i << ": " << errors[i] << endl;
			errors[i] = "";
		}
		errors[0] = "0";
	}
	//fullclean();
}