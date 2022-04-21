#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;

// hash table max size
#define CAPACITY 100000


class HashEntry {
	private:
		int tokenId;
		string content;
		int slot;
	public:
		HashEntry(int tokenId, string content, int slot) {
			this->tokenId = tokenId;
			this->content = content;
			this->slot = slot;
		}

		int getTokenId() {
			return tokenId;
		}

		string getContent() {
			return content;
		}

		int getSlot() {
			return slot;
		}
};

class UnorderedHashTable {
	private:
		HashEntry **table;

		unsigned long long createHash(int tokenId, int Index) {
			// Use divide method ; h(k) = k mod m
			int k = tokenId + Index;
			unsigned long long hash = k % CAPACITY; 

			return hash;
		}

		unsigned long long checkCollisions(long long &hash, int tokenId) {
			// if hash is the same, increase hash by 1, and mod.
			while(table[hash] != nullptr && table[hash]->getTokenId() != tokenId) {
				hash = (hash + 1) % CAPACITY;
			}

			return hash;
		}

	public:
		UnorderedHashTable() {
			table = new HashEntry*[CAPACITY];

			for(int i = 0; i < CAPACITY; i++) {
				table[i] = nullptr;
			}
		}

		string getSlot(int tokenId,int idIndex) {
			long long hash = createHash(tokenId,idIndex);

			if(table[hash] != nullptr) {
				return to_string(table[hash]->getSlot());
			} else {
				return "There is no such entry in the symbol table";
			}
		}

		string getTokenId(int tokenId,int idIndex) {
			long long hash = createHash(tokenId,idIndex);

			if(table[hash] != nullptr) {
				return to_string(table[hash]->getTokenId());
			} else {
				return "There is no such entry in the symbol table";
			}
		}

		void insert(int tokenId, string content,int slot) {
			long long hash = createHash(tokenId, slot);

			checkCollisions(hash, slot);

			// if it is already data, delete the old entry.
			if(table[hash] != nullptr) {
				delete table[hash];
			}

			table[hash] = new HashEntry(tokenId,content, slot);
		}
};

class LexicalAnalyzer {
	private:
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
		UnorderedHashTable identifiersSymbolTable;
		UnorderedHashTable numbersSymbolTable;
		UnorderedHashTable specialSymbolsTable;
		int errorLine = 1;
		int tokenId = 28;
		int numberId = 29;
		int commentId = 30;
		// id index starts at 8, because of reserved words
		int idIndex = 9;
		int numberIndex = 1;
		int symbolIndex = 1;
		// file to write the scanner outputs
		ofstream scannerOutput;
	public:

		LexicalAnalyzer() {

			// initliaze reserved keywords in symbolTable
			identifiersSymbolTable.insert(tokenId,"if", 		1);
			identifiersSymbolTable.insert(tokenId,"else", 		2);
			identifiersSymbolTable.insert(tokenId,"int", 		3);
			identifiersSymbolTable.insert(tokenId,"void", 		4);
			identifiersSymbolTable.insert(tokenId,"return", 	5);
			identifiersSymbolTable.insert(tokenId,"while", 		6);
			identifiersSymbolTable.insert(tokenId,"input", 		7);
			identifiersSymbolTable.insert(tokenId,"output", 	8);

			// create new text file named scannerOutput, where the result of the scanner will be written
			scannerOutput.open("scannerOutput.txt");
		}

