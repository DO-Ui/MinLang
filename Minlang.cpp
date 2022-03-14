// #include <bits/stdc++.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

map<string, int> ints;
map<string, string> strings;
map<string, bool> bools;


int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "Please specify a file to run\n"; return -1;
	}
	ifstream file;
	file.open(argv[1]);

	if (!file.is_open()) {
		cerr << "File not found\n"; return -1;
	}
	if (file.bad()) {
		cerr << "Error reading file stream\n"; return -1;
	}
	
	string code;

	while (file) {
		string command;
		getline(file, command);
		// file >> command;
		code.append(command + "\n");
	}

	
	int current_char = 0;
	int loop_start = -1;
	int loop_end = -1;
	while (current_char < code.length()) {
		char command = code[current_char];
		if (command == ';' || command == ' ' || command == '}' || command == ']' || command == '\n') {
			current_char++;
			continue;
		}
		if (command == '#') {
			int skip = current_char;
			while (code[skip] != '\n') {
				skip++;
			}
			current_char = skip;
			continue;
		}
		if (command == '[') { // Print
			if (code[current_char+1] == '"') { // If litteral string
				//Find the ending " and print in between
				bool newline = true;
				int i = current_char+2;
				while (code[i] != '"') {
					i++;
				}
				if (code[i+1] == '$') {
					// i++;
					newline = false;
				}

				string out;
				for (int k = current_char+2; k < i; k++) {
					out += code[k];
				}
				cout << out;
				if (newline) {
					cout << "\n";
					min(current_char = i+1, (int) code.length());
				}
				else {
					min(current_char = i+2, (int) code.length());
				}
			}
			else {
				string var;
				bool newline = true;
				int i = current_char;
				while (code[i] != ']') {
					i++;
				}
				if (code[i-1] == '$') {
					i--;
					newline = false;
				}
				for (int k = current_char+1; k < i; k++) {
					var += code[k];
				}

				// cout << var << "\n";

				if (ints.count(var)) { // Var is type int
					cout << ints.find(var)->second;
				}
				else if (bools.count(var)) {
					cout << bools.find(var)->second;
				}
				else if (strings.count(var)) {
					cout << strings.find(var)->second;
				}
				if (newline) {
					cout << "\n";
					min(current_char = i+1, (int) code.length());
					continue;
				}
				else {
					min(current_char = i+2, (int) code.length());
					continue;
				}
			}
		}
		else if (command == '<') { // Input
			int i = current_char;
			string var;
			while (code[i] != '>') {
				i++;
			}
			for (int k = current_char+1; k < i; k++) {
				var += code[k];
			}
			
			if (ints.count(var)) { // Var is type int
				cin >> ints.find(var)->second;
			}
			else if (bools.count(var)) {
				cin >> bools.find(var)->second;
			}
			else if (strings.count(var)) {
				cin >> strings.find(var)->second;
			}
			min(current_char = i+1, (int) code.length());
			continue;
		}
		else if (command == '{') { // If statement
			string condition = "";
			int i = current_char;
			while (code[i] != '}') {
				i++;
			}
			for (int k = current_char+1; k < i; k++) {
				condition += code[k];
			}
			
			if (condition.find('=') != string::npos) {
				int l = (int) condition.find('=');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++) {
					condition1 += condition[k];
				}
				for (int k = l+2; k < condition.length(); k++) {
					condition2 += condition[k];
				}

				char * c1;
				char * c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);
				
				if (*c1 == 0 && *c2 == 0) {
					if (stoi(condition1) == stoi(condition2)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (condition1 == "true") {
					if (condition2 == condition1) {
						current_char = i+1;
					} else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (condition1 == "false") {
					if (condition2 == condition1) {
						current_char = i+1;
					} else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (condition1[0] == '"') {
					if (condition2 == condition1) {
						current_char = i+1;
					} else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0) {
					if (ints.find(condition1)->second == stoi(condition2)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0) {
					if (ints.find(condition2)->second == stoi(condition1)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (strings.count(condition1) && condition2[0] == '"') {
					condition2.erase(0,1);
					condition2.erase(condition2.size()-1,1);
					if (strings.find(condition1)->second == condition2) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (strings.count(condition2) && condition1[0] == '"') {
					condition1.erase(0,1);
					condition1.erase(condition1.size()-1,1);
					if (strings.find(condition2)->second == condition1) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (bools.count(condition1) && (condition2 == "true" || condition2 == "false")) {
					if (bools.find(condition1)->second == true && condition2 == "true") {
						current_char = i+1;
					}
					else if (bools.find(condition1)->second == false && condition2 == "false") {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (bools.count(condition2) && (condition1 == "true" || condition1 == "false")) {
					if (bools.find(condition2)->second == true && condition1 == "true") {
						current_char = i+1;
					}
					else if (bools.find(condition2)->second == false && condition1 == "false") {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2)) {
					if (ints.find(condition1)->second == ints.find(condition2)->second) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (bools.count(condition1) && bools.count(condition2)) {
					if (bools.find(condition1)->second == bools.find(condition2)->second) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (strings.count(condition1) && strings.count(condition2)) {
					if (strings.find(condition1)->second == strings.find(condition2)->second) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}

			}
			else if (condition.find('>') != string::npos) {
				int l = (int) condition.find('>');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++) {
					condition1 += condition[k];
				}
				for (int k = l+1; k < condition.length(); k++) {
					condition2 += condition[k];
				}

				char * c1;
				char * c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);
				
				

				if (*c1 == 0 && *c2 == 0) {
					if (stoi(condition1) > stoi(condition2)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0) {
					if (ints.find(condition1)->second > stoi(condition2)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0) {
					if (ints.find(condition2)->second > stoi(condition1)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2)) {
					if (ints.find(condition1)->second > ints.find(condition2)->second) {
						current_char=i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
			}
			else if (condition.find('<') != string::npos) {
				int l = (int) condition.find('<');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++) {
					condition1 += condition[k];
				}
				for (int k = l+1; k < condition.length(); k++) {
					condition2 += condition[k];
				}

				char * c1;
				char * c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);
				
				if (*c1 == 0 && *c2 == 0) {
					if (stoi(condition1) < stoi(condition2)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0) {
					if (ints.find(condition1)->second < stoi(condition2)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0) {
					if (ints.find(condition2)->second < stoi(condition1)) {
						current_char = i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2)) {
					if (ints.find(condition1)->second < ints.find(condition2)->second) {
						current_char=i+1;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}

			}

		}
		else if (command == '^') {
			string condition = "";
			loop_start = current_char;
			loop_end = current_char;
			while (code[loop_end] != '\n') {
				loop_end++;
			}
			int i = current_char+1;
			while (code[i] != '}') {
				i++;
			}
			for (int k = current_char+2; k < i; k++) {
				condition += code[k];
			}
			
			if (condition.find('=') != string::npos) {
				int l = (int) condition.find('=');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++) {
					condition1 += condition[k];
				}
				for (int k = l+2; k < condition.length(); k++) {
					condition2 += condition[k];
				}


				char * c1;
				char * c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);
				
				if (*c1 == 0 && *c2 == 0) {
					if (stoi(condition1) == stoi(condition2)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (condition1 == "true") {
					if (condition2 == condition1) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					} else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (condition1 == "false") {
					if (condition2 == condition1) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					} else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (condition1[0] == '"') {
					if (condition2 == condition1) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					} else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0) {
					if (ints.find(condition1)->second == stoi(condition2)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0) {
					if (ints.find(condition2)->second == stoi(condition1)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (strings.count(condition1) && condition2[0] == '"') {
					condition2.erase(0,1);
					condition2.erase(condition2.size()-1,1);
					if (strings.find(condition1)->second == condition2) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (strings.count(condition2) && condition1[0] == '"') {
					condition1.erase(0,1);
					condition1.erase(condition1.size()-1,1);
					if (strings.find(condition2)->second == condition1) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (bools.count(condition1) && (condition2 == "true" || condition2 == "false")) {
					if (bools.find(condition1)->second == true && condition2 == "true") {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else if (bools.find(condition1)->second == false && condition2 == "false") {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (bools.count(condition2) && (condition1 == "true" || condition1 == "false")) {
					if (bools.find(condition2)->second == true && condition1 == "true") {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else if (bools.find(condition2)->second == false && condition1 == "false") {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2)) {
					if (ints.find(condition1)->second == ints.find(condition2)->second) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (bools.count(condition1) && bools.count(condition2)) {
					if (bools.find(condition1)->second == bools.find(condition2)->second) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}
				else if (strings.count(condition1) && strings.count(condition2)) {
					if (strings.find(condition1)->second == strings.find(condition2)->second) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != ';') {
							i++;
						}
						current_char = i;
					}
				}

			}
			else if (condition.find('>') != string::npos) {
				int l = (int) condition.find('>');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++) {
					condition1 += condition[k];
				}
				for (int k = l+1; k < condition.length(); k++) {
					condition2 += condition[k];
				}

				char * c1;
				char * c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);
				
				

				if (*c1 == 0 && *c2 == 0) {
					if (stoi(condition1) > stoi(condition2)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0) {
					if (ints.find(condition1)->second > stoi(condition2)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0) {
					if (ints.find(condition2)->second > stoi(condition1)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
			}
			else if (condition.find('<') != string::npos) {
				int l = (int) condition.find('<');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++) {
					condition1 += condition[k];
				}
				for (int k = l+1; k < condition.length(); k++) {
					condition2 += condition[k];
				}

				char * c1;
				char * c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);
				
				if (*c1 == 0 && *c2 == 0) {
					if (stoi(condition1) < stoi(condition2)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0) {
					if (ints.find(condition1)->second < stoi(condition2)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0) {
					if (ints.find(condition2)->second < stoi(condition1)) {
						current_char = i+1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end-loop_start)));
						// loop = true;
					}
					else {
						while (code[i] != '\n') {
							i++;
						}
						current_char = i;
					}
				}

			}

			
		}
		else if (code[current_char] != ';' || code[current_char] != ' ' || code[current_char] != '}' || code[current_char] != ']' || code[current_char] != '\n'){ // Variable check
			string var;
			int i = current_char;
			while (code[i] != '=') { // Loop until an operator
				i++;
			}
			for (int k = current_char; k < i; k++) {
				var += code[k];
			}
			char op;
			op = code[i];
			if (op == '=') { // Assignment
				string value;
				int j = i;
				int removed_value;
				while (code[j] != ';') {
					j++;
				}
				for (int k = i+1; k < j; k++) {
					value += code[k];
				}
				
				if (ints.count(var)) { // Already exists
					removed_value = ints.find(var)->second;
					ints.erase(var);
				}
				else if (strings.count(var)) {
					strings.erase(var);
				}
				else if (bools.count(var)) {
					bools.erase(var);
				}

				char * p;
				strtol(value.c_str(), &p, 10);
				if (*p == 0) { // Integer
					int v = stoi(value);
					ints.insert({var, v});
				}
				else if (value == "true") { // Boolean
					bools.insert({var, true});
				}
				else if (value == "false") { // Boolean
					bools.insert({var, false});
				}
				else if (value[0] == '"'){ // String
					value.erase(0,1);
					value.erase(value.size()-1, 1);
					strings.insert({var, value});
				}
				else {
					if (value.find('-') != string::npos) {
						int l = (int) value.find('-');
						string a;
						for (int k = i+1; k < i+l+1; k++) {
							a += code[k];
						}
						string b;
						for (int k = i+l+2; k < j; k++) {
							b += code[k];
						}
						char * z;
						strtol(b.c_str(), &z, 10);
						char * y;
						strtol(a.c_str(), &y, 10);
						if (*z == 0 && ints.count(a)) {
							ints.insert({var, ints.find(a)->second - stoi(b)});
						}
						else if (*y == 0 && ints.count(b)) {
							ints.insert({var, stoi(a) - ints.find(b)->second});
						}
						else if (*z == 0 && *y == 0) {
							ints.insert({var, stoi(a) - stoi(b)});
						}
						else {
							if (a == var && ints.count(b)) {
								ints.insert({var, removed_value - ints.find(b)->second});
							}
							else if (b == var && ints.count(a)) {
								ints.insert({var, ints.find(a)->second - removed_value});	
							}
							else if (a == var && *z == 0) {
								ints.insert({var, removed_value - stoi(b)});
							}
							else if (b == var && *y == 0) {
								ints.insert({var, stoi(a) - removed_value});
							}
							else {
								ints.insert({var, ints.find(a)->second - ints.find(b)->second});	
							}
						}
					}

					else if (value.find('+') != string::npos) {
						int l = (int) value.find('+');
						string a;
						for (int k = i+1; k < i+l+1; k++) {
							a += code[k];
						}
						string b;
						for (int k = i+l+2; k < j; k++) {
							b += code[k];
						}
						char * z;
						strtol(b.c_str(), &z, 10);
						char * y;
						strtol(a.c_str(), &y, 10);
						if (*z == 0 && ints.count(a)) {
							ints.insert({var, ints.find(a)->second + stoi(b)});
						}
						else if (*y == 0 && ints.count(b)) {
							ints.insert({var, stoi(a) + ints.find(b)->second});
						}
						else if (*z == 0 && *y == 0) {
							ints.insert({var, stoi(a) + stoi(b)});
						}
						else {
							if (a == var && ints.count(b)) {
								ints.insert({var, removed_value + ints.find(b)->second});
							}
							else if (b == var && ints.count(a)) {
								ints.insert({var, ints.find(a)->second + removed_value});	
							}
							else if (a == var && *z == 0) {
								ints.insert({var, removed_value + stoi(b)});
							}
							else if (b == var && *y == 0) {
								ints.insert({var, stoi(a) + removed_value});
							}
							else {
								ints.insert({var, ints.find(a)->second + ints.find(b)->second});	
							}
						}
					}
					
					else if (value.find('*') != string::npos) {
						int l = (int) value.find('*');
						string a;
						for (int k = i+1; k < i+l+1; k++) {
							a += code[k];
						}
						string b;
						for (int k = i+l+2; k < j; k++) {
							b += code[k];
						}
						char * z;
						strtol(b.c_str(), &z, 10);
						char * y;
						strtol(a.c_str(), &y, 10);
						if (*z == 0 && ints.count(a)) {
							ints.insert({var, ints.find(a)->second * stoi(b)});
						}
						else if (*y == 0 && ints.count(b)) {
							ints.insert({var, stoi(a) * ints.find(b)->second});
						}
						else if (*z == 0 && *y == 0) {
							ints.insert({var, stoi(a) * stoi(b)});
						}
						else {
							if (a == var && ints.count(b)) {
								ints.insert({var, removed_value * ints.find(b)->second});
							}
							else if (b == var && ints.count(a)) {
								ints.insert({var, ints.find(a)->second * removed_value});	
							}
							else if (a == var && *z == 0) {
								ints.insert({var, removed_value * stoi(b)});
							}
							else if (b == var && *y == 0) {
								ints.insert({var, stoi(a) * removed_value});
							}
							else {
								ints.insert({var, ints.find(a)->second * ints.find(b)->second});	
							}
						}
					}
				
					else if (value.find('/') != string::npos) {
						int l = (int) value.find('/');
						string a;
						for (int k = i+1; k < i+l+1; k++) {
							a += code[k];
						}
						string b;
						for (int k = i+l+2; k < j; k++) {
							b += code[k];
						}
						char * z;
						strtol(b.c_str(), &z, 10);
						char * y;
						strtol(a.c_str(), &y, 10);
						if (*z == 0 && ints.count(a)) {
							ints.insert({var, ints.find(a)->second / stoi(b)});
						}
						else if (*y == 0 && ints.count(b)) {
							ints.insert({var, stoi(a) / ints.find(b)->second});
						}
						else if (*z == 0 && *y == 0) {
							ints.insert({var, stoi(a) / stoi(b)});
						}
						else {
							if (a == var && ints.count(b)) {
								ints.insert({var, removed_value / ints.find(b)->second});
							}
							else if (b == var && ints.count(a)) {
								ints.insert({var, ints.find(a)->second / removed_value});	
							}
							else if (a == var && *z == 0) {
								ints.insert({var, removed_value / stoi(b)});
							}
							else if (b == var && *y == 0) {
								ints.insert({var, stoi(a) / removed_value});
							}
							else {
								ints.insert({var, ints.find(a)->second / ints.find(b)->second});	
							}
						}
					}
					
				}

				current_char = j; // Skip forward
			}
		}
		current_char++;
		
	}

}
