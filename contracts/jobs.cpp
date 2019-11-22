#define DAPP_RAM_PAYER current_provider
#include "jobs.hpp"

using namespace eosio;

[[eosio::action]] void clusterjobs::conntoreg(name registry_contract)
{
  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  if (reg_i == reg_t.end()) {
    reg_t.emplace(get_self(), [&](auto& a) {
      a.registry_contract = registry_contract;
    });
  } else {
    reg_t.modify(reg_i, get_self(), [&](auto& a) {
      a.registry_contract = registry_contract;
    });
  }
}

[[eosio::action]] void clusterjobs::regworker(name worker, name researcher)
{
  require_auth(get_self());

  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  check(reg_i != reg_t.end(), "Must connect this contract to registry contract");
  const auto& regis = *reg_i;

  protocols_t prot_t(regis.registry_contract, researcher.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  workers_t worker_t(get_self(), worker.value);
  auto worker_i = worker_t.find(researcher.value);
  check(worker_i == worker_t.end(),
    "worker exists");

  worker_t.emplace(get_self(), [&](auto& a) {
    a.researcher = researcher;
    a.total_time = microseconds(0);
    a.unpaid_time = microseconds(0);
  });
}

[[eosio::action]] void clusterjobs::delworker(name worker, name researcher)
{
  require_auth(get_self());

  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  check(reg_i != reg_t.end(), "Must connect this contract to registry contract");
  const auto& regis = *reg_i;

  protocols_t prot_t(regis.registry_contract, researcher.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  workers_t worker_t(get_self(), worker.value);
  auto worker_i = worker_t.find(researcher.value);
  check(worker_i != worker_t.end(),
    "worker does not exist");

  worker_t.erase(worker_i);
}

[[eosio::action]] void clusterjobs::regdisptch(name dispatcher, name researcher)
{
  require_auth(get_self());

  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  check(reg_i != reg_t.end(), "Must connect this contract to registry contract");
  const auto& regis = *reg_i;

  protocols_t prot_t(regis.registry_contract, researcher.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  dispatchers_t d_t(get_self(), dispatcher.value);
  auto d_i = d_t.find(researcher.value);
  check(d_i == d_t.end(),
    "dispatcher exists");

  d_t.emplace(get_self(), [&](auto& a) {
    a.researcher = researcher;
    a.total_time = microseconds(0);
    a.unpaid_time = microseconds(0);
  });
}

[[eosio::action]] void clusterjobs::deldisptch(name dispatcher, name researcher)
{
  require_auth(get_self());

  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  check(reg_i != reg_t.end(), "Must connect this contract to registry contract");
  const auto& regis = *reg_i;

  protocols_t prot_t(regis.registry_contract, researcher.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  dispatchers_t d_t(get_self(), dispatcher.value);
  auto d_i = d_t.find(researcher.value);
  check(d_i != d_t.end(),
    "dispatcher does not exist");

  d_t.erase(d_i);
}

[[eosio::action]] void clusterjobs::newjob(name researcher, name worker, uint64_t time)
{
  require_auth(get_self());

  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  check(reg_i != reg_t.end(), "Must connect this contract to registry contract");
  const auto& regis = *reg_i;

  protocols_t prot_t(regis.registry_contract, researcher.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  workers_t worker_t(get_self(), worker.value);
  auto worker_i = worker_t.find(researcher.value);
  check(worker_i != worker_t.end(),
    "worker does not exist for this project");

  jobs_t j_t(
    get_self(),
    worker.value,
    1024, 64, false, false
  );

  auto j_i = j_t.find(time);
  check(j_i == j_t.end(), "Job for this time already exists");

  worker_t.modify(worker_i, get_self(), [&](auto& a) {
    a.researcher = researcher;
    a.total_time += microseconds(time);
    a.unpaid_time += microseconds(time);
  });

  j_t.emplace(get_self(), [&](auto& a) {
    a.researcher = researcher;
    a.time = microseconds(time);
  });
}

[[eosio::action]] void clusterjobs::removejob(name researcher,  name worker, uint64_t time)
{
  require_auth(get_self());

  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  check(reg_i != reg_t.end(), "Must connect this contract to registry contract");
  const auto& regis = *reg_i;

  protocols_t prot_t(regis.registry_contract, researcher.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  jobs_t j_t(
    get_self(),
    worker.value,
    1024, 64, false, false
  );

  auto j_i = j_t.find(time);

  check(j_i != j_t.end(), "Job does not exist");
  j_t.erase(j_i);
}

[[eosio::action]] void clusterjobs::updatedptch(name researcher, name dispatcher, uint64_t time)
{
  require_auth(get_self());

  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  check(reg_i != reg_t.end(), "Must connect this contract to registry contract");
  const auto& regis = *reg_i;

  protocols_t prot_t(regis.registry_contract, researcher.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  dispatchers_t d_t(get_self(), dispatcher.value);
  auto d_i = d_t.find(researcher.value);
  check(d_i != d_t.end(),
    "dispatcher does not");
  
  d_t.modify(d_i, same_payer, [&](auto& a) {
    a.total_time += microseconds(time);
    a.unpaid_time += microseconds(time);
  });
}

[[eosio::action]] void clusterjobs::payout(name owner, name funder, name researcher, name contributor, string type)
{
  registry_t reg_t(get_self(), get_self().value);
  auto reg_i = reg_t.find(0);
  check(reg_i != reg_t.end(), "Must connect this contract to registry contract");
  const auto& regis = *reg_i;

  protocols_t prot_t(regis.registry_contract, researcher.value);
  auto prot_i = prot_t.find(researcher.value);
  check(prot_i != prot_t.end(), "protocol does not exist");

  require_auth(funder);

  check(type.compare("worker") == 0 || type.compare("dispatcher") == 0,
    "contributor type must be either `worker` or `dispatcher`");

  dispatchers_t d_t(get_self(), contributor.value);
  workers_t worker_t(get_self(), contributor.value);
  microseconds unpaid_time = microseconds(0);
  if (type.compare("worker") == 0) {
    auto worker_i = worker_t.find(researcher.value);
    check(worker_i != worker_t.end(),
      "worker does not exist for this project");
    unpaid_time += worker_i->unpaid_time;
  } else {
    auto d_i = d_t.find(researcher.value);
    check(d_i != d_t.end(),
      "dispatcher does not");
    unpaid_time += d_i->unpaid_time;
  }
    
  action(
    permission_level{funder,"active"_n},
    regis.registry_contract,
    "payout"_n,
    std::make_tuple(owner, researcher, contributor, unpaid_time, type)
  ).send();

  if (type.compare("worker") == 0) {
    auto worker_i = worker_t.find(researcher.value);
    worker_t.modify(worker_i, same_payer, [&](auto& a) {
      a.unpaid_time = microseconds(0);
    });
  } else {
    auto d_i = d_t.find(researcher.value);
    d_t.modify(d_i, same_payer, [&](auto& a) {
      a.unpaid_time = microseconds(0);
    });
  }
}
