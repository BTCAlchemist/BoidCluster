FROM mesosphere/mesos:1.7.1
MAINTAINER Mesosphere <support@mesosphere.io>

RUN apt-get update && apt-get -y install lsb-release chronos curl

RUN rm -rf /etc/mesos
RUN rm -rf /etc/chronos/conf

EXPOSE 4400

CMD ["/usr/bin/chronos", "run_jar", "--http_port", "4400", "--zk_hosts",  "localhost:2181", "--master", "zk://localhost:2181"]
