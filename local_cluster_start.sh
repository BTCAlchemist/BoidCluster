sudo systemctl start sshd
start-dfs.sh
#hdfs dfs -mkdir /user/data/spark
#hdfs dfs -put ~/Downloads/spark-2.4.4-bin-hadoop2.7.tgz /user/data/spark/spark-2.4.4-bin-hadoop2.7.tgz

zkServer.sh start
mesos-master --ip=127.0.0.1 --work_dir=/l/v/mesos/work --zk=zk://127.0.0.1:2181/mesos --quorum=1 &
mesos-agent --master=127.0.0.1:5050 --work_dir=/l/v/mesos/work/ --no-systemd_enable_support &
marathon --master zk://127.0.0.1:2181/mesos --zk zk://127.0.0.1:2181/marathon --http_port 3998 &
