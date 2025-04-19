import paho.mqtt.publish as mqtt


mqtt.single("check", str("1"), hostname="91.121.93.94")
