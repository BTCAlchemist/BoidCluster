This is a cluster containerr management system that hedges the usage of the Boid
network for general-purpose containerizable tasks. It is especially useful for
embarassingly-parallel programs and more generally for programs without strict
scheduling requirements.

The goal of this project is to create a classification and library of realistic use cases for public ip-routed
compute clusters: i.e. cluster-at-home systems. Idealogical stress for initial
use cases of the Boid cluster is placed on openness of access to at-home compute clusters for scientific
researchers and for students. This goal is accomplished by leveraging the
team-based structure of the Boid network. Each team, which typically is run by
a funded organization, may sponsor one or more protocols. Teams are encouraged
to fund scientific researchers for their paper-producing projects and motivated
student groups who would like to learn more about cluster computing but don't
necessarily have the skills or access otherwise to do so.

The components of this project are split into the dockerized components and the
EOSIO-based components. The dockerized components includes three different types
of actors: dispatcher, worker, and storage. The dispatcher is responsible for
managing and operating the compute cluster, meaning that it dispatches jobs
throughout the network, amalgamates results, and handles network faults. The
dispatcher should be a single machine interface with high-resource availability. The
worker may be a machine with low-resource availability. The worker operates on
parallelized work units and returns results to the dispatcher.
The storage actor uses the Hadoop Distributed File System (HDFS) to store
amalgamated results and provide bootstrap data for cluster programs.
The EOSIO-based components are the clusterjobs (jobs) contract and the clusterreg
(registry) contract. The clusterjobs contract stores information associated with cluster
workers, dispatchers, and storage actors to keep track of cluster contributions.
As such, the actors are also referred to as contributors. Each job is stored in
Liquidapps.io vRAM. The clusterreg contract stores information associated
with the cluster protocols, including associated researcher, actor, fund
balance and type, and protocol sponsor. 

As this project starts, we will expect the community to engage with us
constructively on the strengths and weaknesses of the Boid cluster, both so that
we have a clear path towards improvement and also so that the community will
have realistic expectations about the abilities and limitations of this form of
cluster computing. Some ideas for improvement are ip-localized cluster groupings
to lower latency, HBase/Cassandra integration for increasing utility of
distributed data storage, IPFS-swarm integration for a more persistent and accessible
data-storage solution, Storm/Kafka integration for datastream tasks, Samza
integration for real-time and fine-grained processing tasks.

Currently, this project has been tested on a local cluster with an example Spark
program. A lot of work still has to be done on this project. Please reach out to
us if you are interested in following along with the progress.
