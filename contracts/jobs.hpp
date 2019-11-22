#pragma once

#include "dist/contracts/eos/dappservices/multi_index.hpp"

#define DAPPSERVICES_ACTIONS() \
  XSIGNAL_DAPPSERVICE_ACTION \
  IPFS_DAPPSERVICE_ACTIONS

#define DAPPSERVICE_ACTIONS_COMMANDS() \
  IPFS_SVC_COMMANDS()

namespace eosiosystem {
  class system_contract;
}

using std::string;
using namespace eosio;

#define CONTRACT_NAME() clusterjobs

CONTRACT_START()
public:
  [[eosio::action]] void conntoreg(name registry_contract);
  [[eosio::action]] void regworker(name worker, name researcher);
  [[eosio::action]] void delworker(name worker, name researcher);
  [[eosio::action]] void regdisptch(name dispatcher, name researcher);
  [[eosio::action]] void deldisptch(name dispatcher, name researcher);
  [[eosio::action]] void newjob(name researcher, name worker, uint64_t time);
  [[eosio::action]] void removejob(name researcher, name worker, uint64_t time);
  [[eosio::action]] void updatedptch(name researcher, name dispatcher, uint64_t time);
  [[eosio::action]] void payout(name owner, name funder, name researcher, name contributor, string type);
private:
  //from registry contract
  TABLE protocol {
    string title;
    name researcher;
    string datadir;
    name fundaccount;
    asset fund;
    name tokencontract;

    uint64_t primary_key() const { return researcher.value; }
  };
  typedef eosio::multi_index<"protocols"_n, protocol> protocols_t;

  TABLE dispatcher {
    name researcher;
    microseconds total_time;
    microseconds unpaid_time;

    uint64_t primary_key() const { return researcher.value; }
  };
  typedef eosio::multi_index<"dispatchers"_n, dispatcher> dispatchers_t;
  TABLE worker {
    name researcher;
    microseconds total_time;
    microseconds unpaid_time;

    uint64_t primary_key() const { return researcher.value; }
  };
  typedef eosio::multi_index<"workers"_n, worker> workers_t;

  TABLE job {
    name researcher;
    microseconds time;

    uint64_t primary_key() const { return time.count(); }
  };
  typedef dapp::multi_index<"jobs"_n, job> jobs_t;

  typedef eosio::multi_index<".jobs"_n, job> jobs_t_v_abi;
  TABLE shardbucket {
    std::vector<char> shard_uri;
    uint64_t shard;

    uint64_t primary_key() const { return shard; }
  };
  typedef eosio::multi_index<"jobs"_n, shardbucket> jobs_t_abi;

  TABLE reg {
    name registry_contract = "x"_n;

    uint64_t primary_key() const { return 0; }
  };
  typedef eosio::multi_index<"registry"_n, reg> registry_t;

CONTRACT_END((conntoreg)(regworker)(delworker)(regdisptch)(deldisptch)(newjob)(removejob)(updatedptch)(payout))
