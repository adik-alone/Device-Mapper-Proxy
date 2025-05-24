echo "Starting stats"
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "+---------------------------------------------+"
echo "Write test"

echo "+---test 1---+"
echo "+---------------------------------------------+"
dd if=/dev/random of=/dev/mapper/dmp1 bs=4k count=1
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "+---------------------------------------------+"

echo "+---test 2---+"
echo "+---------------------------------------------+"
dd if=/dev/random of=/dev/mapper/dmp1 bs=1k count=1
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "+---------------------------------------------+"

echo "+---test 3---+"
echo "+---------------------------------------------+"
dd if=/dev/random of=/dev/mapper/dmp1 bs=8k count=1
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "+---------------------------------------------+"

echo "+---test 4---+"
echo "+---------------------------------------------+"
dd if=/dev/random of=/dev/mapper/dmp1 bs=4k count=10
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "+---------------------------------------------+"

echo "+---test 5---+"
echo "+---------------------------------------------+"
dd if=/dev/random of=/dev/mapper/dmp1 bs=4k count=1000
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "+---------------------------------------------+"