		int checkAcceptorsState(int state, string &id, string &number, string &specialSymbol) {

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
					cout << identifiersSymbolTable.getSlot(tokenId,tempIndexId) << "\n";
					scannerOutput << identifiersSymbolTable.getSlot(tokenId,tempIndexId) << "\n";
				} else {
					identifiersSymbolTable.insert(tokenId ,id, idIndex);
					cout << tokenId << " , " << identifiersSymbolTable.getSlot(tokenId, idIndex) << "\n";
					scannerOutput << tokenId << " , " << identifiersSymbolTable.getSlot(tokenId, idIndex) << "\n";
					idIndex++;
				}
				break;
			case 11:
				numbersSymbolTable.insert(numberId ,number, numberIndex);
				cout << numberId << " , " << numbersSymbolTable.getSlot(numberId, numberIndex) << "\n";
				scannerOutput << numberId << " , " << numbersSymbolTable.getSlot(numberId, numberIndex) << "\n";
				numberIndex++;
				break;
			case 12:
				cout << to_string(commentId) << "\n";
				scannerOutput << "30" << "\n";
				break;
			case 13:
				specialSymbolsTable.insert(9,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(9,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(9,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 14:
				specialSymbolsTable.insert(10,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(10,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(10,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 15:
				specialSymbolsTable.insert(11,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(11,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(11,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 16:
				specialSymbolsTable.insert(12,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(12,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(12,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 17:
				specialSymbolsTable.insert(13,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(13,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(13,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 18:
				specialSymbolsTable.insert(14,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(14,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(14,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 19:
				specialSymbolsTable.insert(15,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(15,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(15,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 20:
				specialSymbolsTable.insert(16,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(16,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(16,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 21:
				specialSymbolsTable.insert(17,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(17,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(17,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 22:
				specialSymbolsTable.insert(18,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(18,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(18,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 23:
				specialSymbolsTable.insert(19,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(19,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(19,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 24:
				specialSymbolsTable.insert(20,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(20,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(20,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 25:
				specialSymbolsTable.insert(21,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(21,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(21,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 26:
				specialSymbolsTable.insert(22,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(22,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(22,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 27:
				specialSymbolsTable.insert(23,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(23,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(23,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 28:
				specialSymbolsTable.insert(24,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(24,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(24,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 29:
				specialSymbolsTable.insert(25,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(25,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(25,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 30:
				specialSymbolsTable.insert(26,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(26,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(26,symbolIndex) << "\n";
				symbolIndex++;
				break;
			case 31:
				specialSymbolsTable.insert(27,specialSymbol, symbolIndex);
				cout << specialSymbolsTable.getTokenId(27,symbolIndex) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(27,symbolIndex) << "\n";
				symbolIndex++;
				break;
			} 

			id = "";
			number = "";
			specialSymbol = "";
			return 0;
		}

		bool checkErrorsState(int state, int errorLine) {

			switch(state) {
				case 32:
					cout << "Id error in line " << errorLine << "\n";
					scannerOutput << "Id error in line " << errorLine << "\n";
					return true;
					break;
				case 33:
					cout << "Number error in line " << errorLine << "\n";
					scannerOutput << "Number error in line " << errorLine << "\n";
					return true;
					break;
				case 34:
					cout << "Special Symbol error in line " << errorLine << "\n";
					scannerOutput << "Special Symbol error in line " << errorLine << "\n";
					return true;
					break;
				case 35:
					cout << "Unknown character error in line " << errorLine << "\n";
					scannerOutput << "Unknown character error in line " << errorLine << "\n";
					return true;
					break;
			}

			return false;
		}
		
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

		bool checkIfCharIsLetter(char ch) {
			string alphabet = "ABCEDFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

			size_t found = alphabet.find(ch);
			if(found != string::npos) {
				return true;
			}

			return false;
		}

		bool checkIfCharIsNumber(char ch) {
			string numbers = "0123456789";

			size_t found = numbers.find(ch);
			if(found != string::npos) {
				return true;
			}

			return false;
		}

		bool checkIfCharIsSpecialSymbol(char ch, int &row) {
			string specialSymbols = "+-*/=!<>;,()[]{}";

			size_t found = specialSymbols.find(ch);
			if(found != string::npos) {
				// sum by 3 because transition table, starts first with letters, digits and whitespace
				row = found + 3;
				return true;
			}

			return false;
		}

		bool checkIfCharIsWhitespace(char ch, int &row) {
			string whitespace = " \n\t";

			size_t found = whitespace.find(ch);
			if(found != string::npos) {
				row = 2;
				return true;
			}

			return false;
		}


		string readCharStream(string fileName) {

			FILE* file = fopen(fileName.c_str(), "r");
			string id = "", number = "", specialSymbol = "";

			char ch;
			int state = 0;
			int row = 0;
			bool isInAcceptorsTable = false;
			bool isInErrorsTable = false;
			int commentErrorLine;

			ch = getc(file);

			// loop while we don't reach the end of the file
			while(!feof(file)) {

				while(isInAcceptorsTable != true && isInErrorsTable != true) {

					if(checkIfCharIsLetter(ch)) {
						row = 0;
						id += ch;
					}

					if(checkIfCharIsNumber(ch)) {
						row = 1;
						number += ch;
					}

					if(checkIfCharIsSpecialSymbol(ch, row)) {
						specialSymbol += ch;
					}

					checkIfCharIsWhitespace(ch,row);

					// check if is an uknown character
					if(!checkIfCharIsLetter(ch) && !checkIfCharIsNumber(ch) && !checkIfCharIsSpecialSymbol(ch, row) && !checkIfCharIsWhitespace(ch, row)) {
						row = 19;
					}

					// count where the error line appears
					if(ch == '\n') {
						errorLine++;
					}
					
					// if we reach the end of the file and we are in a comment
					if(feof(file) && state == 4) {
						cout << "Comment not closed in line " << commentErrorLine << "\n";
						return "\n";
					}

					if(feof(file)) {
						return "\n";
					}

					state = transitionTable[state][row];

					switch (state)
					{
					case 0:
						ch = getc(file);
						break;
					case 1:
						if(state == 10) {
							isInAcceptorsTable = true;
						} else if(state == 32) {
							isInErrorsTable = true;
						}
						ch = getc(file);
						break;
					case 2:
						if(state == 11) {
							isInAcceptorsTable = true;
						} else if(state == 33) {
							isInErrorsTable = true;
						}
						ch = getc(file);
						
						break;
					case 3:
						if(state == 13) {
							isInAcceptorsTable = true;
						} else if(state == 35) {
							isInErrorsTable = true;
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
							isInAcceptorsTable = true;
						} else if(state == 34) {
							isInErrorsTable = true;
						}
						ch = getc(file);
						break;
					case 7:
						if(state == 19) {
							isInAcceptorsTable = true;
						} else if(state == 34) {
							isInErrorsTable = true;
						}
						ch = getc(file);
						break;
					case 8:
						if(state == 20 | state == 21) {
							isInAcceptorsTable = true;
						} else if(state == 34) {
							isInErrorsTable = true;
						}
						ch = getc(file);
						break;
					case 9:
						if(state == 22 | state == 23) {
							isInAcceptorsTable = true;
						} else if(state == 34) {
							isInErrorsTable = true;
						}
						ch = getc(file);
						break;
					case 10:
						if(checkIfCharIsLetter(ch)) {
							isInAcceptorsTable = true;
							ch = getc(file);
							break;
						}
						isInAcceptorsTable = true;
						break;
					case 11:
						if(checkIfCharIsNumber(ch)) {
							isInAcceptorsTable = true;
							ch = getc(file);
							break;
						}
						isInAcceptorsTable = true;
						break;
					case 12:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 13:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 14:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 15:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 16:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 17:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 18:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 19:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 20:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 21:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 22:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 23:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 24:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 25:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 26:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 27:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 28:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 29:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 30:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 31:
						ch = getc(file);
						isInAcceptorsTable = true;
						break;
					case 32:
						isInErrorsTable = true;
						break;
					case 33:
						isInErrorsTable = true;
						break;
					case 34:
						isInErrorsTable = true;
						break;
					case 35:
						isInErrorsTable = true;
						break;
					} // end of switch case
				} // end of while
				
				if(isInAcceptorsTable == true) {

					checkAcceptorsState(state,id, number, specialSymbol);

					state = 0;
					isInAcceptorsTable = false;
				} else if(isInErrorsTable == true) {

					checkErrorsState(state, errorLine);

					state = 0;
					isInErrorsTable = false;
					return "\n";
				}

			} // end of while

			// close the file to prevent corruption
			scannerOutput.close();
			return "\n";
		}
};


int main() {
	
	LexicalAnalyzer lexicalAnalyzer;

	string filename = "";

	cout << "Enter the name of the textfile: ";
	cin >> filename;

	lexicalAnalyzer.readCharStream(filename);

	return 0;

}
