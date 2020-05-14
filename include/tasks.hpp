#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

CONTRACT tasks : public contract {
  public:
    using contract::contract;

    /*

    ACTION upsert_prop(name owner, string title, string description, string contract_hash, string contract_url, uint32_t contract_min, uint32_t contract_max);
    ACTION remove_prop(name owner, uint64_t id);

    ACTION upsert_cont(name user, string type, string state, uint64_t prop_id);
    ACTION remove_cont(name owner, uint64_t prop_id);
    */

    ACTION upsertProp(name owner, string title, string description, string contract_hash, string contract_url, uint32_t contract_min, uint32_t contract_max);
    ACTION rmProp(name owner, uint64_t id);

    ACTION enrollProp(name user, string type, string state, uint64_t prop_id);
    ACTION crtPropCont(name user, uint64_t prop_id, name user, string type, string state, );



    ACTION viewProps();

    ACTION setContractState(uint64_t id, string state);
    ACTION setPropState(uint64_t id, uint64_t prop_id), string state); // 1) check prop is active, else return, 2) only allow reg if prop is pending, 3) if moved to inactive or active state move, notify users and value producer

    ACTION notify(name receiver, string message);
  private:
  /*
    TABLE agreement {
      uint64_t id;
      name owner;
      string title;
      string description;
      string contract_hash;
      string contract_url;
      uint32_t created;
      uint32_t last_updated;
      
      auto primary_key() const { return id; };
      uint64_t get_owner() const { return owner.value; };

      EOSLIB_SERIALIZE(agreement, (id) (owner) (title));
    };
    typedef multi_index<name("agreement"), agreement, indexed_by<name("byowner"), const_mem_fun<agreement, uint64_t, &agreement::get_owner>>> agreements_table;
    */

    TABLE proposal { // a user is limited to 5 active proposals
      uint64_t id;
      name owner;
      string title;
      string description;
      string contract_hash;
      string contract_url;
      string state; //pending, active, inactive
      string escrow_default_state; // pending, approved, hold 
      uint32_t contract_min;
      uint32_t contract_max;
      uint32_t created;
      uint32_t last_updated;
      
      auto primary_key() const { return id; };
      uint64_t get_owner() const { return owner.value; };

      EOSLIB_SERIALIZE(proposal, (id) (owner) (title));
    };
    typedef multi_index<name("proposal"), proposal, indexed_by<name("byowner"), const_mem_fun<proposal, uint64_t, &proposal::get_owner>>> proposal_table;

    TABLE proposal_contract {
      uint64_t id;
      uint64_t prop_id;
      name user;
      string type; // admin, user, proxy 
      string state; // pending, active, inactive, rejected
      string last_modifier; // 1) only admin and proxy can change states, 2) if last modified is admin and state, proxy wont be able to make changes
      uint32_t balance;
      uint32_t created; // date
      uint32_t last_updated; // date

      auto primary_key() const { return id; };
      uint64_t get_id() const { return id; };

      EOSLIB_SERIALIZE(proposal_contract, (id) (prop_id) (user) (type) (state));
    };
    typedef multi_index<name("propcontract"), proposal_contract, indexed_by<name("byid"), const_mem_fun<proposal_contract, uint64_t, &proposal_contract::get_id>>> contract_prop_table;

    TABLE proposal_escrow {
      uint64_t id;
      uint64_t cont_id;
      name verifier; //admin or proxy 
      string balance;
      string state; //pending, approved, rejected or hold
      string memo;
      uint32_t created;
      uint32_t last_modified;

      auto primary_key() const { return id; };
      uint64_t get_id() const {return id; };
      
    }
    
};