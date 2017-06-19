#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Contact {

   public:
      Contact() {} ;
      ~Contact() {};

      std::string &Name() {
          return this->name;
       };
  
      std::string &Name( std::string name) {
          this->name = name;
          return this->name;
      };

      std::string &PhoneNumber() {
          return this->num;
      };

      std::string &PhoneNumber(std::string num) {
          this->num = num;
          return this->num;
      };

      class Contact &operator+=(std::string name) {
          p_next = new class Contact;
          (void)p_next->Name(name);
          return *p_next;    
      }
      
   private:
      std::string name;
      std::string num;
      class Contact *p_next;

      friend class PhoneBook;
};


class PhoneBook {
    public:

      PhoneBook() {
          p_first = NULL;
          p_last  = NULL;
      } ;

      ~PhoneBook() {

          for (class Contact *pcontact = p_first; 
              pcontact != NULL;
              pcontact = pcontact->p_next ) {
          }
      }; 

      // Given the name, return a phone number
      const std::string &Lookup(std::string name) {
         const static std::string empty;

         for (class Contact *pcontact = p_first; 
              pcontact != NULL;
              pcontact = pcontact->p_next ) {

             if (pcontact->Name().compare(name) == 0) {
                 return pcontact->PhoneNumber();
             }
         }
         return empty;
      };

      bool Add(std::string name, std::string phno) {
          if (!p_first) {
             p_first = new Contact;
             p_first->Name(std::string(name));
             p_last = p_first;
          }
          else {
             *p_last += std::string(name);
          }
          p_last->PhoneNumber(std::string(phno));
      };

      bool Delete(std::string name, std::string phno) {

         if (!p_first) {
             return true;
         }
         if (p_first->Name().compare(name) == 0) {
             if (p_first == p_last ) {
                 delete p_first;
                 p_first = NULL;
                 p_last  = NULL;
                 return true;
             }
             delete p_first; 
             p_first = p_first->p_next;
             return true;
         }

         for (class Contact *pcontact = p_first; 
              pcontact->p_next != NULL;
              pcontact = pcontact->p_next ) {
           
             if (pcontact->p_next->Name().compare(name) == 0) {
                 class Contact *pdel = pcontact->p_next;
                 pcontact->p_next = pcontact->p_next->p_next;
                 delete pdel;
                 return true;
             }
         }
         return false;
      };

      static void List(std::ostream &fout, class PhoneBook &phbook) {

         for (class Contact *pcontact = phbook.p_first; 
              pcontact != NULL;
              pcontact = pcontact->p_next ) {
             fout << "Name : " << pcontact->Name() << std::endl;
             fout << "Phone: " << pcontact->PhoneNumber() << std::endl;
         }
      }

    private:
      class Contact *p_first;
      class Contact *p_last;
};


int main(int argc, char *argv[])

{ bool done = false;
  std::string cmd;
  std::string name;
  std::string phone_number;
  class PhoneBook phone_book;
  ifstream fin("phonebook.txt", ios::in);

  std::string line;

  
   if (!fin.is_open()) {
      cerr << "could not read contact info in phonebook.txt" << endl;
   }
   else {
       while (std::getline(fin, line)) {
           size_t colon_pos = line.find(":");
           string attr  = line.substr(0,colon_pos);
cerr << "read " << line << endl;
           if (attr.compare("name") == 0) {
               name = line.substr(colon_pos+1);
           }
           else if (attr.compare("phone") == 0) {
               phone_number = line.substr(colon_pos+1);
           }
           
           if (!name.empty() && !phone_number.empty()) {
               phone_book.Add(name, phone_number);
               name.clear();
               phone_number.clear();
           }
       }
   }

   cout << "*** MY PHONEBOOK APPLICATION ***" << endl;

   while (!done) {

      cout << "A(dd) | S(earch) | D(elete) | L(ist) | Q(uit): ";
      cin  >> cmd;

      // We will work with an input string that starts wit hthe right char. So, "armadillo" will 
      // be a command that calls phonebook.add().

      switch (cmd.c_str()[0]) {
      case 'a':
      case 'A':
          cout << "enter name: "; 
          cin  >> name;
 
          cout << "enter phone number: "; 
          cin  >> phone_number;

          phone_book.Add(name, phone_number);
          break;

      case 's':
      case 'S':
          cout << "enter name: "; 
          cin  >> name;
 
          phone_number = phone_book.Lookup(name);
          if (phone_number.empty()) {
              cerr << "name " << name << " not found" << endl;
          }
          else {
              cout << "Phone Number: " << phone_number << endl;
          }
          break;

      case 'd':
      case 'D':
          cout << "enter name: "; 
          cin  >> name;
 
          // Delete doesn't care about the phone number but we pass it for future 
          // enhancement

          if (!phone_book.Delete(name, phone_number)) {
              cerr << "name " << name << " not found" << endl;
          }
          else {
              cout << "name " << name << " deleted" << endl;
          }

          break;

      case 'l':
      case 'L':
          PhoneBook::List(cout, phone_book);

          break;

      case 'q':
      case 'Q':
          done = true;
          break;

      default:
          cerr << "Invalid command " << endl;
          break;
      }

      cin.get();  // Clean out the input buffer

   }

    
}
