#Jobs contract

##Purpose
- Manage incoming work units, workers, and work dispatchers
- Connect to vRAM thru jobs table
- Manage payouts to workers and work dispatchers by connecting to registry contract

## Actions
- conntoreg(name registry_contract);
- regworker(name worker, name researcher);
- delworker(name worker, name researcher);
- regdisptch(name dispatcher, name researcher);
- deldisptch(name dispatcher, name researcher);
- newjob(name researcher, name worker, uint64_t time);
- removejob(name researcher, name worker, uint64_t time);
- updatedptch(name researcher, name dispatcher, uint64_t time);
- payout(name owner, name funder, name researcher, name contributor, string type);

##Tables
- Manage dispatcher time contributions
TABLE dispatcher {
  name researcher;
  microseconds total_time;
  microseconds unpaid_time;

  uint64_t primary_key() const { return researcher.value; }
};

- Manage dispatcher worker contributions
TABLE worker {
  name researcher;
  microseconds total_time;
  microseconds unpaid_time;

  uint64_t primary_key() const { return researcher.value; }
};

- Manage completed jobs
TABLE job {
  name researcher;
  microseconds time;

  uint64_t primary_key() const { return time.count(); }
};

- Manage connection to registry
TABLE reg {
  name registry_contract = "x"_n;

  uint64_t primary_key() const { return 0; }
};

##Usage
- Connect to registry contract
`conntoreg()`
- Register worker
`regworker()`
- Delete worker
`delworker()`
- Register dispatcher
`regdisptch()`
- Delete dispatcher
`deldisptch()`
- Payout worker/dispatcher
`payout()`
`type` parameter is either "worker" or "dispatcher"

#Registry contract

##Purpose
- Manage registry of research protocols
- Manage payouts to contributors

##Actions
- addprotocol(string title, name owner, name researcher, string datadir, name fundaccount, asset fund, name tokencontract, float dispatcher_pay_rate, float worker_pay_rate);
- delprotocol(name owner, name researcher);
- payout(name owner, name researcher, name worker, microseconds unpaid_time, string type);

##Tables
- Registry of research protocols
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

##Usage
- Add new research protocol
`addprotocol()`
Payout token must have standard accounts table layout (see eosio.token)
Must add `eosio.code` permissions under active permissions of registry contract
- Delete research protocol
`delprotocol()`
- Payout
`payout()`
Only triggered from jobs contract
Only triggered by funder

#Apache Mesos/Mesosphere Marathon/Apache Zookeeper/Apache HDFS

##Purpose
- Cluster scheduler and container manager (Mesos)
- Cluster manager and operator (Marathon)
- Resource manager (Zookeeper)
- Data and resource storage (HDFS)
- Distribute necessary work information through containers
- Register dispatchers as mesos-masters
- Register workers as mesos-slaves

##Usage
- Start necessary containers in actors/dispatchers folder
`cd actors/dispatchers && docker-compose up -d`
- TODO Start necessary containers in actors/workers folder
`cd actors/workers && docker-compose up -d`
- TODO Start necessary containers in actors/storage folder
`cd actors/storage && docker-compose up -d`
- TODO Submit jobs
`sample_submit.sh`
- TODO Query Mesos REST API for information about contributors
