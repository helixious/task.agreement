#include <tasks.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

bool contains(const std::vector<string>& array, const string item) {
  bool result = (find(array.begin(), array.end(), item) != array.end());
  return result;
};

uint32_t now() {
  return current_time_point().sec_since_epoch();
};


ACTION tasks::upsertProp(name owner, string title, string description, string contract_hash, string contract_url, uint32_t contract_min, uint32_t contract_max) {
  vector<string> states = {
    "pending",
    "active",
    "inactive"
  };
  // user auth
  require_auth(user);

  // validations
  check(contains(states, state), "invalid state");
  check(!title.empty(), "no title");
  check(!description.empty(), "no description");
  check(!contract_hash.empty(), "no contract_hash");
  check(!contract_url.empty(), "no contract_url");

  const uint32_t date = now();

  // init agreements table
  const name _code = get_self();
  proposal_table _proposals(_code, _code.value);
  auto user_proposals = _proposals.get_index<name("byowner")>();
  auto itr = user_proposals.lower_bound(user.value);
  bool is_new_prop = true;

  while(itr != user_proposals.end()) {
    if(itr->title == title) {
      require_auth(itr->user);
      print("Updating Proposal: ", title);
      user_proposals.modify(itr, user, [&](auto &p) {
        p.description = description;
        p.contract_hash = contract_hash;
        p.contract_url = contract_url;
        p.contract_min = contract_min;
        p.contract_max = contract_max;
        p.last_updated = date;
      });
      is_new_prop = false;
      break;
    }
    itr++;
  };

  if(is_new_prop) {
    print("New Proposal: ", title);
    check(user_proposals.size() < 5, "prop limit reached");
    _proposals.emplace(user, [&](auto &p) {
      p.id = _proposals.available_primary_key();
      p.user = user;
      p.title = title;
      p.description = description;
      p.contract_hash = contract_hash;
      p.contract_url = contract_url;
      p.contract_min = contract_min; // default 10
      p.contract_max = contract_max; // default 250
      p.created = date;
      p.last_updated = date;
    });
  }
};

ACTION tasks:rmProp(name owner, uint64_t id) {
    // init agreements table
  const name _code = get_self();

  proposal_table _proposals(_code, _code.value);
  auto user_proposals = _proposals.get_index<name("byowner")>();
  auto itr = user_proposals.lower_bound(user.value);

  while(itr != user_proposals.end()) {
    if(itr->id == id) {
      require_auth(itr->user);
      user_proposals.erase(itr);
      break;
    }
    itr++;
  };

  check(itr != user_proposals.end(), "invalid proposal id!");

};

    ACTION enrollProp(name user, string type, string state, uint64_t prop_id);
    ACTION crtPropCont(name user, uint64_t prop_id, name user, string type, string state, );



    ACTION viewProps();

    ACTION setContractState(uint64_t id, string state);
    ACTION setPropState(uint64_t id, uint64_t prop_id), string state); // 1) check prop is active, else return, 2) only allow reg if prop is pending, 3) if moved to inactive or active state move, notify users and value producer

    ACTION notify(name receiver, string message);



ACTION tasks::upsert(name user, string title, string description, string contract_hash, string contract_url, string state, uint32_t contract_min = 10, uint32_t contract_max = 250) {
  
  vector<string> states = {
    "pending",
    "active",
    "inactive"
  };

      string description;
      string contract_hash;
      string contract_url;


  // user auth
  require_auth(user);

  // validations
  check(contains(states, state), "invalid state");
  check(!title.empty(), "no title");
  check(!description.empty(), "no description");
  check(!contract_hash.empty(), "no contract_hash");
  check(!contract_url.empty(), "no contract_url");


  const uint32_t date = now();

  print("date: ", date);

  // init agreements table
  const name _code_acct = get_self();
  proposal_table _proposals(_code_acct, _code_acct.value);
  auto user_proposals = _proposals.get_index<name("byuser")>();
  auto itr = user_proposals.lower_bound(user.value);
  bool is_new_prop = true;

  while(itr != user_proposals.end()) {
    if(itr->title == title) {
      require_auth(itr->user);
      print("Updating Proposal: ", title);
      user_proposals.modify(itr, user, [&](auto &p) {
        p.description = description;
        p.contract_hash = contract_hash;
        p.contract_url = contract_url;
        p.contract_min = contract_min;
        p.contract_max = contract_max;
        p.last_updated = date;
      });
      is_new_prop = false;
      break;
    }
    itr++;
  };

  if(is_new_prop) {
    print("New Proposal: ", title);
    check(user_proposals.size() < 5, "prop limit reached");
    _proposals.emplace(user, [&](auto &p) {
      p.id = _proposals.available_primary_key();
      p.user = user;
      p.title = title;
      p.description = description;
      p.contract_hash = contract_hash;
      p.contract_url = contract_url;
      p.contract_min = contract_min; // default 10
      p.contract_max = contract_max; // default 250
      p.created = date;
      p.last_updated = date;
    });
  }

  


  /*

  if(is_new_contract == true) {
    _agreements.emplace(user, [&](auto &a) {
      a.agreement_id = _agreements.available_primary_key();
      a.user = user;
      a.title = title;
      a.description = description;
    });
    //print("Contract created for      :", name(user), "\n");
  }

  */
};


ACTION tasks::remove(name user, uint64_t id) {
  // init agreements table
  const name _account = get_self();
  agreements_table _agreements(_account, _account.value);
  auto user_agreements = _agreements.get_index<name("byuser")>();
  auto itr = user_agreements.lower_bound(user.value);

  while(itr != user_agreements.end()) {
    if(itr->id == id) {
      require_auth(itr->user);
      user_agreements.erase(itr);
      break;
    }
    itr++;
  };

  check(itr != user_agreements.end(), "invalid agreement id!");

};

EOSIO_DISPATCH(tasks, (upsert)(remove));
