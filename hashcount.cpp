#include "stdio.h"
#include "string"
#include "iostream"
#include "fstream"
using namespace std;



class HashTable { 
protected:
  
    struct HashNode {
        const char *value;
        struct HashNode *next;
    };

#define HASH_TABLE_SIZE 16

    int table_size;
    struct HashNode *hashtab[HASH_TABLE_SIZE];

    int hash(int len, const char *str) {
           int hashval = 0;

           // Naive. Can use FTN-1 for bewtter results
           for ( int i = 0; i < len; i++ )
           {
               hashval ^= str[i];
           }

           hashval %= HASH_TABLE_SIZE;
       };
          

public:
    int num_collisions;

    HashTable() { 
         num_collisions = 0;
         for (int i = 0; i< HASH_TABLE_SIZE; i++) {  
              hashtab[i] = NULL;
         }
      };

    ~HashTable() { };

    // Add a copy of the string to the hash table 
    void add(const string& str) {
         struct HashNode *pnode = new struct HashNode;
         int hval = hash(str.length(), str.c_str());

         pnode->next  = NULL;
         pnode->value = string(str).c_str();

         if (!hashtab[hval]) 
         {
             hashtab[hval] = pnode;
         }
         else {
             struct HashNode *thisnode = hashtab[hval];

             num_collisions++;
             while( thisnode->next) thisnode = thisnode->next;
             thisnode->next = pnode;
         }
    }
         
    // True if the string is present in the hash table
    bool contains(const string &str) {
         int hval = hash(str.length(), str.c_str());

         if (!hashtab[hval]) 
         {
             return false;
         }
         else {
             struct HashNode *thisnode = hashtab[hval];

             while( thisnode) {

                 if (str.compare(thisnode->value) == 0) {
                    return true;
                 }
                 thisnode = thisnode->next;
             }
         }
         return false;
    }


    // Gets the hash value of the string if it is present in the table
    int get(const string& str);
};




int main(int argc, char *argv[])

{ std::string word;
   HashTable htab;

   ifstream fin;
   fin.open(argv[1]);
    //do {
    while (std::getline(fin, word)) {
        fin >> word ;
        fin.get();

        cerr << "read word " << word << endl;

        if (!htab.contains(word)) htab.add(word);
    } //while (!word.empty());
    
    cout << "Number of Collisions: " << htab.num_collisions << endl;
}

