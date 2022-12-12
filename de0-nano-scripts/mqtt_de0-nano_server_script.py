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


def on_message_temp(mosq, obj, msg):
    # This callback will only be called for messages with topics that match
    # $SYS/broker/messages/#
    print("temp: " + msg.topic + " " + str(msg.qos) + " " + str(msg.payload))


def on_message_jasnosc(mosq, obj, msg):
    # This callback will only be called for messages with topics that match
    # $SYS/broker/bytes/#
    print("jasnosc: " + msg.topic + " " + str(msg.qos) + " " + str(msg.payload))


def on_message(mosq, obj, msg):
    # This callback will be called for messages that we receive that do not
    # match any patterns defined in topic specific callbacks, i.e. in this case
    # those messages that do not have topics $SYS/broker/messages/# nor
    # $SYS/broker/bytes/#
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))


mqttc = mqtt.Client()

# Add message callbacks that will only trigger on a specific subscription match.
mqttc.message_callback_add("temat/temp/#", on_message_temp)
mqttc.message_callback_add("temat/jasnosc/#", on_message_jasnosc)
mqttc.on_message = on_message
mqttc.connect("127.0.0.1", 1883, 60)
mqttc.subscribe("temat/#", 0)

mqttc.loop_forever()

