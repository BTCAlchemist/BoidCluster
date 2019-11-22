#include "registry.hpp"

using namespace eosio;
using std::string;

void clusterreg::addprotocol(string title, name owner, name researcher, string datadir, name fundaccount, asset fund, name tokencontract, float dispatcher_pay_rate, float worker_pay_rate)
{
  require_auth(owner);

  protocols_t prot_t(get_self(), owner.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i == prot_t.end(), "protocol exists");

  protocol_funded(tokencontract, fundaccount, fund);

  prot_t.emplace(owner, [&](auto& p) {
    p.researcher = researcher;
    p.datadir = datadir;
    p.fundaccount = fundaccount;
    p.fund = fund;
    p.tokencontract = tokencontract;
    p.dispatcher_pay_rate = dispatcher_pay_rate;
    p.worker_pay_rate = worker_pay_rate;
  });
}

void clusterreg::delprotocol(name owner, name researcher)
{
  require_auth(owner);

  protocols_t prot_t(get_self(), owner.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  prot_t.erase(prot_i);
}

void clusterreg::payout(name owner, name researcher, name worker, microseconds unpaid_time, string type)
{

  protocols_t prot_t(get_self(), owner.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");
  const auto &prot = *prot_i;

  require_auth(prot.fundaccount);

  symbol sym = prot.fund.symbol;
  asset quantity;
  check(type.compare("worker") == 0 || type.compare("dispatcher") == 0,
    "contributor type must be either `worker` or `dispatcher`");
  if (type.compare("worker")) {
    int64_t amount = (int64_t)unpaid_time.count()*prot.worker_pay_rate;
    quantity = asset{amount,sym};
  } else if (type.compare("dispatcher") == 0){
    int64_t amount = (int64_t)unpaid_time.count()*prot.dispatcher_pay_rate;
    quantity = asset{amount,sym};
  }
  protocol_funded(prot.tokencontract, prot.fundaccount, quantity);

  if (worker != prot.fundaccount)
  {
    string memo = "payment for " + type + " on " + prot.title +\
                  "in Boid cluster environment";
    action(
      permission_level{prot.fundaccount,"active"_n},
      prot.tokencontract,
      "transfer"_n,
      std::make_tuple(prot.fundaccount, worker, quantity, memo)
    ).send();
  }

  prot_t.modify(prot_i, same_payer, [&](auto& a) {
    a.fund -= quantity;
  });
}

void clusterreg::protocol_funded(name token_c, name fund_a, asset quantity)
{
  accounts acct_t(token_c, fund_a.value);
  auto acct_i = acct_t.find(quantity.symbol.code().raw());
  check(acct_i != acct_t.end(),
    "fund account does not exist in specified token contract");
  check(acct_i->balance >= quantity,
    "fund account does not have sufficient balance");
}
