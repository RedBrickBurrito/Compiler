#ifndef UNORDERED_HASH_TABLE_H
#define UNORDERED_HASH_TABLE_H

#include <iostream>
#include <string.h>

using namespace std;

// hash table max size
#define CAPACITY 100000000

// A class that stores the tokenId, content and slot number of a token.
class HashEntry {
	private:
		int tokenId;
		string content;
		int slot;
		int numberOfArgs = 0;
		bool variable;
		bool function;
		bool global;
		bool local;
	public:
		/**
		 * This function is a constructor for the HashEntry class
		 * 
		 * @param tokenId The token id of the token.
		 * @param content The content of the token.
		 * @param slot The slot number of the hash table.
		 */
		HashEntry(int tokenId, string content, int slot, int numberOfArgs , bool variable, bool function, bool global, bool local) {
			this->tokenId = tokenId;
			this->content = content;
			this->slot = slot;
			this->numberOfArgs = numberOfArgs;
			this->variable = variable;
			this->function = function;
			this->global = global;
			this->local = local;
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

		/**
		 * It returns the value of the number of args.
		 * 
		 * @return The number of args.
		 */
		int getNumberOfArgs() {
			return numberOfArgs;
		}

		/**
		 * This function returns the value of the variable property
		 * 
		 * @return The variable property of the class.
		 */
		bool getVariableProperty() {
			return variable;
		}

		/**
		 * This function returns a boolean value that is true if the function is a function, and false if it
		 * is not a function
		 * 
		 * @return The function property of the object.
		 */
		bool getFunctionProperty() {
			return function;
		}

		/**
		 * This function returns the value of the global property
		 * 
		 * @return The value of the global variable.
		 */
		bool getGlobalProperty() {
			return global;
		}

		/**
		 * This function returns the value of the local property
		 * 
		 * @return The local variable is being returned.
		 */
		bool getLocalProperty() {
			return local;
		}

		void setNumberOfArgs(int numberOfArgs) {
			this->numberOfArgs = numberOfArgs;
		}

		/**
		 * This function sets the variable property of the object to the value of the variable parameter
		 * 
		 * @param variable If true, the property is a variable. If false, it's a constant.
		 */
		void setVariableProperty(bool variable) {
			this->variable = variable;
		}

		/**
		 * This function sets the function property of the object to the value of the function parameter
		 * 
		 * @param function Whether or not the property is a function.
		 */
		void setFunctionProperty(bool function) {
			this->function = function;
		}

		/**
		 * It sets the global property of the class.
		 * 
		 * @param global If true, the property will be set on the global object. If false, it will be set on
		 * the current object.
		 */
		void setGlobalProperty(bool global) {
			this->global = global;
		}

		/**
		 * This function sets the local property of the object to the value of the local parameter
		 * 
		 * @param local If true, the property is local to the object. If false, the property is global.
		 */
		void setLocalProperty(bool local) {
			this->local = local;
		}

};

/* This class is used to store the symbol table. It uses a hash table to store the symbol table */
class UnorderedHashTable {
	private:
		int index = 0;
		/* Create two array, one for storing the hashes, and the other for storing the content string */
		long long hashArr[1000];
		string contentArr[1000];
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
		void insert(int tokenId, string content,int slot,int numberOfArgs , bool variable, bool function, bool global, bool local ) {
			long long hash = createHash(tokenId, content);
			hashArr[index] = hash;
			contentArr[index] = content;

			checkCollisions(hash, slot);

			if(table[hash] != nullptr) {
				delete table[hash];
			}

			table[hash] = new HashEntry(tokenId,content, slot, numberOfArgs, variable, function, global, local );
			index++;
		}

		/**
		 * It prints the hash table
		 * 
		 */
		void printTable() {
			int arraySize = sizeof(hashArr)/sizeof(hashArr[0]);
			cout << "------------------- ID'S HASH TABLE ---------------------------" << "\n";
			for(int i = 0; i < arraySize; i++) {
				// check if we have reached the end of the hash table hash numbers
				if(hashArr[i] == 0) {
					return;
				}

				cout << table[hashArr[i]]->getContent() << " 	--> " << "Variable : " << table[hashArr[i]]->getVariableProperty() <<
						" " << "Function : " << table[hashArr[i]]->getFunctionProperty() << " " << "Global : " << table[hashArr[i]]->getGlobalProperty() <<
						" " << "Local : " << table[hashArr[i]]->getLocalProperty() << " " << "Args : " << table[hashArr[i]]->getNumberOfArgs() << "\n";
			}
		}

		/**
		 * This function checks if the current token is the main keyword
		 * 
		 * @param index the index of the token in the contentArr
		 * 
		 * @return A boolean value.
		 */
		bool getMainKeyword(int index) {
			int tokenId = 28;
			long long hash = createHash(tokenId, contentArr[index]);

			if(table[hash] != nullptr) {
				string main = table[hash]->getContent();
				if(main == "main") {
					return true;
				}
			}

			return false;
		}

		/**
		 * This function sets the number of args property value
		 * 
		 * @param index the index of the content in the contentArr
		 * @param numberOfArgs number of args
		 */
		void setNumberOfArgsProperty(int index, int numberOfArgs) {
			int tokenId = 28;
			long long hash = createHash(tokenId, contentArr[index]);

			if(table[hash] != nullptr) {
				table[hash]->setNumberOfArgs(numberOfArgs);
			}	
		}

		/**
		 * This function sets the variable property of a variable in the symbol table
		 * 
		 * @param index the index of the content in the contentArr
		 * @param variable true if the variable is a variable, false if it's a constant
		 */
		void setVariableProperty(int index, bool variable) {
			int tokenId = 28;
			long long hash = createHash(tokenId, contentArr[index]);

			if(table[hash] != nullptr) {
				table[hash]->setVariableProperty(variable);
			}	
		}

		/**
		 * This function sets the function property of a symbol table entry
		 * 
		 * @param index the index of the token in the contentArr
		 * @param function true if the identifier is a function, false if it's a variable
		 */
		void setFunctionProperty(int index, bool function) {
			int tokenId = 28;
			long long hash = createHash(tokenId, contentArr[index]);

			if(table[hash] != nullptr) {
				table[hash]->setFunctionProperty(function);
			}	
		}

		/**
		 * This function sets the global property of a variable
		 * 
		 * @param index the index of the content in the contentArr
		 * @param global true if the variable is global, false if it's local
		 */
		void setGlobalProperty(int index, bool global) {
			int tokenId = 28;
			long long hash = createHash(tokenId, contentArr[index]);

			if(table[hash] != nullptr) {
				table[hash]->setGlobalProperty(global);
			}	
		}

		/**
		 * It sets the local property of the variable at the given index to the given value.
		 * 
		 * @param index the index of the content in the contentArr
		 * @param local true if the variable is local, false if it's global
		 */
		void setLocalProperty(int index, bool local) {
			int tokenId = 28;
			long long hash = createHash(tokenId, contentArr[index]);

			if(table[hash] != nullptr) {
				table[hash]->setLocalProperty(local);
			}	
		}
};

#endif