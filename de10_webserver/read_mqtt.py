#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright (c) 2014 Roger Light <roger@atchoo.org>
#
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Distribution License v1.0
# which accompanies this distribution.
#
# The Eclipse Distribution License is available at
#   http://www.eclipse.org/org/documents/edl-v10.php.
#
# Contributors:
#    Roger Light - initial implementation
# All rights reserved.

# This shows a simple example of an MQTT subscriber using a per-subscription message handler.

#import context  # Ensures paho is in PYTHONPATH

import paho.mqtt.client as mqtt

def write_to_db(mosq, obj, msg):
    message_to_write = msg.topic + " " + msg.payload.decode("utf-8")
    db_r = open("www/db.txt", "r")
    lines = db_r.readlines()
    if len(lines) > 10 :
        db = open("www/db.txt", "w")
    else:
        db = open("www/db.txt", "a")   
    db.write(message_to_write+"\n")
    db.close()
    
def on_message_temp(mosq, obj, msg):
    write_to_db(mosq, obj, msg)

def on_message_temp2(mosq, obj, msg):
    write_to_db(mosq, obj, msg)

def on_message_temp3(mosq, obj, msg):
    write_to_db(mosq, obj, msg)

def on_message_temp4(mosq, obj, msg):
    write_to_db(mosq, obj, msg)

def on_message_color(mosq, obj, msg):
    write_to_db(mosq, obj, msg)

def on_message_acc(mosq, obj, msg):
    write_to_db(mosq, obj, msg)


def on_message(mosq, obj, msg):
    # This callback will be called for messages that we receive that do not
    # match any patterns defined in topic specific callbacks, i.e. in this case
    # those messages that do not have topics $SYS/broker/messages/# nor
    # $SYS/broker/bytes/#
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))


mqttc = mqtt.Client()

# Add message callbacks that will only trigger on a specific subscription match.
mqttc.message_callback_add("iot/temperature", on_message_temp)
mqttc.message_callback_add("iot/temperature2", on_message_temp2)
mqttc.message_callback_add("iot/temperature3", on_message_temp3)
mqttc.message_callback_add("iot/temperature4", on_message_temp4)
mqttc.message_callback_add("iot/color", on_message_color)
mqttc.message_callback_add("iot/acc", on_message_acc)
mqttc.on_message = on_message
mqttc.connect("127.0.0.1", 1883, 60)
mqttc.subscribe("iot/#", 0)

mqttc.loop_forever()

