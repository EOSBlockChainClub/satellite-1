#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;
using std::string;



class Satellite : public eosio::contract {
  using contract::contract;
  account_name dpo_owner;

 public:

   //##################EMPLOYEES RELATED##################//

   /// @abi action
   void add(account_name person,
    string hash_first_name,
    string hash_last_name,
    string hash_address,
    string hash_nationality,
    string hash_gender,
    string hash_martial_status,
    string hash_children_number,
    string hash_blodd_type,
    string hash_email,
    string hash_phone_num) {

     //verification commented for testing purposes
     //require_auth(dpo_owner)

     _employees employees_table(_self,_self);

    const auto& iterator = employees_table.find(person);
    eosio_assert(iterator != employees_table.end(), "Employee is already in the table");

     employees_table.emplace(_self, [&](auto& emp) {
        emp._person = person;
        emp._hash_first_name = hash_first_name;
        emp._hash_last_name = hash_last_name;
        emp._hash_address = hash_address;
        emp._hash_nationality = hash_nationality;
        emp._hash_gender = hash_gender;
        emp._hash_martial_status = hash_martial_status;
        emp._hash_children_number = hash_children_number;
        emp._hash_blodd_type = hash_blodd_type;
        emp._hash_email = hash_email;
        emp._hash_phone_num = hash_phone_num;
     });
     print("Added employee record hash.");

   }

   ///@abi action
   void update(account_name person,
    string hash_first_name,
    string hash_last_name,
    string hash_address,
    string hash_nationality,
    string hash_gender,
    string hash_martial_status,
    string hash_children_number,
    string hash_blodd_type,
    string hash_email,
    string hash_phone_num){

    //TO DO : decide level of clearance
    //require_auth(???)
    _employees employees_table(_self, _self);
    auto iterator = employees_table.find(person);
    eosio_assert(iterator != employees_table.end(), "No such employee in the table");

    employees_table.modify(iterator, person, [&](auto& emp){
    emp._person = person;
    emp._hash_first_name = hash_first_name;
    emp._hash_last_name = hash_last_name;
    emp._hash_address = hash_address;
    emp._hash_nationality = hash_nationality;
    emp._hash_gender = hash_gender;
    emp._hash_martial_status = hash_martial_status;
    emp._hash_children_number = hash_children_number;
    emp._hash_blodd_type = hash_blodd_type;
    emp._hash_email = hash_email;
    emp._hash_phone_num = hash_phone_num;
    });
    print("A record was modified.");
}

   //should remove a specific record
   ///@abi action
   void deleterec(account_name person){
     //require_auth(_person)
     _employees employees_table(_self, _self);
     auto iterator = employees_table.find(person);
     eosio_assert(iterator != employees_table.end(), "No such employee in the table");

     employees_table.erase(iterator);
     print("Employee record deleted.");


   }
   //##################EMPLOYEES RELATED END##################//

   //##################REQUESTS RELATED ##################//

   ///@abi action
   void addreq(account_name from, account_name to, string hashdes, bool sensitive, bool acceptedbydpo){
     _requests requests_tables(_self,_self);
     //auto iterator = requests_tables.find()

     requests_tables.emplace(_self, [&](auto& req) {
       req._from = from;
       req._to = to;
       req._hash_description = hashdes;
       req._sensitive = sensitive;
       req._acceptedbydpo = acceptedbydpo;
     });
     print("New request added.");
   }


   ///@abi action
   void delrequest(account_name from, account_name to, string hashdesc, bool sensitive, bool acceptedbydpo){
     _requests requests_tables(_self,_self);
     for (auto i=requests_tables.begin();i!=requests_tables.end();) {
       if(i->_hash_description==hashdesc && i->_from==from && i->_to==to && i->_sensitive == sensitive && i->_acceptedbydpo == acceptedbydpo){
         requests_tables.erase(i);

       }
       print("Request deleted.");
   }
 }



   //##################REQUESTS RELATED END ##################//






   //remove all records from DB
   ///@abi action
   void dropall(){
     _employees employees_table(_self,_self);
     for (auto i=employees_table.begin();i!=employees_table.end();) {
       i  = employees_table.erase(i);
     }
     print("All data dropped.");


   }


   void printall(){
     _employees employees_table(_self,_self);
     for (auto i=employees_table.begin();i!=employees_table.end();) {
       print(i->_person);
       i++;
     }
   }

 private:

   /// @abi table employee
   //TO DO : add other parameters
   struct employee{
    account_name _person;
    string _hash_first_name;
    string _hash_last_name;
    string _hash_address;
    string _hash_nationality;
    string _hash_gender;
    string _hash_martial_status;
    string _hash_children_number;
    string _hash_blodd_type;
    string _hash_email;
    string _hash_phone_num;


     uint64_t primary_key() const {return _person;}

     EOSLIB_SERIALIZE(employee,
       (_person)(_hash_first_name)(_hash_last_name)
       (_hash_address)(_hash_nationality)
       (_hash_gender)(_hash_martial_status)(_hash_children_number)
       (_hash_blodd_type) (_hash_email)(_hash_email)(_hash_phone_num));

   };
   typedef multi_index<N(employee), employee> _employees;



   ///@abi table requests
   struct request{
     account_name _from;
     account_name _to;
     string _hash_description;
     bool _sensitive;
     bool _acceptedbydpo;

     uint64_t primary_key() const {return string_to_name(_hash_description.c_str());}

     EOSLIB_SERIALIZE(request, (_from)(_to) (_hash_description) (_sensitive) (_acceptedbydpo))
   };
   typedef multi_index<N(request), request> _requests;


};

EOSIO_ABI( Satellite, (add)(update)(deleterec)(addreq)(delrequest)(dropall)(printall))
