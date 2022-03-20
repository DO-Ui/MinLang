#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <list>

using std::cerr;
using std::cin;
using std::cout;
using std::ifstream;
using std::list;
using std::min;
using std::string;
using std::strtol;
using std::unordered_map;

unordered_map<string, int> ints;
unordered_map<string, string> strings;
unordered_map<string, bool> bools;

unordered_map<string, list<int>> int_arrs;
unordered_map<string, list<string>> string_arrs;
unordered_map<string, list<bool>> bool_arrs;

int main(int argc, char **argv)
{
	string flag = argv[1];
	if (flag == "--help" || flag == "-h" || argc < 2)
	{
		cout << "Minlang v1.1 ( https://github.com/DO-Ui/MinLang )\n";
		cout << "Usage: Minlang [file]\n";
		cout << "-h / --help    Show this help message and exit\n";
		cout << "-v / --version Show the current version of Minlang\n";
		return 0;
	}

	if (flag == "-v" || flag == "--version")
	{
		cout << "Minlang version 1.1 ( https://github.com/DO-Ui/MinLang )\n";
		return 0;
	}

	ifstream file;
	file.open(argv[1]);

	if (!file.is_open())
	{
		cerr << "File not found\n";
		return 0;
	}
	if (file.bad())
	{
		cerr << "Error reading file stream\n";
		return 0;
	}

	string code;

	while (file)
	{
		string command = "";
		getline(file, command);
		code.append(command + "\n");
	}

	int current_char = 0;
	int loop_start = -1;
	int loop_end = -1;
	while (current_char < code.length())
	{
		char command = code[current_char];
		if (command == ';' || command == ' ' || command == '}' || command == ']' || command == '\n')
		{
			current_char++;
			continue;
		}
		if (command == '#')
		{
			int skip = current_char;
			while (code[skip] != '\n')
				skip++;
			current_char = skip;
			continue;
		}
		if (command == '[')
		{ // Print
			if (code[current_char + 1] == '"')
			{ // If literal string
				// Find the ending " and print in between
				bool newline = true;
				int i = current_char + 2;
				while (code[i] != '"')
					i++;
				if (code[i + 1] == '$')
					newline = false;
				string out;
				for (int k = current_char + 2; k < i; k++)
					out += code[k];
				cout << out;
				if (newline)
				{
					cout << "\n";
					current_char = min(i + 1, (int)code.length());
				}
				else
					current_char = min(i + 2, (int)code.length());
			}
			else
			{
				string var;
				bool newline = true;
				int i = current_char;
				while (code[i] != ']')
					i++;
				if (code[i - 1] == '$')
				{
					i--;
					newline = false;
				}
				for (int k = current_char + 1; k < i; k++)
					var += code[k];
				if (var.find('(') != string::npos)
				{ // Printing array
					int l = (int)var.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
					{
						arr += var[k];
					}
					string index = "";
					l++;
					while (var[l] != ')')
					{
						index += var[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
					{
						real_index = ints.find(index)->second;
					}
					else
					{
						real_index = stoi(index);
					}

					if (int_arrs.count(arr))
					{
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						cout << *it;
					}
					else if (string_arrs.count(arr))
					{
						list<string> a = string_arrs.find(arr)->second;
						list<string>::iterator it = a.begin();
						advance(it, real_index);
						cout << *it;
					}
					else if (bool_arrs.count(arr))
					{
						list<bool> a = bool_arrs.find(arr)->second;
						list<bool>::iterator it = a.begin();
						advance(it, real_index);
						cout << *it;
					}
				}
				else
				{
					if (ints.count(var))
						cout << ints.find(var)->second;
					else if (bools.count(var))
						cout << bools.find(var)->second;
					else if (strings.count(var))
						cout << strings.find(var)->second;
				}
				if (newline)
				{
					cout << "\n";
					current_char = min(i + 1, (int)code.length());
					continue;
				}
				else
				{
					current_char = min(i + 2, (int)code.length());
					continue;
				}
			}
		}
		else if (command == '<')
		{ // Input
			int i = current_char;
			string var = "";
			while (code[i] != '>')
				i++;
			for (int k = current_char + 1; k < i; k++)
				var += code[k];
			if (var.find('(') != string::npos)
			{
				int l = (int)var.find('(');
				string arr = "";
				for (int k = 0; k < l; k++)
				{
					arr += var[k];
				}
				string index = "";
				l++;
				while (var[l] != ')')
				{
					index += var[l];
					l++;
				}

				int real_index;
				if (ints.count(index))
				{
					real_index = ints.find(index)->second;
				}
				else
				{
					real_index = stoi(index);
				}

				if (int_arrs.count(arr))
				{
					list<int> a = int_arrs.find(arr)->second;
					auto it = a.begin();
					advance(it, real_index);
					int ele;
					cin >> ele;

					a.insert(it, ele);
					prev(it, 1);
					a.erase(it);
					int_arrs.erase(arr);
					int_arrs.insert({arr, a});
				}
				else if (string_arrs.count(arr))
				{
					list<string> a = string_arrs.find(arr)->second;
					auto it = a.begin();
					advance(it, real_index);
					string ele;
					cin >> ele;

					a.insert(it, ele);
					prev(it, 1);
					a.erase(it);
					string_arrs.erase(arr);
					string_arrs.insert({arr, a});
				}
				else if (bool_arrs.count(arr))
				{
					list<bool> a = bool_arrs.find(arr)->second;
					auto it = a.begin();
					advance(it, real_index);
					string ele;
					cin >> ele;

					if (ele[0] == 't')
					{
						a.insert(it, true);
					}
					else
					{
						a.insert(it, false);
					}
					prev(it, 1);
					a.erase(it);
					bool_arrs.erase(arr);
					bool_arrs.insert({arr, a});
				}
			}

			if (ints.count(var))
				cin >> ints.find(var)->second;
			else if (bools.count(var))
				cin >> bools.find(var)->second;
			else if (strings.count(var))
				cin >> strings.find(var)->second;
			current_char = min(i + 1, (int)code.length());
			continue;
		}
		else if (command == '{')
		{ // If statement
			string condition = "";
			int i = current_char;
			while (code[i] != '}')
				i++;
			for (int k = current_char + 1; k < i; k++)
				condition += code[k];

			if (condition.find('=') != string::npos)
			{
				int l = (int)condition.find('=');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++)
					condition1 += condition[k];
				for (int k = l + 2; k < condition.length(); k++)
					condition2 += condition[k];
				char *c1;
				char *c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);

				if (*c1 == 0 && *c2 == 0)
				{
					if (stoi(condition1) == stoi(condition2))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (condition1 == "true" && (condition2 == "true" || condition2 == "false"))
				{
					if (condition2 == condition1)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (condition1 == "false" && (condition2 == "true" || condition2 == "false"))
				{
					if (condition2 == condition1)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (condition1[0] == '"' && condition2[0] == '"')
				{
					if (condition2 == condition1)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0)
				{
					if (ints.find(condition1)->second == stoi(condition2))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0)
				{
					if (ints.find(condition2)->second == stoi(condition1))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (strings.count(condition1) && condition2[0] == '"')
				{
					condition2.erase(0, 1);
					condition2.erase(condition2.size() - 1, 1);
					if (strings.find(condition1)->second == condition2)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (strings.count(condition2) && condition1[0] == '"')
				{
					condition1.erase(0, 1);
					condition1.erase(condition1.size() - 1, 1);
					if (strings.find(condition2)->second == condition1)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (bools.count(condition1) && (condition2 == "true" || condition2 == "false"))
				{
					if (bools.find(condition1)->second && condition2 == "true")
						current_char = i + 1;
					else if (!bools.find(condition1)->second && condition2 == "false")
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (bools.count(condition2) && (condition1 == "true" || condition1 == "false"))
				{
					if (bools.find(condition2)->second && condition1 == "true")
						current_char = i + 1;
					else if (!bools.find(condition2)->second && condition1 == "false")
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2))
				{
					if (ints.find(condition1)->second == ints.find(condition2)->second)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (bools.count(condition1) && bools.count(condition2))
				{
					if (bools.find(condition1)->second == bools.find(condition2)->second)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (strings.count(condition1) && strings.count(condition2))
				{
					if (strings.find(condition1)->second == strings.find(condition2)->second)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (condition1.find('(') != string::npos)
				{
					int l = (int)condition1.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition1[k];

					string index = "";
					l++;
					while (condition1[l] != ')')
					{
						index += condition1[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if second condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c2 == 0)
						{
							if (*it == stoi(condition2))
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition2))
						{
							if (*it == ints.find(condition2)->second)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition2.find('(') != string::npos)
						{
							int l1 = (int)condition2.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition2[k];

							string index1 = "";
							l1++;
							while (condition2[l1] != ')')
							{
								index += condition2[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it == *it1)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
					else if (string_arrs.count(arr))
					{
						// if second condition is string
						list<string> a = string_arrs.find(arr)->second;
						list<string>::iterator it = a.begin();
						advance(it, real_index);
						if (condition2[0] == '"')
						{
							condition2.erase(0, 1);
							condition2.erase(condition2.size() - 1, 1);
							if (*it == condition2)
								current_char = i + 1;
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
					else if (bool_arrs.count(arr) && (condition2 == "true" || condition2 == "false"))
					{
						// if second condition is bool
						list<bool> a = bool_arrs.find(arr)->second;
						list<bool>::iterator it = a.begin();
						advance(it, real_index);
						if (*it && condition2 == "true")
							current_char = i + 1;
						else if (!(*it) && condition2 == "false")
							current_char = i + 1;
						else
						{
							while (code[i] != '\n')
								i++;
							current_char = i;
						}
					}
				}
				else if (condition2.find('(') != string::npos)
				{
					int l = (int)condition2.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition2[k];

					string index = "";
					l++;
					while (condition2[l] != ')')
					{
						index += condition2[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if first condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c1 == 0)
						{
							if (*it == stoi(condition1))
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition1))
						{
							if (*it == ints.find(condition1)->second)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition1.find('(') != string::npos)
						{
							int l1 = (int)condition1.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition1[k];

							string index1 = "";
							l1++;
							while (condition1[l1] != ')')
							{
								index += condition1[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it == *it1)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
					else if (string_arrs.count(arr))
					{
						// if first condition is string
						list<string> a = string_arrs.find(arr)->second;
						list<string>::iterator it = a.begin();
						advance(it, real_index);
						if (condition1[0] == '"')
						{
							condition1.erase(0, 1);
							condition1.erase(condition1.size() - 1, 1);
							if (*it == condition1)
								current_char = i + 1;
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
					else if (bool_arrs.count(arr) && (condition1 == "true" || condition1 == "false"))
					{
						// if first condition is bool
						list<bool> a = bool_arrs.find(arr)->second;
						list<bool>::iterator it = a.begin();
						advance(it, real_index);
						if (*it && condition1 == "true")
							current_char = i + 1;
						else if (!(*it) && condition1 == "false")
							current_char = i + 1;
						else
						{
							while (code[i] != '\n')
								i++;
							current_char = i;
						}
					}
				}
			}

			else if (condition.find('>') != string::npos)
			{
				int l = (int)condition.find('>');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++)
					condition1 += condition[k];
				for (int k = l + 1; k < condition.length(); k++)
					condition2 += condition[k];

				char *c1;
				char *c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);

				if (*c1 == 0 && *c2 == 0)
				{
					if (stoi(condition1) > stoi(condition2))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0)
				{
					if (ints.find(condition1)->second > stoi(condition2))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0)
				{
					if (ints.find(condition2)->second > stoi(condition1))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2))
				{
					if (ints.find(condition1)->second > ints.find(condition2)->second)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}

				else if (condition1.find('(') != string::npos)
				{
					int l = (int)condition1.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition1[k];

					string index = "";
					l++;
					while (condition1[l] != ')')
					{
						index += condition1[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if second condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c2 == 0)
						{
							if (*it > stoi(condition2))
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition2))
						{
							if (*it > ints.find(condition2)->second)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition2.find('(') != string::npos)
						{
							int l1 = (int)condition2.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition2[k];

							string index1 = "";
							l1++;
							while (condition2[l1] != ')')
							{
								index += condition2[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it > *it1)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
				}
				else if (condition2.find('(') != string::npos)
				{
					int l = (int)condition2.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition2[k];

					string index = "";
					l++;
					while (condition2[l] != ')')
					{
						index += condition2[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if first condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c1 == 0)
						{
							if (stoi(condition1) > *it)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition1))
						{
							if (ints.find(condition1)->second > *it)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition1.find('(') != string::npos)
						{
							int l1 = (int)condition1.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition1[k];

							string index1 = "";
							l1++;
							while (condition1[l1] != ')')
							{
								index += condition1[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it1 > *it)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
				}
			
			}
			else if (condition.find('<') != string::npos)
			{
				int l = (int)condition.find('<');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++)
					condition1 += condition[k];
				for (int k = l + 1; k < condition.length(); k++)
					condition2 += condition[k];
				char *c1;
				char *c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);

				if (*c1 == 0 && *c2 == 0)
				{
					if (stoi(condition1) < stoi(condition2))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0)
				{
					if (ints.find(condition1)->second < stoi(condition2))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0)
				{
					if (ints.find(condition2)->second < stoi(condition1))
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2))
				{
					if (ints.find(condition1)->second < ints.find(condition2)->second)
						current_char = i + 1;
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}

				// thing


				else if (condition1.find('(') != string::npos)
				{
					int l = (int)condition1.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition1[k];

					string index = "";
					l++;
					while (condition1[l] != ')')
					{
						index += condition1[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if second condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c2 == 0)
						{
							if (*it < stoi(condition2))
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition2))
						{
							if (*it < ints.find(condition2)->second)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition2.find('(') != string::npos)
						{
							int l1 = (int)condition2.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition2[k];

							string index1 = "";
							l1++;
							while (condition2[l1] != ')')
							{
								index += condition2[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it < *it1)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
				}
				else if (condition2.find('(') != string::npos)
				{
					int l = (int)condition2.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition2[k];

					string index = "";
					l++;
					while (condition2[l] != ')')
					{
						index += condition2[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if first condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c1 == 0)
						{
							if (stoi(condition1) < *it)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition1))
						{
							if (ints.find(condition1)->second < *it)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition1.find('(') != string::npos)
						{
							int l1 = (int)condition1.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition1[k];

							string index1 = "";
							l1++;
							while (condition1[l1] != ')')
							{
								index += condition1[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it1 < *it)
							{
								current_char = i + 1;
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
				}
			
			}
		}
		else if (command == '^')
		{
			string condition = "";
			loop_start = current_char;
			loop_end = current_char;
			while (code[loop_end] != '\n')
				loop_end++;
			int i = current_char + 1;
			while (code[i] != '}')
				i++;
			for (int k = current_char + 2; k < i; k++)
				condition += code[k];
			if (condition.find('=') != string::npos)
			{
				int l = (int)condition.find('=');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++)
					condition1 += condition[k];
				for (int k = l + 2; k < condition.length(); k++)
					condition2 += condition[k];

				char *c1;
				char *c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);

				if (*c1 == 0 && *c2 == 0)
				{
					if (stoi(condition1) == stoi(condition2))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (condition1 == "true" && (condition2 == "true" || condition2 == "false"))
				{
					if (condition2 == condition1)
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (condition1 == "false" && (condition2 == "true" || condition2 == "false"))
				{
					if (condition2 == condition1)
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (condition1[0] == '"' && condition2[0] == '"')
				{
					if (condition2 == condition1)
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0)
				{
					if (ints.find(condition1)->second == stoi(condition2))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0)
				{
					if (ints.find(condition2)->second == stoi(condition1))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (strings.count(condition1) && condition2[0] == '"')
				{
					condition2.erase(0, 1);
					condition2.erase(condition2.size() - 1, 1);
					if (strings.find(condition1)->second == condition2)
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (strings.count(condition2) && condition1[0] == '"')
				{
					condition1.erase(0, 1);
					condition1.erase(condition1.size() - 1, 1);
					if (strings.find(condition2)->second == condition1)
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (bools.count(condition1) && (condition2 == "true" || condition2 == "false"))
				{
					if (bools.find(condition1)->second && condition2 == "true")
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else if (!bools.find(condition1)->second && condition2 == "false")
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (bools.count(condition2) && (condition1 == "true" || condition1 == "false"))
				{
					if (bools.find(condition2)->second && condition1 == "true")
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else if (!bools.find(condition2)->second && condition1 == "false")
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2))
				{
					if (ints.find(condition1)->second == ints.find(condition2)->second)
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (bools.count(condition1) && bools.count(condition2))
				{
					if (bools.find(condition1)->second == bools.find(condition2)->second)
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				else if (strings.count(condition1) && strings.count(condition2))
				{
					if (strings.find(condition1)->second == strings.find(condition2)->second)
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != ';')
							i++;
						current_char = i;
					}
				}
				// thing

				else if (condition1.find('(') != string::npos)
				{
					int l = (int)condition1.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition1[k];

					string index = "";
					l++;
					while (condition1[l] != ')')
					{
						index += condition1[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if second condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c2 == 0)
						{
							if (*it == stoi(condition2))
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition2))
						{
							if (*it == ints.find(condition2)->second)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition2.find('(') != string::npos)
						{
							int l1 = (int)condition2.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition2[k];

							string index1 = "";
							l1++;
							while (condition2[l1] != ')')
							{
								index += condition2[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it == *it1)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
					else if (string_arrs.count(arr))
					{
						// if second condition is string
						list<string> a = string_arrs.find(arr)->second;
						list<string>::iterator it = a.begin();
						advance(it, real_index);
						if (condition2[0] == '"')
						{
							condition2.erase(0, 1);
							condition2.erase(condition2.size() - 1, 1);
							if (*it == condition2) {
								current_char = i + 1;
							code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
					else if (bool_arrs.count(arr) && (condition2 == "true" || condition2 == "false"))
					{
						// if second condition is bool
						list<bool> a = bool_arrs.find(arr)->second;
						list<bool>::iterator it = a.begin();
						advance(it, real_index);
						if (*it && condition2 == "true") {
							current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
						}
						else if (!(*it) && condition2 == "false") {
							current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
						}
						else
						{
							while (code[i] != '\n')
								i++;
							current_char = i;
						}
					}
				}
				else if (condition2.find('(') != string::npos)
				{
					int l = (int)condition2.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition2[k];

					string index = "";
					l++;
					while (condition2[l] != ')')
					{
						index += condition2[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if first condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c1 == 0)
						{
							if (*it == stoi(condition1))
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition1))
						{
							if (*it == ints.find(condition1)->second)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition1.find('(') != string::npos)
						{
							int l1 = (int)condition1.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition1[k];

							string index1 = "";
							l1++;
							while (condition1[l1] != ')')
							{
								index += condition1[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it == *it1)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
					else if (string_arrs.count(arr))
					{
						// if first condition is string
						list<string> a = string_arrs.find(arr)->second;
						list<string>::iterator it = a.begin();
						advance(it, real_index);
						if (condition1[0] == '"')
						{
							condition1.erase(0, 1);
							condition1.erase(condition1.size() - 1, 1);
							if (*it == condition1) {
								current_char = i + 1;
							code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));

							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
					else if (bool_arrs.count(arr) && (condition1 == "true" || condition1 == "false"))
					{
						// if first condition is bool
						list<bool> a = bool_arrs.find(arr)->second;
						list<bool>::iterator it = a.begin();
						advance(it, real_index);
						if (*it && condition1 == "true") {
							current_char = i + 1;
							code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
						}
						else if (!(*it) && condition1 == "false") {
							current_char = i + 1;
							code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
						}
						else
						{
							while (code[i] != '\n')
								i++;
							current_char = i;
						}
					}
				}
			}
			else if (condition.find('>') != string::npos)
			{
				int l = (int)condition.find('>');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++)
					condition1 += condition[k];
				for (int k = l + 1; k < condition.length(); k++)
					condition2 += condition[k];

				char *c1;
				char *c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);

				if (*c1 == 0 && *c2 == 0)
				{
					if (stoi(condition1) > stoi(condition2))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0)
				{
					if (ints.find(condition1)->second > stoi(condition2))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0)
				{
					if (ints.find(condition2)->second > stoi(condition1))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2))
				{
					if (ints.find(condition1)->second > ints.find(condition2)->second){
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				// thing

				else if (condition1.find('(') != string::npos)
				{
					int l = (int)condition1.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition1[k];

					string index = "";
					l++;
					while (condition1[l] != ')')
					{
						index += condition1[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if second condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c2 == 0)
						{
							if (*it > stoi(condition2))
							{
								current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition2))
						{
							if (*it > ints.find(condition2)->second)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition2.find('(') != string::npos)
						{
							int l1 = (int)condition2.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition2[k];

							string index1 = "";
							l1++;
							while (condition2[l1] != ')')
							{
								index += condition2[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it > *it1)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
				}
				else if (condition2.find('(') != string::npos)
				{
					int l = (int)condition2.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition2[k];

					string index = "";
					l++;
					while (condition2[l] != ')')
					{
						index += condition2[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if first condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c1 == 0)
						{
							if (stoi(condition1) > *it)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition1))
						{
							if (ints.find(condition1)->second > *it)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition1.find('(') != string::npos)
						{
							int l1 = (int)condition1.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition1[k];

							string index1 = "";
							l1++;
							while (condition1[l1] != ')')
							{
								index += condition1[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it1 > *it)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
				}
			
			}
			else if (condition.find('<') != string::npos)
			{
				int l = (int)condition.find('<');
				string condition1 = "";
				string condition2 = "";
				for (int k = 0; k < l; k++)
					condition1 += condition[k];
				for (int k = l + 1; k < condition.length(); k++)
					condition2 += condition[k];

				char *c1;
				char *c2;
				strtol(condition1.c_str(), &c1, 10);
				strtol(condition2.c_str(), &c2, 10);

				if (*c1 == 0 && *c2 == 0)
				{
					if (stoi(condition1) < stoi(condition2))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && *c2 == 0)
				{
					if (ints.find(condition1)->second < stoi(condition2))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition2) && *c1 == 0)
				{
					if (ints.find(condition2)->second < stoi(condition1))
					{
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				else if (ints.count(condition1) && ints.count(condition2))
				{
					if (ints.find(condition1)->second < ints.find(condition2)->second){
						current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
					}
					else
					{
						while (code[i] != '\n')
							i++;
						current_char = i;
					}
				}
				// thing

				else if (condition1.find('(') != string::npos)
				{
					int l = (int)condition1.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition1[k];

					string index = "";
					l++;
					while (condition1[l] != ')')
					{
						index += condition1[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if second condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c2 == 0)
						{
							if (*it < stoi(condition2))
							{
								current_char = i + 1;
						code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition2))
						{
							if (*it < ints.find(condition2)->second)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition2.find('(') != string::npos)
						{
							int l1 = (int)condition2.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition2[k];

							string index1 = "";
							l1++;
							while (condition2[l1] != ')')
							{
								index += condition2[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it < *it1)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
				}
				else if (condition2.find('(') != string::npos)
				{
					int l = (int)condition2.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
						arr += condition2[k];

					string index = "";
					l++;
					while (condition2[l] != ')')
					{
						index += condition2[l];
						l++;
					}
					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
						real_index = stoi(index);

					if (int_arrs.count(arr))
					{
						// if first condition is int
						list<int> a = int_arrs.find(arr)->second;
						list<int>::iterator it = a.begin();
						advance(it, real_index);
						if (*c1 == 0)
						{
							if (stoi(condition1) < *it)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (ints.count(condition1))
						{
							if (ints.find(condition1)->second < *it)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
						else if (condition1.find('(') != string::npos)
						{
							int l1 = (int)condition1.find('(');
							string arr1 = "";
							for (int k = 0; k < l1; k++)
								arr1 += condition1[k];

							string index1 = "";
							l1++;
							while (condition1[l1] != ')')
							{
								index += condition1[l1];
								l1++;
							}
							int real_index;
							if (ints.count(index))
								real_index = ints.find(index)->second;
							else
								real_index = stoi(index);

							list<int> b = int_arrs.find(arr1)->second;
							list<int>::iterator it1 = b.begin();
							advance(it1, real_index);
							if (*it1 < *it)
							{
								current_char = i + 1;
								code.insert(loop_end, "\n" + code.substr(loop_start, (loop_end - loop_start)));
							}
							else
							{
								while (code[i] != '\n')
									i++;
								current_char = i;
							}
						}
					}
				}
			
				
			}
		}
		else if (code[current_char] != ';' || code[current_char] != ' ' || code[current_char] != '}' || code[current_char] != ']' || code[current_char] != '\n')
		{ // Variable check
			string var;
			int i = current_char;
			while (code[i] != '=' && code[i] != '-' && code[i] != '+')
				i++;
			for (int k = current_char; k < i; k++)
				var += code[k];
			char op;
			op = code[i];
			if (op == '=')
			{ // Assignment
				string value;
				int j = i;
				int removed_value;
				while (code[j] != ';')
					j++;
				for (int k = i + 1; k < j; k++)
					value += code[k];
				if (var.find('(') != string::npos)
				{ // Assign arr index to value
					int l = (int)var.find('(');
					string arr = "";
					for (int k = 0; k < l; k++)
					{
						arr += var[k];
					}
					string index = "";
					l++;
					while (var[l] != ')')
					{
						index += var[l];
						l++;
					}

					int real_index;
					if (ints.count(index))
						real_index = ints.find(index)->second;
					else
					{
						real_index = stoi(index);
					}

					//
					if (int_arrs.count(arr))
					{
						list<int> a = int_arrs.find(arr)->second;
						auto it = a.begin();
						advance(it, real_index);
						char *p;
						strtol(value.c_str(), &p, 10);
						if (*p == 0)
						{
							a.insert(it, stoi(value));
						}
						else
						{
							cerr << "Value is not same type as array\n";
							return -1;
						}
						prev(it, 1);
						a.erase(it);
						int_arrs.erase(arr);
						int_arrs.insert({arr, a});
					}
					else if (string_arrs.count(arr))
					{
						list<string> a = string_arrs.find(arr)->second;
						auto it = a.begin();
						advance(it, real_index);

						if (value[0] == '"')
						{
							value.erase(0, 1);
							value.erase(value.size() - 1, 1);
							a.insert(it, value);
						}
						else
						{
							cerr << "Value is not same type as array\n";
							return -1;
						}
						prev(it, 1);
						a.erase(it);
						string_arrs.erase(arr);
						string_arrs.insert({arr, a});
					}
					else if (bool_arrs.count(arr))
					{
						list<bool> a = bool_arrs.find(arr)->second;
						auto it = a.begin();
						advance(it, real_index);

						if (value[0] == 't')
						{
							a.insert(it, true);
						}
						else if (value[0] == 'f')
						{
							a.insert(it, false);
						}
						else
						{
							cerr << "Value is not the same type as array\n";
							return -1;
						}
						prev(it, 1);
						a.erase(it);
						bool_arrs.erase(arr);
						bool_arrs.insert({arr, a});
					}
					current_char += l;
					continue;
				}

				if (ints.count(var))
				{ // Already exists
					removed_value = ints.find(var)->second;
					ints.erase(var);
				}
				else if (strings.count(var))
					strings.erase(var);
				else if (bools.count(var))
					bools.erase(var);
				else if (int_arrs.count(var))
					int_arrs.erase(var);
				else if (string_arrs.count(var))
					string_arrs.erase(var);
				else if (bool_arrs.count(var))
					bool_arrs.erase(var);
				char *p;
				strtol(value.c_str(), &p, 10);
				if (*p == 0)
				{ // Integer
					int v = stoi(value);
					ints.insert({var, v});
				}
				else if (value == "true")
				{ // Boolean
					bools.insert({var, true});
				}
				else if (value == "false")
				{ // Boolean
					bools.insert({var, false});
				}
				else if (value[0] == '"')
				{ // String
					value.erase(0, 1);
					value.erase(value.size() - 1, 1);
					strings.insert({var, value});
				}
				else if (value[0] == '(')
				{ // Create array with set values
					value.erase(0, 1);
					value.erase(value.size() - 1, 1);
					if (value[0] == '"')
					{ // Array is string type
						list<string> temp;
						string elem = "";
						bool in = false;
						for (int z = 0; z <= value.size(); z++)
						{
							if (value[z] == ',' || z == value.size())
							{
								temp.push_back(elem);
								elem = "";
								continue;
							}
							if (value[z] == '"')
							{
								in = !in;
								continue;
							}
							if (in)
								elem += value[z];
						}
						string_arrs.insert({var, temp});
					}
					else if (value[0] == 't' || value[0] == 'f')
					{ // Array is boolean type
						list<bool> temp;
						string elem = "";
						for (int i = 0; i <= value.size(); i++)
						{
							if (value[i] == ' ')
								continue;
							if (value[i] == ',' || i == value.size())
							{
								if (elem[0] == 't')
									temp.push_back(true);
								else
									temp.push_back(false);
								elem = "";
								continue;
							}
							elem += value[i];
						}
						bool_arrs.insert({var, temp});
					}
					else
					{ // Array is int
						list<int> temp;
						string elem = "";
						for (int i = 0; i <= value.size(); i++)
						{
							if (value[i] == ' ')
								continue;
							if (value[i] == ',' || i == value.size())
							{
								temp.push_back(stoi(elem));
								elem = "";
								continue;
							}
							elem += value[i];
						}
						int_arrs.insert({var, temp});
					}
				}
				else
				{
					if (value.find('-') != string::npos)
					{
						int l = (int)value.find('-');
						string a = "";
						for (int k = i + 1; k < i + l + 1; k++)
							a += code[k];
						string b = "";
						for (int k = i + l + 2; k < j; k++)
							b += code[k];
						char *z;
						strtol(b.c_str(), &z, 10);
						char *y;
						strtol(a.c_str(), &y, 10);
						if (*z == 0 && ints.count(a))
							ints.insert({var, ints.find(a)->second - stoi(b)});
						else if (*y == 0 && ints.count(b))
							ints.insert({var, stoi(a) - ints.find(b)->second});
						else if (*z == 0 && *y == 0)
							ints.insert({var, stoi(a) - stoi(b)});
						else
						{
							if (a == var && ints.count(b))
								ints.insert({var, removed_value - ints.find(b)->second});
							else if (b == var && ints.count(a))
								ints.insert({var, ints.find(a)->second - removed_value});
							else if (a == var && *z == 0)
								ints.insert({var, removed_value - stoi(b)});
							else if (b == var && *y == 0)
								ints.insert({var, stoi(a) - removed_value});
							else
								ints.insert({var, ints.find(a)->second - ints.find(b)->second});
						}
					}

					else if (value.find('+') != string::npos)
					{
						int l = (int)value.find('+');
						string a = "";
						for (int k = i + 1; k < i + l + 1; k++)
							a += code[k];
						string b = "";
						for (int k = i + l + 2; k < j; k++)
							b += code[k];
						char *z;
						strtol(b.c_str(), &z, 10);
						char *y;
						strtol(a.c_str(), &y, 10);
						if (*z == 0 && ints.count(a))
							ints.insert({var, ints.find(a)->second + stoi(b)});
						else if (*y == 0 && ints.count(b))
							ints.insert({var, stoi(a) + ints.find(b)->second});
						else if (*z == 0 && *y == 0)
							ints.insert({var, stoi(a) + stoi(b)});
						else
						{
							if (a == var && ints.count(b))
								ints.insert({var, removed_value + ints.find(b)->second});
							else if (b == var && ints.count(a))
								ints.insert({var, ints.find(a)->second + removed_value});
							else if (a == var && *z == 0)
								ints.insert({var, removed_value + stoi(b)});
							else if (b == var && *y == 0)
								ints.insert({var, stoi(a) + removed_value});
							else
								ints.insert({var, ints.find(a)->second + ints.find(b)->second});
						}
					}

					else if (value.find('*') != string::npos)
					{
						int l = (int)value.find('*');
						string a = "";
						for (int k = i + 1; k < i + l + 1; k++)
							a += code[k];
						string b = "";
						for (int k = i + l + 2; k < j; k++)
							b += code[k];
						char *z;
						strtol(b.c_str(), &z, 10);
						char *y;
						strtol(a.c_str(), &y, 10);
						if (*z == 0 && ints.count(a))
							ints.insert({var, ints.find(a)->second * stoi(b)});
						else if (*y == 0 && ints.count(b))
							ints.insert({var, stoi(a) * ints.find(b)->second});
						else if (*z == 0 && *y == 0)
							ints.insert({var, stoi(a) * stoi(b)});
						else
						{
							if (a == var && ints.count(b))
								ints.insert({var, removed_value * ints.find(b)->second});
							else if (b == var && ints.count(a))
								ints.insert({var, ints.find(a)->second * removed_value});
							else if (a == var && *z == 0)
								ints.insert({var, removed_value * stoi(b)});
							else if (b == var && *y == 0)
								ints.insert({var, stoi(a) * removed_value});
							else
								ints.insert({var, ints.find(a)->second * ints.find(b)->second});
						}
					}

					else if (value.find('/') != string::npos)
					{
						int l = (int)value.find('/');
						string a = "";
						for (int k = i + 1; k < i + l + 1; k++)
							a += code[k];
						string b = "";
						for (int k = i + l + 2; k < j; k++)
							b += code[k];
						char *z;
						strtol(b.c_str(), &z, 10);
						char *y;
						strtol(a.c_str(), &y, 10);
						if (*z == 0 && ints.count(a))
							ints.insert({var, ints.find(a)->second / stoi(b)});
						else if (*y == 0 && ints.count(b))
							ints.insert({var, stoi(a) / ints.find(b)->second});
						else if (*z == 0 && *y == 0)
							ints.insert({var, stoi(a) / stoi(b)});
						else
						{
							if (a == var && ints.count(b))
								ints.insert({var, removed_value / ints.find(b)->second});
							else if (b == var && ints.count(a))
								ints.insert({var, ints.find(a)->second / removed_value});
							else if (a == var && *z == 0)
								ints.insert({var, removed_value / stoi(b)});
							else if (b == var && *y == 0)
								ints.insert({var, stoi(a) / removed_value});
							else
								ints.insert({var, ints.find(a)->second / ints.find(b)->second});
						}
					}
				}
				current_char = j; // Skip forward
			}
			else if (op == '-')
			{ // Remove value at index from array
				string value;
				int j = i;
				int removed_value;
				while (code[j] != ';')
					j++;
				for (int k = i + 1; k < j; k++)
					value += code[k];

				value.erase(0, 1);
				value.erase(value.size() - 1, 1);

				int real_index;
				if (ints.count(value))
					real_index = ints.find(value)->second;
				else
					real_index = stoi(value);

				if (int_arrs.count(var))
				{
					list<int> a = int_arrs.find(var)->second;
					auto it = a.begin();
					advance(it, real_index);
					a.erase(it);
					int_arrs.erase(var);
					int_arrs.insert({var, a});
				}
				else if (string_arrs.count(var))
				{
					list<string> a = string_arrs.find(var)->second;
					auto it = a.begin();
					advance(it, real_index);
					a.erase(it);
					string_arrs.erase(var);
					string_arrs.insert({var, a});
				}
				else if (bool_arrs.count(var))
				{
					list<bool> a = bool_arrs.find(var)->second;
					auto it = a.begin();
					advance(it, real_index);
					a.erase(it);
					bool_arrs.erase(var);
					bool_arrs.insert({var, a});
				}

				current_char = j;
			}
			else if (op == '+')
			{ // Append values to array
				string value;
				int j = i;
				int removed_value;
				while (code[j] != ';')
					j++;
				for (int k = i + 1; k < j; k++)
					value += code[k];

				value.erase(0, 1);
				value.erase(value.size() - 1, 1);
				if (value[0] == '"')
				{ // Array is string type
					list<string> a = string_arrs.find(var)->second;
					string elem = "";
					bool in = false;
					for (int z = 0; z <= value.size(); z++)
					{
						if (value[z] == ',' || z == value.size())
						{
							a.push_back(elem);
							elem = "";
							continue;
						}
						if (value[z] == '"')
						{
							in = !in;
							continue;
						}
						if (in)
							elem += value[z];
					}
					string_arrs.erase(var);
					string_arrs.insert({var, a});
				}
				else if (value[0] == 't' || value[0] == 'f')
				{
					list<bool> a = bool_arrs.find(var)->second;
					string elem = "";
					for (int i = 0; i <= value.size(); i++)
					{
						if (value[i] == ' ')
							continue;
						if (value[i] == ',' || i == value.size())
						{
							if (elem[0] == 't')
								a.push_back(true);
							else
								a.push_back(false);
							elem = "";
							continue;
						}
						elem += value[i];
					}
					bool_arrs.erase(var);
					bool_arrs.insert({var, a});
				}
				else
				{
					list<int> a = int_arrs.find(var)->second;
					string elem = "";
					for (int i = 0; i <= value.size(); i++)
					{
						if (value[i] == ' ')
							continue;
						if (value[i] == ',' || i == value.size())
						{
							a.push_back(stoi(elem));
							elem = "";
							continue;
						}
						elem += value[i];
					}
					int_arrs.erase(var);
					int_arrs.insert({var, a});
				}
				current_char = j;
			}
		}
		current_char++;
	}
}
