#pragma once

#include <string>
#include <set>
#include <cmath>

#include <eosio/eosio.hpp>
#include <eosio/multi_index.hpp>
#include <eosio/dispatcher.hpp>
#include <eosio/contract.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
#include <eosio/action.hpp>
#include <eosio/symbol.hpp>
#include <eosio/name.hpp>

using namespace eosio;
using std::string;

CONTRACT clusterreg : public contract
{
public:
  using contract::contract;

  ACTION addprotocol(string title, name owner, name researcher, string datadir, name fundaccount, asset fund, name tokencontract, float dispatcher_pay_rate, float worker_pay_rate);
  ACTION delprotocol(name owner, name researcher);
  ACTION payout(name owner, name researcher, name worker, microseconds unpaid_time, string type);

private:
    TABLE account {
        asset balance;
        uint64_t primary_key() const { return balance.symbol.code().raw();}
    };

    typedef eosio::multi_index<"accounts"_n, account> accounts;

  TABLE protocol {
    string title;
    name researcher;
    string datadir;
    name fundaccount;
    asset fund;
    name tokencontract;
    float worker_pay_rate;
    float dispatcher_pay_rate;

    uint64_t primary_key() const { return researcher.value; }
  };
  typedef eosio::multi_index<"protocols"_n, protocol> protocols_t;

  void protocol_funded(name token_c, name fund_a, asset quantity);

};

EOSIO_DISPATCH(clusterreg,
  (addprotocol)
  (delprotocol)
  (payout)
)
