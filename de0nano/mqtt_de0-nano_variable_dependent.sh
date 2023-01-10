# !/bin/bash
last_send_temp="9999"
while :
	do
		echo $(date +"%s%3N") 
        temp=$(cat /sys/class/hwmon/hwmon0/temp1_input)
        if [ "$temp" != "$last_send_temp" ]; then
                last_send_temp=$temp
                echo "sending $temp"
	      	mosquitto_pub -h 172.20.98.222 -t iot/temperature -m "$temp"
	else
		echo "not sending"
	fi
        sleep 0.5
		echo $(date +"%s%3N") 
	done

