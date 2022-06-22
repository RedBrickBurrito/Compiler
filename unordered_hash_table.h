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

#endif