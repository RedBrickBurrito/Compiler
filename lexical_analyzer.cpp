#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;

// hash table max size
#define CAPACITY 100000000

// A class that stores the tokenId, content and slot number of a token.
class HashEntry {
	private:
		int tokenId;
		string content;
		int slot;
	public:
		/**
		 * This function is a constructor for the HashEntry class
		 * 
		 * @param tokenId The token id of the token.
		 * @param content The content of the token.
		 * @param slot The slot number of the hash table.
		 */
		HashEntry(int tokenId, string content, int slot) {
			this->tokenId = tokenId;
			this->content = content;
			this->slot = slot;
		}

		/**
		 * This function returns the tokenId
		 * 
		 * @return The tokenId
		 */
		int getTokenId() {
			return tokenId;
		}

		/**
		 * This function returns the content of the string stored
		 * 
		 * @return The content of the the string stored.
		 */
		string getContent() {
			return content;
		}

		/**
		 * It returns the value of the slot variable.
		 * 
		 * @return The slot number of the item.
		 */
		int getSlot() {
			return slot;
		}
};

/* This class is used to store the symbol table. It uses a hash table to store the symbol table */
class UnorderedHashTable {
	private:
		/* Creating a pointer to a pointer to a HashEntry. */
		HashEntry **table;

		/**
		 * The function below takes in a tokenId and an Id and returns a hash value.
		 * 
		 * @param tokenId The token id of the token that is being hashed.
		 * @param Id The string that we want to hash.
		 * 
		 * @return The hash value of the tokenId and Id.
		 */
		unsigned long long createHash(int tokenId, string Id) {
			string asciiCode = "";

			// iterate and transform each character in string to ascii code
			for(int i = 0; i < Id.length(); i++) {
				asciiCode += to_string(int(Id[i]));
				if(asciiCode.length() > 14) {
					break;
				}
			}

			// Use divide method ; h(k) = k mod m
			string tempK = to_string(tokenId) + asciiCode;
			long long k = stoll(tempK);
			unsigned long long hash = k % CAPACITY; 

			return hash;
		}

		/**
		 * If the hash is the same, increase the hash by 1, and mod
		 * 
		 * @param hash the hash value of the token
		 * @param tokenId the tokenId of the token you want to find.
		 * 
		 * @return The hash value of the tokenId.
		 */
		unsigned long long checkCollisions(long long &hash, int tokenId) {

			while(table[hash] != nullptr && table[hash]->getTokenId() != tokenId) {
				hash = (hash + 1) % CAPACITY;
			}

			return hash;
		}

	public:
		/**
		 * It creates a new hash table with a capacity of CAPACITY.
		 */
		UnorderedHashTable() {
			table = new HashEntry*[CAPACITY];

			for(int i = 0; i < CAPACITY; i++) {
				table[i] = nullptr;
			}
		}

		/**
		 * It takes in a tokenId and a content and creates a hash from them. If the hash is not null, it
		 * returns the slot of the hash. Otherwise, it returns a string saying that there is no such entry in
		 * the symbol table
		 * 
		 * @param tokenId The token id of the token you want to get the slot of.
		 * @param content The content of the token
		 * 
		 * @return The slot of the entry in the symbol table.
		 */
		string getSlot(int tokenId,string content) {
			long long hash = createHash(tokenId,content);

			if(table[hash] != nullptr) {
				return to_string(table[hash]->getSlot());
			} else {
				return "There is no such entry in the symbol table";
			}
		}

		/**
		 * It returns the content of the entry with the given tokenId and content.
		 * 
		 * @param tokenId The token id of the token you want to get the content of.
		 * @param content the content of the token
		 * 
		 * @return The content of the entry in the symbol table.
		 */
		string getContent(int tokenId, string content) {
			long long hash = createHash(tokenId, content);

			if(table[hash] != nullptr) {
				return table[hash]->getContent();
			} else {
				return "There is no such entry in the symbol table";
			}
		}

		/**
		 * This function takes in a tokenId and a content and returns the tokenId of the entry in the symbol
		 * table that matches the tokenId and content
		 * 
		 * @param tokenId The token id of the token you want to get the token id of.
		 * @param content The content of the token
		 * 
		 * @return The tokenId of the entry in the symbol table.
		 */
		string getTokenId(int tokenId,string content) {
			long long hash = createHash(tokenId,content);

			if(table[hash] != nullptr) {
				return to_string(table[hash]->getTokenId());
			} else {
				return "There is no such entry in the symbol table";
			}
		}

		/**
		 * It creates a hash for the tokenId and content and then checks for collisions. If there is a
		 * collision, it will delete the old entry and create a new one.
		 * 
		 * @param tokenId The token id of the token.
		 * @param content the string that is being hashed
		 * @param slot the slot in the table that the entry is being inserted into.
		 */
		void insert(int tokenId, string content,int slot) {
			long long hash = createHash(tokenId, content);

			checkCollisions(hash, slot);

			if(table[hash] != nullptr) {
				delete table[hash];
			}

			table[hash] = new HashEntry(tokenId,content, slot);
		}
};

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
		UnorderedHashTable identifiersSymbolTable;
		UnorderedHashTable numbersSymbolTable;
		UnorderedHashTable specialSymbolsTable;
		int errorLine = 1;
		// Id of tokens, as defined in the analysis section
		int tokenId = 28;
		int numberId = 29;
		int commentId = 30;
		// id index starts at 8, because of reserved words
		int idIndex = 9;
		int numberIndex = 1;
		int symbolIndex = 1;
		const int previousStates = 4;
		// file to write the scanner outputs
		ofstream scannerOutput;
	public:

		/**
		 * The constructor of the LexicalAnalyzer class initializes the reserved keywords in the symbol table
		 */
		LexicalAnalyzer() {
			
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
					cout << identifiersSymbolTable.getSlot(tokenId,lowerCaseId) << "\n";
					scannerOutput << identifiersSymbolTable.getSlot(tokenId,lowerCaseId) << "\n";
				} else {
					// if the id is already stored in the table
					if(id == identifiersSymbolTable.getContent(tokenId, id)) {
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
				cout << to_string(commentId) << "\n";
				scannerOutput << "30" << "\n";
				break;
			case 13:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 14:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 15:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 16:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 17:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 18:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 19:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 20:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 21:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 22:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 23:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 24:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 25:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 26:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 27:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 28:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 29:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 30:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
				break;
			case 31:
				if(specialSymbol == specialSymbolsTable.getContent(state - previousStates, specialSymbol)) {
					printAcceptorsOutput(state - previousStates, specialSymbol);
					break;
				}
				specialSymbolsTable.insert(state - previousStates,specialSymbol, symbolIndex);
				printAcceptorsOutput(state - previousStates, specialSymbol);
				symbolIndex++;
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
				cout << tokenId << " , " << identifiersSymbolTable.getSlot(tokenId, content) << "\n";
				scannerOutput << tokenId << " , " << identifiersSymbolTable.getSlot(numberId, content) << "\n";
				return;
			} else if (numberId == 29) {
				cout << numberId << " , " << numbersSymbolTable.getSlot(numberId, content) << "\n";
				scannerOutput << numberId << " , " << numbersSymbolTable.getSlot(numberId, content) << "\n";
				return;
			} else if( specialSymbolId >= 9 && specialSymbolId <= 27) {
				cout << specialSymbolsTable.getTokenId(specialSymbolId,content) << "\n";
				scannerOutput << specialSymbolsTable.getTokenId(specialSymbolId,content) << "\n";
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