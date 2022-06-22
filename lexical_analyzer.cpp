#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include "unordered_hash_table.h"
using namespace std;

class LexicalAnalyzer {
	private:
		// The code below is a transition table for the DFA.
		int transitionTable[36][20] = {{1,2,0,14,15,16,3,6,7,8,9,24,25,26,27,28,29,30,31,35},
									  {1,32,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,32},
									  {33,2,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,33},
									  {13,13,13,13,13,4,13,13,13,13,13,13,13,13,13,13,13,13,13,35},
									  {4,4,4,4,4,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
									  {4,4,4,4,4,4,12,4,4,4,4,4,4,4,4,4,4,4,4,4},
									  {17,17,17,17,17,17,17,18,17,17,17,17,17,17,17,17,17,17,17,34},
									  {19,19,34,34,34,34,34,19,34,19,19,19,19,19,19,19,19,19,19,34},
									  {20,20,20,20,20,20,20,21,20,20,20,20,20,20,20,20,20,20,20,34},
									  {22,22,22,22,22,22,22,23,22,22,23,22,22,22,22,22,22,22,22,34},
									  {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
									  {11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11},
									  {12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},
									  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
									  {14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14},
									  {15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15},
									  {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
									  {17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17},
									  {18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18},
									  {19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19},
									  {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20},
									  {21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21},
									  {22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22},
									  {23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23},
									  {24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24},
									  {25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25},
									  {26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26},
									  {27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27},
									  {28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28},
									  {29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29},
									  {30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30},
									  {31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31},
									  {32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32},
									  {33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33},
									  {34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34},
									  {35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35},
									};
		int errorLine = 1;
		// Id of tokens, as defined in the analysis section
		int tokenId = 28;
		int numberId = 29;
		int commentId = 30;
		// start index at 1
		int idIndex = 1;
		int numberIndex = 1;
		// becase acceptors states start at 10, 13 is where special symbols acceptors state start
		const int previousStates = 13;
		// file to write the scanner outputs
		ofstream scannerOutput;
	public:
		UnorderedHashTable identifiersSymbolTable;
		UnorderedHashTable numbersSymbolTable;
		string specialSymbols[19] = {"/", "+", "-", "*", "=", "==", "!=", "<", "<=", ">", ">=", ";", ",", "(", ")", "[", "]", "{", "}"};
		string reservedWords[8] = {"if", "else", "int", "void", "return", "while", "input", "output"};

		/**
		 * The constructor for the LexicalAnalyzer class
		 */
		LexicalAnalyzer() {
			// create new text file named scannerOutput, where the result of the scanner will be written
			scannerOutput.open("scannerOutput.txt");
		}

		/**
		 * This function checks the state of the acceptors and prints the output to the console and to the
		 * scannerOutput.txt file
		 * 
		 * @param state the state that the DFA is in
		 * @param id string variable that stores the id
		 * @param number the number that was found
		 * @param specialSymbol is the string that contains the special symbol that was found
		 * 
		 * @return void, it is not returning anything.
		 */
		void checkAcceptorsState(int state, string &id, string &number, string &specialSymbol) {

			string lowerCaseId = "";
			// create a temp Index, so that we dont modify the id Index
			int tempIndexId;
			switch (state)
			{
			case 10:
				// transform the id to lowercase so that we can check if is reserved word
				for(int i = 0; i < id.size(); i++) {
					char temp;

					temp = std::tolower(id[i]);
					lowerCaseId += temp;
				}

				if(checkIfIdIsReservedWord(lowerCaseId, tempIndexId)) {
					cout << to_string(tempIndexId) << "\n";
					scannerOutput << to_string(tempIndexId)  << "\n";
				} else {
					// if the id is already stored in the table
					if(id == identifiersSymbolTable.getContent(tokenId,id)) {
						printAcceptorsOutput(tokenId, id);
						break;
					} else {
						identifiersSymbolTable.insert(tokenId ,id, idIndex);
						printAcceptorsOutput(tokenId, id);
						idIndex++;
					}
				}
				break;
			case 11:
				// if the number is already stored in the table
				if(number == numbersSymbolTable.getContent(numberId, number)) {
					printAcceptorsOutput(numberId, number);
					break;
				}
				numbersSymbolTable.insert(numberId ,number, numberIndex);
				printAcceptorsOutput(numberId, number);

				numberIndex++;
				break;
			case 12:
				break;
			case 13:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 14:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 15:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 16:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 17:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 18:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 19:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 20:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 21:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 22:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 23:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 24:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 25:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 26:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 27:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 28:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 29:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 30:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}		
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			case 31:
				if(specialSymbol == specialSymbols[state - previousStates]) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				printAcceptorsOutput(state - previousStates, specialSymbol);
				break;
			} 

			// reset string variables, for next loop
			id = "";
			number = "";
			specialSymbol = "";
			return;
		}

		/**
		 * It prints the tokenId and the slot of the token
		 * 
		 * @param tokenId The token id of the token that was accepted.
		 * @param content The content of the token.
		 * 
		 * @return the tokenId and the content of the token.
		*/
		void printAcceptorsOutput(int tokenId, string content) {

			int numberId = tokenId, specialSymbolId = tokenId;

			if(tokenId == 28) {
				cout << tokenId << "  " << identifiersSymbolTable.getSlot(tokenId, content) << "\n";
				scannerOutput << tokenId << " " << identifiersSymbolTable.getSlot(tokenId, content) << "\n";
				return;
			} else if (numberId == 29) {
				cout << numberId << "  " << numbersSymbolTable.getSlot(numberId, content) << "\n";
				scannerOutput << numberId << " " << numbersSymbolTable.getSlot(numberId, content) << "\n";
				return;
			} else if( specialSymbolId >= 0 && specialSymbolId <= 19) {
				// it starts at 9, because before we have the reserved words
				int startingSpecialSymbolsNumber = 9;
				cout << to_string(specialSymbolId + startingSpecialSymbolsNumber) << "\n";
				scannerOutput << to_string(specialSymbolId + startingSpecialSymbolsNumber) << "\n";
				return;
			}

			return;
		}

		/**
		 * It checks if the string passed to it is a reserved word, and if it is, it returns true and sets
		 * the tempIdIndex to the index of the reserved word in the identifiers symbol table.
		 * 
		 * @param id The string that is being checked to see if it is a reserved word.
		 * @param tempIdIndex This is the index of the reserved word in the reservedWord array.
		 * 
		 * @return a boolean value.
		 */
		bool checkIfIdIsReservedWord(string id, int &tempIdIndex) {
			if(id == "if") { tempIdIndex = 1; return true;}
			if(id == "else") { tempIdIndex = 2; return true;}
			if(id == "int") { tempIdIndex = 3; return true;}
			if(id == "void") { tempIdIndex = 4; return true;}
			if(id == "return") { tempIdIndex = 5; return true;}
			if(id == "while") { tempIdIndex = 6; return true;}
			if(id == "input") { tempIdIndex = 7; return true;}
			if(id == "output") { tempIdIndex = 8; return true;}
			return false;
		}

		/**
		 * This function checks the state of the DFA and prints out the appropriate error message
		 * 
		 * @param state the state that the DFA is in
		 * @param errorLine The line number where the error occurred
		 * 
		 * @return void, it is not returning anything.
		 */
		void checkErrorsState(int state, int errorLine) {

			switch(state) {
				case 32:
					cout << "Id error in line " << errorLine << "\n";
					scannerOutput << "Id error in line " << errorLine << "\n";
					break;
				case 33:
					cout << "Number error in line " << errorLine << "\n";
					scannerOutput << "Number error in line " << errorLine << "\n";
					break;
				case 34:
					cout << "Special Symbol error in line " << errorLine << "\n";
					scannerOutput << "Special Symbol error in line " << errorLine << "\n";
					break;
				case 35:
					cout << "Unknown character error in line " << errorLine << "\n";
					scannerOutput << "Unknown character error in line " << errorLine << "\n";
					break;
			}
		}
		
		/**
		 * It checks if a character is a letter
		 * 
		 * @param ch The character to check if it's a letter.
		 * 
		 * @return A boolean value.
		 */
		bool checkIfCharIsLetter(char ch) {
			string alphabet = "ABCEDFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

			size_t found = alphabet.find(ch);
			if(found != string::npos) {
				return true;
			}

			return false;
		}

		/**
		 * It checks if a character is a number
		 * 
		 * @param ch The character to check if it's a number.
		 * 
		 * @return a boolean value.
		 */
		bool checkIfCharIsNumber(char ch) {
			string numbers = "0123456789";

			size_t found = numbers.find(ch);
			if(found != string::npos) {
				return true;
			}

			return false;
		}

		/**
		 * It checks if the character is a special symbol and if it is, it returns the column number of the
		 * transition table
		 * 
		 * @param ch the character to check
		 * @param column the column of the transition table
		 * 
		 * @return a boolean value.
		 */
		bool checkIfCharIsSpecialSymbol(char ch, int &column) {
			string specialSymbols = "+-*/=!<>;,()[]{}";

			size_t found = specialSymbols.find(ch);
			if(found != string::npos) {
				// sum by 3 because transition table, starts first with letters, digits and whitespace
				column = found + 3;
				return true;
			}

			return false;
		}

		/**
		 * It checks if the character is a whitespace character
		 * 
		 * @param ch The character to check
		 * @param column This is the column number of the character in the file.
		 * 
		 * @return a boolean value.
		 */
		bool checkIfCharIsWhitespace(char ch, int &column) {
			string whitespace = " \n\t";

			size_t found = whitespace.find(ch);
			if(found != string::npos) {
				column = 2;
				return true;
			}

			return false;
		}


		/**
		 * It reads the file character by character and checks if it's a letter, number, whitespace, special
		 * symbol or unknown character. If it's a letter, number or special symbol it stores it in a string
		 * and checks if it's an acceptor state or an error state. If it's an acceptor state it checks if
		 * it's a keyword, identifier, number or special symbol and prints it in the output file. If it's an
		 * error state it prints the error in the output file.
		 * 
		 * @param fileName the name of the file that we want to read
		 * 
		 * @return a string.
		 */
		string readCharStream(string fileName) {

			FILE* file = fopen(fileName.c_str(), "r");
			string id = "", number = "", specialSymbol = "";

			char ch;
			int state = 0;
			// transition state column, which includes letter, digit, whitespace, special symbols, unknown char
			int column = 0;
			bool isAcceptorState = false;
			bool isErrorState = false;
			int commentErrorLine;

			ch = getc(file);

			// loop while we don't reach the end of the file
			while(!feof(file)) {

				while(isAcceptorState != true && isErrorState != true) {

					if(checkIfCharIsLetter(ch)) {
						column = 0;
						id += ch;
					}

					if(checkIfCharIsNumber(ch)) {
						column = 1;
						number += ch;
					}

					if(checkIfCharIsSpecialSymbol(ch, column)) {
						specialSymbol += ch;
					}

					checkIfCharIsWhitespace(ch,column);

					/* Checking if the character is a letter, number, special symbol, or whitespace. If it is not any
					of those, it sets the corresponding column to an unknown character error */
					if(!checkIfCharIsLetter(ch) && !checkIfCharIsNumber(ch) && !checkIfCharIsSpecialSymbol(ch, column) && !checkIfCharIsWhitespace(ch, column)) {
						column = 19;
					}

					// count where the error line appears
					if(ch == '\n') {
						errorLine++;
					}
					
					/* The code below is checking if the file is at the end of the file and if the state is 4. If both
					of these are true, then it will print out that the comment was not closed in the line that the
					comment was opened. */
					if(feof(file) && state == 4) {
						cout << "Comment not closed in line " << commentErrorLine << "\n";
						return "\n";
					}

					// if we reached the end of file set the column to whitespace to store whatever id, number or special symbol left.
					if(feof(file)) {
						column = 2;
					}

					state = transitionTable[state][column];

					// The code below is a switch case statement that is used to determine the next state of the DFA. */
					switch (state)
					{
					case 0:
						ch = getc(file);
						break;
					case 1:
						if(state == 10) {
							isAcceptorState = true;
							break;
						} else if(state == 32) {
							isErrorState = true;
							break;
						}
						ch = getc(file);
						break;
					case 2:
						if(state == 11) {
							isAcceptorState = true;
							break;
						} else if(state == 33) {
							isErrorState = true;
						}
						ch = getc(file);
						
						break;
					case 3:
						if(state == 13) {
							isAcceptorState = true;
							break;
						} else if(state == 35) {
							isErrorState = true;
							break;
						}
						ch = getc(file);
						
						break;
					case 4:
						if(ch == '*') {
							commentErrorLine = errorLine;
						}
						ch = getc(file);
						break;
					case 5:
						ch = getc(file);
						break;
					case 6:
						if(state == 17 | state == 18) {
							isAcceptorState = true;
							break;
						} else if(state == 34) {
							isErrorState = true;
							break;
						}
						ch = getc(file);
						break;
					case 7:
						if(state == 19) {
							isAcceptorState = true;
							break;
						} else if(state == 34) {
							isErrorState = true;
							break;
						}
						ch = getc(file);
						break;
					case 8:
						if(state == 20 | state == 21) {
							isAcceptorState = true;
							break;
						} else if(state == 34) {
							isErrorState = true;
							break;
						}
						ch = getc(file);
						break;
					case 9:
						if(state == 22 | state == 23) {
							isAcceptorState = true;
							break;
						} else if(state == 34) {
							isErrorState = true;
							break;
						}
						ch = getc(file);
						break;
					case 10:
						if(checkIfCharIsLetter(ch)) {
							isAcceptorState = true;
							ch = getc(file);
							break;
						}
						isAcceptorState = true;
						break;
					case 11:
						if(checkIfCharIsNumber(ch)) {
							isAcceptorState = true;
							ch = getc(file);
							break;
						}
						isAcceptorState = true;
						break;
					case 12:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 13:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 14:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 15:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 16:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 17:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 18:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 19:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 20:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 21:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 22:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 23:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 24:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 25:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 26:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 27:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 28:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 29:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 30:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 31:
						ch = getc(file);
						isAcceptorState = true;
						break;
					case 32:
						isErrorState = true;
						break;
					case 33:
						isErrorState = true;
						break;
					case 34:
						isErrorState = true;
						break;
					case 35:
						isErrorState = true;
						break;
					} // end of switch case
				} // end of while
				
				if(isAcceptorState == true) {

					checkAcceptorsState(state,id, number, specialSymbol);

					state = 0;
					isAcceptorState = false;
				} else if(isErrorState == true) {

					checkErrorsState(state, errorLine);

					state = 0;
					return "\n";
				}

			} // end of while

			// close the file to prevent corruption
			scannerOutput << "$" << "\n";
			scannerOutput.close();
			return "\n";
		}
};

class SyntaxAnalysis {
	private: 
		string currentTokenTableIndex = "";
	public:
	ifstream file;
	bool matched = false;
	SyntaxAnalysis() {
		file.open("scannerOutput.txt");
	}

	string getNextToken(string &currentToken) {
		currentToken = "";
		currentTokenTableIndex = "";

		while(getline(file, currentToken, '\n')) {
			// check if the input is an identifier, or a number
			if(currentToken.substr(0,2) == "28" || currentToken.substr(0,2) == "29") {
				int position = currentToken.find(" ");

				currentTokenTableIndex = currentToken.substr(position, currentToken.length());
				currentToken = currentToken.substr(0, position);
			} 

			return currentToken;
		}
	}

	void startSyntaxAnalysis() {

		string currentToken = "";

		getNextToken(currentToken);

		program(currentToken);

		if(currentToken == "$") {
			cout << "Syntax Analysis OK" << "\n";
			abort();
		} else {
			cout << "Syntax Analysis ERROR" << "\n";
			abort();
		}

		
	}

	void program(string &currentToken) {
		if( currentToken == "3" ) {
			getNextToken(currentToken);
			if( currentToken == "28" ) {
				getNextToken(currentToken);
				declaration(currentToken);
				program(currentToken);
			} else {
				cout << "Syntax Error: Expected ID, got: " + currentToken << "\n";
			}
		}

		if( currentToken == "4" ) {
			getNextToken(currentToken);
			if( currentToken == "28") {
				getNextToken(currentToken);
				if( currentToken == "22") {
					getNextToken(currentToken);
					params(currentToken);
					if(currentToken == "23") {
						getNextToken(currentToken);
						compound_stmt(currentToken);
						program(currentToken);
					} else {
						cout << "Syntax Error: Expected ) , got: " + currentToken << "\n";
					}

				} else {
					cout << "Syntax Error: Expected ( , got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected ID, got: " + currentToken << "\n";
			}
		}

		if( currentToken == "$") {
			return;
		}
	}

	void declaration(string &currentToken)	{
		if( currentToken == "20") {
			getNextToken(currentToken);
			return;
		}

		if(currentToken == "24") {
			getNextToken(currentToken);
			if( currentToken == "29" ) {
				getNextToken(currentToken);
				if( currentToken == "25") {
					getNextToken(currentToken);
					if( currentToken == "20") {
						getNextToken(currentToken);
						return ;
					} else {
						cout << "Syntax Error: Expected ;, got: " + currentToken << "\n";
					}
				} else {
					cout << "Syntax Error: Expected ], got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected NUM, got: " + currentToken << "\n";
			}
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			params(currentToken);
			if( currentToken == "23") {
				getNextToken(currentToken);
				compound_stmt(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
			}
		} else {
			cout << "Syntax Error: Expected (, got: " + currentToken << "\n";
		}

	}

	void params(string &currentToken) {
		if( currentToken == "3") {
			getNextToken(currentToken);
			if( currentToken == "28" ) {
				getNextToken(currentToken);
				param(currentToken);
				param_list(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ID, got: " + currentToken << "\n";
			}
		}

		if( currentToken == "4") {
			getNextToken(currentToken);
			return;
		} else {
			cout << "Syntax Error: Expected void, got: " + currentToken << "\n";
		}
	}

	void param_list(string &currentToken) {
		if( currentToken == "21") {
			getNextToken(currentToken);
			if( currentToken == "3" ) {
				getNextToken(currentToken);
				if( currentToken == "28" ) {
					getNextToken(currentToken);
					param(currentToken);
					param_list(currentToken);
					return ;
				}
			}
		}

		if( currentToken == "23") {
			return;
		} else {
			cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
		}
	}

	void param(string &currentToken) {
		if( currentToken == "24" ) {
			getNextToken(currentToken);
			if( currentToken == "25") {
				getNextToken(currentToken);
				return ;
			} else {
				cout << "Syntax Error: Expected ], got: " + currentToken << "\n";
			}
		}

		if( currentToken == "21" ) {
			return;
		}

		if( currentToken == "23") {
			return;
		} else {
			cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
		}
	}

	void compound_stmt(string &currentToken) {
		if( currentToken == "26") {
			getNextToken(currentToken);
			local_declarations(currentToken);
			statement_list(currentToken);
			if( currentToken == "27") {
				getNextToken(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected }, got: " + currentToken << "\n";
			}
		} else {
			cout << "Syntax Error: Expected {, got: " + currentToken << "\n";
		}
	}

	void local_declarations(string &currentToken) {
		if( currentToken == "3") {
			getNextToken(currentToken);
			if( currentToken == "28" ) {
				getNextToken(currentToken);
				local_declarations_prime(currentToken);
				return;
			}
		}

		if( currentToken == "28" || currentToken == "26" || currentToken == "1" || 
			currentToken == "6"  || currentToken == "5"  || currentToken == "7" || 
			currentToken == "8" || currentToken == "27" ) {
			
			return;
		} else {
			cout << "Syntax Error: Expected either ID, { , if, while, return, input, output, or }, got: " + currentToken << "\n";
		}
	}

	void local_declarations_prime(string &currentToken) {
		if( currentToken == "20") {
			getNextToken(currentToken);
			local_declarations(currentToken);
			return;
		}

		if( currentToken == "24") {
			getNextToken(currentToken);
			if( currentToken == "29" ) {
				getNextToken(currentToken);
				if( currentToken == "25") {
					getNextToken(currentToken);
					if( currentToken == "20") {
						getNextToken(currentToken);
						local_declarations(currentToken);
						return;
					} else {
						cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
					}
				} else {
					cout << "Syntax Error: Expected ] , got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected NUM , got: " + currentToken << "\n";
			}
		} else {
			cout << "Syntax Error: Expected [ , got: " + currentToken << "\n";
		}
	}

	void statement_list(string &currentToken) {
		if( currentToken == "28" ) {
			getNextToken(currentToken);
			statement(currentToken);
			statement_list(currentToken);
			return;
		}

		if( currentToken == "26") {
			getNextToken(currentToken);
			local_declarations(currentToken);
			statement_list(currentToken);
			if( currentToken == "27") {
				getNextToken(currentToken);
				statement_list(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected }, got: " + currentToken << "\n";
			}
		}

		if( currentToken == "1" ) {
			getNextToken(currentToken);
			if( currentToken == "22" ) {
				getNextToken(currentToken);
				expression(currentToken);
				if( currentToken == "23" ) {
					getNextToken(currentToken);
					statement_list(currentToken);
					else_part(currentToken);
					statement_list(currentToken);
					return;
				} else {
					cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected (, got: " + currentToken << "\n";
			}
		}

		if( currentToken == "6") {
			getNextToken(currentToken);
			if( currentToken == "22") {
				getNextToken(currentToken);
				expression(currentToken);
				if( currentToken == "23" ) {
					getNextToken(currentToken);
					statement_list(currentToken);
					statement_list(currentToken);
					return;
				} else {
					cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected (, got: " + currentToken << "\n";
			}
		}

		if( currentToken == "5" ) {
			getNextToken(currentToken);
			return_stmt(currentToken);
			statement_list(currentToken);
			return;
		}

		if( currentToken == "7") {
			getNextToken(currentToken);
			if( currentToken == "28" ) {
				getNextToken(currentToken);
				var(currentToken);
				if( currentToken == "20" ) {
					getNextToken(currentToken);
					statement_list(currentToken);
					return;
				} else {
					cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected ID, got: " + currentToken << "\n";
			}
		}

		if( currentToken == "8") {
			getNextToken(currentToken);
			expression(currentToken);
			if( currentToken == "20") {
				getNextToken(currentToken);
				statement_list(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "27" ) {
			return;
		} else {
			cout << "Syntax Error: Expected } , got: " + currentToken << "\n";
		}
	}

	void statement(string &currentToken) {
		if( currentToken == "24" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "25") {
				getNextToken(currentToken);
				if( currentToken == "13") {
					getNextToken(currentToken);
					expression(currentToken);
					if( currentToken == "20" ) {
						getNextToken(currentToken);
						return;
					} else {
						cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
					}
				} else {
					cout << "Syntax Error: Expected = , got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected ] , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "13" ) {
			getNextToken(currentToken);
			expression(currentToken);
			if( currentToken == "20" ) {
				getNextToken(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			call_stmt(currentToken);
			if( currentToken == "20" ) {
				getNextToken(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
			} 
		}

	}

	void else_part(string &currentToken) {
		if( currentToken == "2") {
			getNextToken(currentToken);
			statement_list(currentToken);
			return;
		}

		if( currentToken == "28" || currentToken == "26" || currentToken == "1" || 
			currentToken == "6"  || currentToken == "5"  || currentToken == "7"
			&& currentToken == "8" || currentToken == "27" ) {
			return;
		} else {
			cout << "Syntax Error: Expected either ID, { , if, while, return, input, output, or }, got: " + currentToken << "\n";
		}
	}

	void return_stmt(string &currentToken) {
		if( currentToken == "20" ) {
			getNextToken(currentToken);
			return;
		}

		if( currentToken == "28" ) {
			getNextToken(currentToken);
			factor_prime(currentToken);
			term_prime(currentToken);
			arithmetic_expression_prime(currentToken);
			expression_prime(currentToken);
			if( currentToken == "20" ) {
				getNextToken(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "23" ) {
				getNextToken(currentToken);
				term_prime(currentToken);
				arithmetic_expression_prime(currentToken);
				expression_prime(currentToken);
				if( currentToken == "20" ) {
					getNextToken(currentToken);
					return;
				} else {
					cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected ) , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "29" ) {
			getNextToken(currentToken);
			term_prime(currentToken);
			arithmetic_expression_prime(currentToken);
			expression_prime(currentToken);
			if( currentToken == "20" ) {
				getNextToken(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ; , got: " + currentToken << "\n";
			}
		}

	}

	void var(string &currentToken) {
		if( currentToken == "24" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "25" ) {
				getNextToken(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ] , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "28" || currentToken == "26" || currentToken == "1" || 
			currentToken == "6"  || currentToken == "5"  || currentToken == "7"
			&& currentToken == "8" || currentToken == "20" ) {
			return;
		} else {
			cout << "Syntax Error: Expected either ID, { , if, while, return, input, output, or }, got: " + currentToken << "\n";
		}
	}

	void expression(string &currentToken) {
		if( currentToken == "28" ) {
			getNextToken(currentToken);
			factor_prime(currentToken);
			term_prime(currentToken);
			arithmetic_expression_prime(currentToken);
			expression_prime(currentToken);
			return;
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "23" ) {
				getNextToken(currentToken);
				term_prime(currentToken);
				arithmetic_expression_prime(currentToken);
				expression_prime(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected  ) , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "29" ) {
			getNextToken(currentToken);
			term_prime(currentToken);
			arithmetic_expression_prime(currentToken);
			expression_prime(currentToken);
			return;
		}

	}

	void expression_prime(string &currentToken) {
		if( currentToken == "17" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			return;
		}

		if( currentToken == "16" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			return;
		}

		if( currentToken == "18" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			return;
		}

		if( currentToken == "19" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			return;
		}

		if( currentToken == "14" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			return;
		}

		if( currentToken == "15" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			return;
		}

		if( currentToken == "23" || currentToken == "20" ) {
			return;
		} else {
			cout << "Syntax Error: Expected  ) , got: " + currentToken << "\n";
		}
	}

	void arithmetic_expression(string &currentToken) {
		if( currentToken == "28" ) {
			getNextToken(currentToken);
			factor_prime(currentToken);
			term_prime(currentToken);
			arithmetic_expression_prime(currentToken);
			return;
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "23" ) {
				getNextToken(currentToken);
				term_prime(currentToken);
				arithmetic_expression_prime(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected  ) , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "29" ) {
			getNextToken(currentToken);
			term_prime(currentToken);
			arithmetic_expression_prime(currentToken);
			return;
		}

	}

	void arithmetic_expression_prime(string &currentToken) {
		if( currentToken == "10" ) {
			getNextToken(currentToken);
			term(currentToken);
			arithmetic_expression_prime(currentToken);
			return;
		}

		if( currentToken == "11" ) {
			getNextToken(currentToken);
			term(currentToken);
			arithmetic_expression_prime(currentToken);
			return;
		}

		if( currentToken == "17" || currentToken == "16" || currentToken == "18" || currentToken == "19" || 
			currentToken == "14" || currentToken == "15" || currentToken == "25" || currentToken == "23" ||
			currentToken == "20" || currentToken == "21" ) {
			return;
		} else {
			cout << "Syntax Error: Expected either <=, <, >, >=, ==, !=, ], ), or ; , got: " + currentToken << "\n";
		}

	}

	void term(string &currentToken) {
		if( currentToken == "28" ) {
			getNextToken(currentToken);
			factor_prime(currentToken);
			term_prime(currentToken);
			return;
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "23" ) {
				getNextToken(currentToken);
				term_prime(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ) , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "29" ) {
			getNextToken(currentToken);
			term_prime(currentToken);
			return;
		}

	}

	void term_prime(string &currentToken) {
		if( currentToken == "12" ) {
			getNextToken(currentToken);
			factor(currentToken);
			term_prime(currentToken);
			return;
		}

		if( currentToken == "9" ) {
			getNextToken(currentToken);
			factor(currentToken);
			term_prime(currentToken);
			return;
		}

		if( currentToken == "11" || currentToken == "10" || currentToken == "20" || currentToken == "17" ||
			currentToken == "16" || currentToken == "18" || currentToken == "19" || currentToken == "14" ||
			currentToken == "15" || currentToken == "25" || currentToken == "23" || currentToken == "21" ) {
			return;
		} else {
			cout << "Syntax Error: Expected either -, +, ;, <=, <, >, >=, ==, !=, ], ), , , got: " + currentToken << "\n";
		}

	}

	void factor(string &currentToken) {
		if( currentToken == "28" ) {
			getNextToken(currentToken);
			factor_prime(currentToken);
			return;
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "23" ) {
				getNextToken(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ) , got: " + currentToken << "\n";
			}
		}

		if( currentToken == "29" ) {
			getNextToken(currentToken);
			return;
		}
	}

	void factor_prime(string &currentToken) {
		if( currentToken == "24" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "25" ) {
				getNextToken(currentToken);
				return;
			}
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			call_stmt(currentToken);
			return;
		}

		if( currentToken == "12" || currentToken == "9" || currentToken == "11" || currentToken == "10" ||
			currentToken == "17" || currentToken == "16" || currentToken == "18" || currentToken == "19" ||
			currentToken == "14" || currentToken == "15" || currentToken == "20" || currentToken == "25" ||
			currentToken == "23" || currentToken == "21" ) {

			return;
		} else {
			cout << "Syntax Error: Expected either *, /, -, +, <=, <, >, >=, ==, !=, ;, ], ), or , got: " + currentToken << "\n";
		}   
	}

	void call_stmt(string &currentToken) {
		if( currentToken == "28" ) {
			getNextToken(currentToken);
			factor_prime(currentToken);
			term_prime(currentToken);
			arithmetic_expression_prime(currentToken);
			args(currentToken);
			if( currentToken == "23" ) {
				getNextToken(currentToken);
				return;
			} else {
				cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
			}
		}

		if( currentToken == "22" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			if( currentToken == "23" ) {
				getNextToken(currentToken);
				term_prime(currentToken);
				arithmetic_expression_prime(currentToken);
				args(currentToken);
				if( currentToken == "23" ) {
					getNextToken(currentToken);
					return;
				} else {
					cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
				}
			} else {
				cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
			}
		}

		if( currentToken == "29" ) {
			getNextToken(currentToken);
			term_prime(currentToken);
			arithmetic_expression_prime(currentToken);
			args(currentToken);
			if( currentToken == "23" ) {
				getNextToken(currentToken);
				return;
			} 
		}

		if( currentToken == "23" ) {
			getNextToken(currentToken);
			return;
		}

	}

	void args(string &currentToken) {
		if( currentToken == "21" ) {
			getNextToken(currentToken);
			arithmetic_expression(currentToken);
			args(currentToken);
			return;
		}

		if( currentToken == "23") {
			return;
		} else {
			cout << "Syntax Error: Expected ), got: " + currentToken << "\n";
		}
	}	

};




int main() {
	
	LexicalAnalyzer lexicalAnalyzer;

	string filename = "";

	cout << "Enter the name of the textfile: ";
	cin >> filename;

	lexicalAnalyzer.readCharStream(filename);

	string currentString = "";

	SyntaxAnalysis syntaxAnalysis;

	syntaxAnalysis.startSyntaxAnalysis();

	return 0;
}