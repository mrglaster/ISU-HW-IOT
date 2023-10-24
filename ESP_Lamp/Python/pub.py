import time

import paho.mqtt.client as paho
import questionary

broker = 'broker.emqx.io'

client = paho.Client("client-isu-81")

print("Connecting to broker ", broker)
client.connect(broker)
client.loop_start()
print("Publishing")

topic = questionary.text("What's topic?").ask()
print(topic)

start_time = 20
cur_time_limit = start_time
min_time = 10
while True:
    client.publish(topic, "0")
    print(0)
    time.sleep(20)
    client.publish(topic, "1")
    print(1)
    time.sleep(cur_time_limit)
    client.publish(topic, "0")
    print(0)
    time.sleep(40 - cur_time_limit)
    if cur_time_limit > min_time:
        cur_time_limit -= 1
    else:
        cur_time_limit = start_time

client.disconnect()
client.loop_stop()