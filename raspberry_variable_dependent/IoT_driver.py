#Copyright (c) 2007, Eclipse Foundation, Inc. and its licensors.
#
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#	Neither the name of the Eclipse Foundation, Inc. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. 
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import paho.mqtt.client as mqtt
import time
import os
import light

def create_Pid_File():
	pid = str(os.getpid())
	currentFile = open('/home/pi/IoT_folder/IoT_driver/pid_file/IoT_driver.pid', 'w')
	currentFile.write(pid)
	currentFile.close()

#def topic_to_publish(idx):
#	topics = {
#		5 : "sleep/temp",
#        6 : "sleep/humidity",
#	}
#	return topics.get(idx, "invalid_topic")


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))
	# Subscribing in on_connect() means that if we lose the connection and
	# reconnect then subscriptions will be renewed.
	#client.subscribe("domoticz/out")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	
	payload = str(msg.payload.decode("utf-8","ignore"))
	print(msg.topic+" "+str(msg.payload))
	

create_Pid_File()
print("creating pid file")
client = mqtt.Client()
client.on_connect = on_connect

client.connect("172.20.98.222", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
#client.loop_forever()
time.sleep(1)
last_valid_msg="nothing"
while True:
	msg_to_publish = light.read_color_values()
	if msg_to_publish != last_valid_msg:
		last_valid_msg = msg_to_publish
		print("publish")
		client.publish("iot/color",msg_to_publish,qos=0) #publish retained messeage
	else:
		print("dont publish")
	time.sleep(1)  #sleeps for 2
