# !/bin/bash
while :
	do
		temp=$(cat /sys/class/hwmon/hwmon0/temp1_input)
		mosquitto_pub -h 172.20.98.222 -t iot/temperature -m "$temp"
		sleep 0.5
	done

