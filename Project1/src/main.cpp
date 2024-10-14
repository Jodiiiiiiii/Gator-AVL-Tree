#include <iostream>
#include <string>
#include <sstream>
#include "GatorAVLTree.h"
using namespace std;

/* Note: 
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file and any header files you have on Gradescope. 
*/

int main(){

	// read first input line (number of commands)
	string line;
	getline(cin, line);
	// create string stream to parse input into individual components
	istringstream in(line);
	int numCommands;
	in >> numCommands;
	
	// create custom GatorAVLTree data structure
	GatorAVLTree students;

	// conduct commands
	for(int i = 0; i < numCommands; i++)
	{
		// print endline character to start new command on next line
		if(i != 0)
			cout << endl;

		// read next command line
		getline(cin, line);
		// clear and re-instantiate string stream to parse components
		in.clear();
		in.str(line);

		// read command, then determine proper function to call
		string command;
		in >> command;
		if(command == "insert")
		{
			// read name input (keep reading until starting and final quotation marks are found)
			string name;
			in >> name;

			if(name[0] != '"')
			{
				cout << "unsuccessful";
				continue; // continute to next comma nd; since name is not within quotes, it is improperly formatted
			}

			while(name[name.length()-1] != '"')
			{
				string nameAppend;
				in >> nameAppend;
				name.push_back(' ');
				name.append(nameAppend);
			}
			// trim quotation mark off front and back of name
			name = name.substr(1, name.length()-2);

			// validate that name is composed entirely of alphabetical characters (or spaces)
			bool nonAlpha = false;
			for(char c : name)
			{
				if(!isalpha(c) && c != ' ')
				{
					// failure!
					cout << "unsuccessful";
					nonAlpha = true;
					break; // breaks out of for loop for checking characters
				}
			}
			if(nonAlpha)
				continue; // ends this execution of insert command and moves to next command

			// read id input, first as a string
			string idStr;
			in >> idStr;
			// attempt to convert id to int
			int id;
			try
			{
				// validate the string is composed entirely of digits
				bool nonNumerical = false;
				for(char c : idStr)
				{
					if(!isdigit(c))
					{
						// failure!
						cout << "unsuccessful";
						nonNumerical = true;
						break; // breaks out of for loop for checking characters
					}
				}
				if(nonNumerical)
					continue; // ends this execution of the command and moves to next command

				if(idStr.length() == 8)
				{
					id = stoi(idStr);

					// validate int is unsigned and has 8 digits (including leading zeros) before calling function
					if(id >= 0 && idStr.length() == 8)
						students.Insert(name, id);
					else
						cout << "unsuccessful";
				}
				else
					cout << "unsuccessful";
			}
			catch(invalid_argument e)
			{	
				// input not valid integer
				cout << "unsuccessful";
			}
		}
		else if(command == "remove")
		{
			// read id input, first as a string
			string idStr;
			in >> idStr;
			// attempt to convert id to int
			int id;
			try
			{
				// validate the string is composed entirely of digits
				bool nonNumerical = false;
				for(char c : idStr)
				{
					if(!isdigit(c))
					{
						// failure!
						cout << "unsuccessful";
						nonNumerical = true;
						break; // breaks out of for loop for checking characters
					}
				}
				if(nonNumerical)
					continue; // ends this execution of the command and moves to next command

				if(idStr.length() == 8)
				{
					id = stoi(idStr);

					// validate int is unsigned and has 8 digits before calling function
					if(id >= 0 && idStr.length() == 8)
						students.Remove(id);
					else
						cout << "unsuccessful";
				}
				else
					cout << "unsuccessful";
				
			}
			catch(invalid_argument e)
			{	
				// input not valid integer
				cout << "unsuccessful";
			}
		}
		else if(command == "search")
		{
			// read parameter as string
			string paramStr;
			in >> paramStr;

			// search(name) case
			if(paramStr[0]=='"')
			{
				// read paramStr input (keep reading until starting and final quotation marks are found)
				while(paramStr[paramStr.length()-1] != '"')
				{
					string paramAppend;
					in >> paramAppend;
					paramStr.push_back(' ');
					paramStr.append(paramAppend);
				}
				// trim quotation marks off front and back of name
				paramStr = paramStr.substr(1, paramStr.length()-2);

				// validate that search name is composed entirely of alphabetical characters (or spaces)
				bool nonAlpha = false;
				for(char c : paramStr)
				{
					if(!isalpha(c) && c != ' ')
					{
						// failure!
						cout << "unsuccessful";
						nonAlpha = true;
						break; // exits for loop for verifying individual characters
					}
				}
				if(nonAlpha)
					continue; // ends this execution of search command and moves to next command
				
				// call function since all validation checks have been passed for string parameter
				students.Search(paramStr);
			}
			else // search(ID) case
			{
				// attempt to convert to int to determine which overloaded function to call
				int paramInt;
				try
				{
					// validate the string is composed entirely of digits
					bool nonNumerical = false;
					for(char c : paramStr)
					{
						if(!isdigit(c))
						{
							// failure!
							cout << "unsuccessful";
							nonNumerical = true;
							break; // breaks out of for loop for checking characters
						}
					}
					if(nonNumerical)
						continue; // ends this execution of the command and moves to next command

					if(paramStr.length() == 8)
					{
						paramInt = stoi(paramStr);

						// validate int is unsigned and has 8 digits (including leading zeros) before calling function
						if(paramInt >= 0 && paramStr.length() == 8)
						{
							students.Search(paramInt);
						}
						else
						{
							// valid integer, but either not unsigned or not 8 digits
							cout << "unsuccessful";
						}	
					}
					else
					{
						// valid integer, but either not unsigned or not 8 digits
						cout << "unsuccessful";
					}	
					
				}
				catch(invalid_argument e)
				{
					// input not valid integer
					cout << "unsuccessful";
				}
			}
		}
		else if(command == "printInorder")
		{
			students.PrintInorder();
		}
		else if(command == "printPreorder")
		{
			students.PrintPreorder();
		}
		else if(command == "printPostorder")
		{
			students.PrintPostorder();
		}
		else if(command == "printLevelCount")
		{
			students.PrintLevelCount();
		}
		else if(command == "removeInorder")
		{
			// read id input, first as a string
			string indexStr;
			in >> indexStr;
			// attempt to convert id to int
			int index;
			try
			{
				// validate the string is composed entirely of digits
				bool nonNumerical = false;
				for(char c : indexStr)
				{
					if(!isdigit(c))
					{
						// failure!
						cout << "unsuccessful";
						nonNumerical = true;
						break; // breaks out of for loop for checking characters
					}
				}
				if(nonNumerical)
					continue; // ends this execution of the command and moves to next command

				index = stoi(indexStr);

				// validate int is unsigned (between both min and max value)
				if(index >= 0 && index <= 4294967295) // upper bound likely not necessary
					students.RemoveInorder(index);	
				else
					cout << "unsuccessful";
			}
			catch(invalid_argument e)
			{	
				// input not valid integer
				cout << "unsuccessful";
			}
		}
		else // no valid command name
		{
			cout << "unsuccessful";
		}
	}
	cout << endl;

	return 0;
}