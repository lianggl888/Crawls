# ���湤��

ʹ��Kafka�ܹ�����Ϣ���У�Structured Streaming �Խ����ݵ�HDFS�Լ���ETL����

![](./imgs/pipeline_after.png)

## ִ�г���

�����������Ŀ¼��`main_run.sh`�ű�Ϊ������ڣ�������Ϊ��ʱ���񣬻����ֶ�ִ��. 

`$ bash main_run.sh`

��ǰ�����Զ����ű�����������entrobus32������, �����Զ����ű�������`/home/liujs/etc/mycrontab`

ÿ4Сʱִ��һ�Σ���crontab�������ļ��м���(ע������crontabʹ���Լ���һ�׻�����������Ҫ�ѻ����������뵽main_run.sh��)
```
# ÿ��4��Сʱ��kafka���������ѯ����
0 */4 * * * bash /home/liujs/crawler/direction_amap/main_run.sh
11 */4 * * * bash /home/liujs/crawler/direction_baidu/main_run.sh
# ÿ��7�챸��һ��Kafka�İٶȲ�ѯ����
0 1 */7 * * bash /home/liujs/crawler/direction_baidu/backup/backup_source.sh
```

`$ crontab mycrontab`

## Kafka �ܹ�

[Kafka����](http://kafka.apache.org/intro)

[�ٷ������ĵ�](http://kafka.apache.org/quickstart)

Kafka�汾��kafka_2.12-2.2.0 , kafkaĿ¼: `export KAFKA_HOME=/home/liujs/usr/local/kafka`

zookeeper��Ⱥ: entrobus32:2181,entrobus12:2181,entrobus28:2181/kafka1 (Kafkaע����Ϣ��zookeeper��/kafka1��)

Kafka��Ⱥ: entrobus32:9092,entrobus28:9092,entrobus12:9092 
```
id0: entrobus32:9092
id1: entrobus28:9092
id2: entrobus12:9092
```

Kafka�����ļ�server.properties
```
...
# kafka��zookeeper��ע����Ϣλ�ã�һ�����zookeeper��Ŀ¼�´���chroot������������kafka1
zookeeper.connect=entrobus32:2181,entrobus12:2181,entrobus28:2181/kafka1
# ��־��Ϣ���·��
log.dirs=/home/liujs/kafka-logs

# broker�ķ���Ĭ������
num.partitions=2

# default replication for auto created topic
# �Զ�����������Ĭ�ϸ�����
default.replication.factor=2

# Ĭ��������offset�ĸ����������ã��������������������1
offsets.topic.replication.factor=2
transaction.state.log.replication.factor=2
transaction.state.log.min.isr=2
...
```

����kafka��ֹͣkafka(�ڶ�Ӧbroker���ڵĻ����²���) 
```
# �����ػ�����, ��JMX����kafka-manager�鿴metric
export JMX_PORT=9988
kafka-server-start.sh -daemon $KAFKA_HOME/config/server.properties
# ֹͣKafka broker
kafka-server-stop.sh
# �鿴��ǰKafka����
jps
```

����Topic����

```
�鿴����
kafka-topics.sh --list --bootstrap-server entrobus32:9092

�鿴�Ƿ�������
kafka-console-consumer.sh --bootstrap-server entrobus32:9092 \
--topic directionbaidu --from-beginning

��������
kafka-topics.sh --describe --bootstrap-server entrobus32:9092 \
--topic direction_amap

��������
kafka-topics.sh --create \
--bootstrap-server entrobus32:9092 \
--replication-factor 2 --partitions 2 \
--topic directionbaiduss

ɾ������
kafka-topics.sh --delete --bootstrap-server entrobus32:9092 \
--topic directionamap
```

��������ĸ�����, ��Ҫ���������json�ļ�������kafka-reassign-partitions.sh���߽��д���.[�ٷ��ο�����](http://kafka.apache.org/documentation/#operations)

1. �鿴��ǰ����ķ���Partitions�͸�����Replications (�������������һ�������и���Replicas��һ����ͬ������Isr)
```
kafka-topics.sh --describe --bootstrap-server entrobus32:9092 \
--topic direction_amap
```

2. ����increase-replication-factor.json�ļ����������£�
```
{
    "version": 1,
    "partitions": [{
            "topic": "bus8684",
            "partition": 0,
            "replicas": [0, 2]
        }]
}
```
3. ִ�мƻ�, ע��zookeeper��λ�ñ���Ϊchroot
```
kafka-reassign-partitions.sh --zookeeper entrobus32:2181/kafka1 --reassignment-json-file increase-replication-factor.json --execute
```

kafka ��ع���: [kafka-manager](https://github.com/yahoo/kafka-manager)

��ص�ַ�� [http://entrobus32:10005](http://entrobus32:10005)

��ǰkafka-managerĿ¼Ϊ32�Ż�/home/liujs/usr/local/kafka-manager
```
����kafka-manager
cd $KAFKA_MANAGER_HOME
nohup bin/kafka-manager -Dhttp.port=10005 &
```

���Բ鿴��ǰ��broker/topic��Ϣ


ʹ��pyspark����:

```
# ��Ҫ��������jar�� (Ϊ��ʹ��kafka��delta)
pyspark --packages org.apache.spark:spark-sql-kafka-0-10_2.11:2.4.3,io.delta:delta-core_2.11:0.2.0 \
--master yarn \
--num-executors 3 \
--driver-memory 4g \
--executor-memory 2g \
--executor-cores 1
```

## �������ⶨ�壺

* poibaidu: ȡ��
* qqhouse: ������
* fang���ⷿƵ����: ������
* fangesf�����ַ������������У��ƻ�������ɣ�: ������
* gzmtr������ȴ���д���ֶεȴ���������: ��������
* dianping����Ҫ�ȴ��Ƚϳ���ʱ�����Զ�����: ��������
* easygo����Ҫ�ȴ��Ƚϳ���ʱ�����Զ�����: ��������
* tianyancha������ץ��������ȡ����Ҫ�ȴ��Ƚϳ�ʱ�����ӱ���windows10�ϴ���Ⱥ��ͨѶ���⣩: ��������
* direction_amap/directionbaidu

    ʵʱ��ȡ�̹�԰�ܲ��������ݰ��ƻ��������ڱ������������ݡ������Data Injection��ETL����

* shop58

    58ͬ����������