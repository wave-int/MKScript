#include <iostream>
#include <string>
#include <locale>
#include <windows.h>
using namespace std;

int symnum, argnum, strnum = 0;
bool start, finish, error, text, comma;
int argint[9999];

string args[9999];
string argstr[9999];
string str, answer, name;


void readstr(){	
	if (comma == false)
		error = true;
	comma = false;
	text = true;
	argnum++;
	args[argnum] = "str";
	while (text == true) {
		cout << "readstr - " << text << " symnum: " << symnum << " - " << str[symnum] << endl;
		if (symnum == str.length()) {
			error = true;
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
void readint(int first) {
	if (comma == false)
		error = true;
	comma = false;
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
			case '0': break;
			default: 
				argint[argnum] /= 10;
				argint[0] = 0;
				break;			
		}
	}
	while (argint[0] == 1);
	symnum--;
	cout << "---------" << argint[argnum] << endl;
}


int main() {
	setlocale(LC_ALL, "");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	///////////////////////////////////////////////////////
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
		error = false;
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
						comma = false;
						switch (str[symnum + 1]){
							case '.': 
								//readfloat(0); 
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
								error = true; 
								break;
						}
						break;
					case ',':
						if (argnum > 0 && comma == false)
							comma = true;
						else
							error = true;
						break;	

					//for (int n = 0; n < 10; n++)
						//if (n == str[symnum])
							//readint(n);
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
					default: error = true; cout << "default : '" << str[symnum] << "' " << str.length() << " " << symnum << endl; break;
					}				
				
			}
		
			
			
			
			
			if (start == false or finish == false) 
				error = true;
			if (comma == true)
				error = 1;
			answer = "";
			if (error == false) {
				//=====================ANSWER 
				cout << endl << "args: " << args[0] << endl;
				args[0] = to_string(argnum);
				for (argnum = 0; argnum < stoi(args[0]) + 1; argnum++) {
					if (args[argnum] == "int")
						answer += to_string(argint[argnum]); 
					if (args[argnum] == "str")
						answer += argstr[argnum];
					if (args[argnum] == "zero")
						answer += '0';
					cout << "arg " << argnum << ": " << args[argnum] << endl;
						//default: answer += "NULL"; break;
				}				
			}
			else
				cout << "-----------------------------" << "В " << strnum << " строке обнаружена ошибка!" << endl;
			
			cout << "=============================" << answer << endl;
			///*
			cout << "comma: " << comma << endl;// << "аргументы:";
			cout << "start: " << start << endl;
			cout << "finish: " << finish << endl;
			cout << "arnum: " << argnum << endl;
			cout << "argint[0]" << argint[0] << endl;
			cout << "error: " << error << endl;
			//cout << "stoi(argnum[argint]): " << stoi(argnum[argint]) << endl;
			//cout << "args[0]: " << args[0] << endl;
			//*/



			//////////////////////////////////////////////////////////////// ЗАЧИСТКА
			args[0] = to_string(argnum);
			for (argnum = 0; argnum < stoi(args[0]) + 2; argnum++) {
				if (args[argnum] == "int")
					argint[argnum] = 0;
				if (args[argnum] == "str")
					argstr[argnum] = "";
			}
			argnum = 0;
			args[0] = "0";
	}
}