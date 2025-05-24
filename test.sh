echo "Starting stats"
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "Write test"
echo "+---------------------------------------------+"
dd if=/dev/random of=/dev/mapper/dmp1 bs=4k count=1
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "+---------------------------------------------+"
echo "Read test"
echo "+---------------------------------------------+"
dd if=/dev/mapper/basic_target1 of=/dev/null bs=4k count=1
echo "+---------------------------------------------+"
cat /sys/module/device_mapper_proxy/stat/volumes
echo "+---------------------------------------------+"
