insmod device_mapper_proxy.ko
dmsetup create zero1 --table "0 10000 zero"
ls -la /dev/mapper/*
dmsetup create dmp1 --table "0 10000 dmp /dev/mapper/zero1"
ls -la /dev/mapper/*
