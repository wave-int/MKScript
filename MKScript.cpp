#include <iostream>
#include <string>
#include <cmath>
#include <locale>
#include <windows.h>
using namespace std;

bool autoround, autospace, test = false;
string strget, function, errors[1024], str; int i, symnum, strnum = 0;
int argnum; string result, args[1024]; char ops[1024]; bool start, finish;
string argstr[1024]; int argint[1024]; float argfloat[1024]; bool argbool[1024];
string standart[1]; string varnames[1024]; string vartypes[1024]; int vars = 0;
string varstr[1024]; int varint[1024]; float varfloat[1024]; bool varbool[1024];
string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";//118
string keywords[13] = {"str", "int", "float", "bool", "unknown", "строка", "число", "дробь", "булево", "тру", "фейк", "изрекаю", "обозначим"};
void error(string reason) {
	cout << " (ошибка) ";
	errors[0] = to_string(stoi(errors[0]) + 1);
	if (reason.length() == 2)
		reason += " - неожиданный символ";
	errors[stoi(errors[0])] = reason;
}
void readstr() {
	cout << symnum << " - ' - начало строки" << endl;
	symnum++;
	argnum++;
	args[argnum] = "str";
	if (str[symnum] != '\'')
		do {
			cout << symnum << " - " << str[symnum] << " - чтение строки..." << endl;
			argstr[argnum] = argstr[argnum] + str[symnum];
			symnum++;		
		} while (str[symnum] != '\'' and symnum < str.length());
	cout << symnum << " - ' - конец строки, " << argnum << " аргумент записан";
	if (argstr[argnum].length() == 0)
		cout << " (пустая строка)";
	cout << endl;
	//argstr[argnum].pop_back();
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
				cout << symnum << " - " << str[symnum] << " - чтение дробной части..." << endl; break;
			default: cout << symnum << " - " << str[symnum] << " - конец числа, " << argnum << " аргумент записан" << endl; break;
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
				cout << symnum - 1 << " - " << str[symnum - 1] << " - конец числа, " << argnum << " аргумент записан" << endl;
			symnum--;//+-
			break;
		default: cout << symnum << " - " << str[symnum] << " - число, " << argnum << " аргумент записан" << endl; break;
	}
	if (argnum > 1 and ops[argnum] == 's')//'n'
		if (notint == true)
			error("нет запятой перед " + to_string(argfloat[argnum]));
		else
			error("нет запятой перед " + to_string(argint[argnum]));
}
void addvar(string name, string type) {
	vars++;
	varnames[vars] = name;
	if (type == "строка") {
		vartypes[vars] = "str";
		varstr[vars] = "";
		cout << endl << "переменная " << name << " записана" << endl << endl;
	}		
	if (type == "число") {
		vartypes[vars] = "int";
		varint[vars] = 0;
		cout << endl << "переменная " << name << " записана" << endl << endl;
	}		
	if (type == "дробь") {
		vartypes[vars] = "float";
		varfloat[vars] = 0;
		cout << endl << "переменная " << name << " записана" << endl << endl;
	}		
	if (type == "булево") {
		vartypes[vars] = "bool";
		varbool[vars] = false;
		cout << endl << "переменная " << name << " записана" << endl << endl;
	}
	if (argnum > 2)
		error("функция обозначения переменной принимает только 2 аргумента (идентификатор, тип данных)");
}
void readunknown() {
	string word;
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
				cout << symnum << " - " << str[symnum] << " - чтение неизвестного идентификатора..." << endl;
			}
		if (text == true)
			word += str[symnum];
		else {
			for (i = 1; i < 8; i++)
				if (str[symnum] == valid[i])
					valid[0] == '1';
			if (valid[0] == '0')
				error("некорректная запись идентификатора");
		}
	} while (text == true);
	

	if (function == "обозначим") {
		if (argnum == 1)
			if (word != "тру" && word != "фейк") {
				for (i = 1; i < 10; i++)
					if (word == keywords[i])
						free = false;
				for (i = 1; i < vars + 1; i++)
					if (word == varnames[i])
						free = false;
				if (free == false)
					error("идентификатор зарезервирован");
				else
					args[1] = word;
			}
			else
				error("идентификатор зарезервирован");
		if (argnum == 2) {
			if (word == "строка" or word == "число" or word == "дробь" or word == "булево")
				args[2] = word;
			else
				error("неизвестный тип");
		}
		if (argnum > 2)
			error("неверный формат обозначения переменной");
	}
	else {
		if (word != "тру" && word != "фейк")
			for (i = 1; i < 10; i++)
				if (word == keywords[i])
					error("идентификатор зарезервирован");
		if (word == "тру") {
			cout << "значение выявлено (тру), аргумент записан" << endl;
			argbool[argnum] = true;
			args[argnum] = "bool";
		}
		if (word == "фейк") {
			cout << "значение выявлено (фейк), аргумент записан" << endl;
			argbool[argnum] = false;
			args[argnum] = "bool";
		}
		for (i = 1; i < vars; i++)
			if (word == varnames[i]) {
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
		error("неизвестное имя: " + word);
		argnum--;
		cout << "идентификатор \"" << word << "\" неизвестен, отмена записи аргумента" << endl;
	}
	symnum--;
	if (argnum > 1 and ops[argnum] == 's')
		error("нет запятой");
}
void perform() {
	string operands;
	int first = 1;
	bool firstable = false;
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
	do {
		for (i = 2; i < argnum + 1; i++) {
			operands = args[i - 1] + args[i];
			if (ops[i] == '*') {				
				if (operands == "strstr" or operands == "strint" or operands == "strfloat" or operands == "strbool")
					error("строку нельзя умножать");
				if (operands == "intstr" or operands == "floatstr" or operands == "boolstr")
					error("на строку нельзя умножать");
				if (operands == "boolstr" or operands == "boolint" or operands == "boolfloat" or operands == "boolbool")
					error("математические действия не применимы к двоичному значению");
				if (operands == "strbool" or operands == "intbool" or operands == "floatbool")
					error("математические действия не применимы к двоичному значению");
				if (operands == "intfloat" or operands == "floatint") {
					argfloat[i - 1] += argint[i - 1];
					argfloat[1] += argint[1];
					argfloat[i] *= argfloat[i - 1];
					args[i] = "float";
				}
				if (operands == "intint")
					argint[i] *= argint[i - 1];
				if (operands == "floatfloat")
					argfloat[i] *= argfloat[i - 1];
			}
			if (ops[i] == '/') {
				if (operands == "strstr" or operands == "strint" or operands == "strfloat" or operands == "strbool")
					error("строку нельзя делить");
				if (operands == "intstr" or operands == "floatstr" or operands == "boolstr")
					error("на строку нельзя делить");
				if (operands == "boolstr" or operands == "boolint" or operands == "boolfloat" or operands == "boolbool")
					error("математические действия не применимы к двоичному значению");
				if (operands == "strbool" or operands == "intbool" or operands == "floatbool")
					error("математические действия не применимы к двоичному значению");
				if (operands == "intfloat" or operands == "floatint") {
					argfloat[i - 1] += argint[i - 1];
					argfloat[i] += argint[i];
					if (argfloat[i] != 0)
						argfloat[i] = argfloat[i - 1] / argfloat[i];
					else
						error("на ноль делить нельзя");
					args[i] = "float";
				}
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
			case '-': {
				if (operands == "strstr" or operands == "strint" or operands == "strfloat" or operands == "strbool")
					error("из строки нельзя вычитать");
				if (operands == "intstr" or operands == "floatstr" or operands == "boolstr")
					error("нельзя вычесть строку");
				if (operands == "boolstr" or operands == "boolint" or operands == "boolfloat" or operands == "boolbool")// упорядочить
					error("математические действия не применимы к двоичному значению");
				if (operands == "strbool" or operands == "intbool" or operands == "floatbool")
					error("математические действия не применимы к двоичному значению");
				if (operands == "intfloat" or operands == "floatint") {
					argfloat[i - 1] += argint[i - 1];
					argfloat[1] += argint[1];
					argfloat[i] = argfloat[i - 1] - argfloat[i];
					args[i] = "float";
				}
				if (operands == "intint")
					argint[i] = argint[i - 1] - argint[i];
				if (operands == "floatfloat")
					argfloat[i] = argfloat[i - 1] - argfloat[i];
				break;
			}			
			case '+': {
				if (operands == "strbool" or operands == "intbool" or operands == "floatbool")
					error("математические действия не применимы к двоичному значению");
				if (operands == "boolstr" or operands == "boolint" or operands == "boolfloat" or operands == "boolbool")
					error("математические действия не применимы к двоичному значению");
				if (operands == "strstr")
					argstr[i] = argstr[i - 1] + argstr[i];
				if (operands == "strint")
					argstr[i] = argstr[i - 1] + to_string(argint[i]);
				if (operands == "strfloat")
					argstr[i] = argstr[i - 1] + to_string(argfloat[i]);
				if (operands == "strint" or operands == "strfloat")
					args[i] = "str";
				if (operands == "intstr")
					argstr[i] = to_string(argint[i - 1]) + argstr[i];
				if (operands == "intint")
					argint[i] += argint[i - 1];
				if (operands == "intfloat" or operands == "floatint") {
					argfloat[i - 1] += argint[i - 1];
					argfloat[1] += argint[1];
					argfloat[i] += argfloat[i - 1];
					args[i] = "float";
				}
				if (operands == "floatstr")
					argstr[i] = to_string(argfloat[i - 1]) + argstr[i];
				if (operands == "floatfloat")
					argfloat[i] += argfloat[i - 1];
				break;
			}
		case 's': break;
		case ',': break;
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
bool compare(){
	/*
		switch () {
			case '=': break;
			case '>': break;
			case '<': break;
			case '!': break;
		}
		*/
	return false;
}
string output() {
	string outstr = "";
	for (i = 1; i < argnum + 1; i++) {
		if (autospace == true && i > 1)
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
//гибкая функция зачистки
int main() {
	setlocale(LC_ALL, "");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	autoround = true;
	test = true;
	autospace = true;
	while (test == true) {
		strnum++;
		getline(cin, strget);
		start = false;
		finish = false;
		errors[0] = "0";
		argint[0] = 0;
		argstr[0] = "";
		argnum = 0;
		symnum = 0;
		ops[0] = 'F';
		result = "";
		function = "notset";
		str = "";
		if (strget[0] == ' ') {//с конца тоже удалять
			cout << "удаление пробелов..." << endl;
			do
				symnum++;
			while (symnum + 1 < strget.length() && strget[symnum + 1] == ' ');
			if (symnum == strget.length())
				error("строка пуста");
			else
				for (symnum+= 1; symnum < strget.length(); symnum++)
					str += strget[symnum];
			cout << "обработанная строка: " << str << endl;
		}
		else
			str = strget;

		if (str[str.length() - 1] != ';')
			str += ';';//УБРАТЬ
		if (str[str.length() - 1] != ';')
			error("неверное окончание строки");

		if (str.rfind("изрекаю", 0) == 0)
			function = "изрекаю";
		if (str.rfind("обозначим", 0) == 0)
			function = "обозначим";

		if (function == "notset")
			error("функция не задана");
		if (errors[0] == "0") {
			cout << endl << "функция: " << function << endl << endl << "аргументация:" << endl;
			for (symnum = function.length(); symnum < str.length() and errors[0] == "0"; symnum++) {//вынести аргументацию в отдельную функцию
				if (ops[argnum + 1] == '\0')
					ops[argnum + 1] = 's';
				switch (str[symnum]) {
				case ' ': cout << symnum << " -   - пробел" << endl; break;
				case ',':
					if (argnum == 0)
						error("перечисления не начинаются с запятой");
					if (ops[argnum + 1] == ',')
						error("лишняя запятая");
					ops[argnum + 1] = ',';
					cout << symnum << " - , - запятая" << endl;
					break;
				case ';': cout << symnum << " - ; - конец команды" << endl << endl; break;
				case '-': case '+': case '*': case '/':
					ops[argnum + 1] = str[symnum];
					ops[0] = 'T';
					cout << symnum << " - " << str[symnum] << " - действие задано" << endl;
					break;
				case '0': readint(0); break;//сократить
				case '1': readint(1); break;
				case '2': readint(2); break;
				case '3': readint(3); break;
				case '4': readint(4); break;
				case '5': readint(5); break;
				case '6': readint(6); break;
				case '7': readint(7); break;
				case '8': readint(8); break;
				case '9': readint(9); break;
				case '(':
					cout << symnum << " - ( - начало аргументации" << endl;
					if (start == true)
						error("повторное начало аргументации");
					start = true;
					break;
				case ')':
					cout << symnum << " - ) - конец аргументации" << endl;
					if (finish == true)
						error("повторный конец аргументации");
					finish = true;
					break;
				case '\'': readstr(); break;
				default:
					if (alphabet.find(str[symnum]) != alphabet.npos)
						readunknown();
					else
						error(to_string(str[symnum]));
					break;
				}
			}
			if (start == false)
				error("нет открывающей скобки");
			if (finish == false)
				error("нет закрывающей скобки");
			if (argnum == 0)
				error("аргументы отсутствуют");
			if (ops[0] == 'T')
				perform();
		}

		if (function == "изрекаю")
			cout << endl << "вывод: " << output() << endl << endl;
		if (function == "обозначим")
			addvar(args[1], args[2]);

		cout << "ошибки: " << errors[0] << endl;
		for (i = 1; i < stoi(errors[0]) + 1; i++) {
			cout << "ошибка " << i << ": " << errors[i] << endl;
			errors[i] = "";
		}
		errors[0] = "0";

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
		
		
		result = "";
	}
}
