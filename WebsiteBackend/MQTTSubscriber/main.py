import sys
import paho.mqtt.client as paho
import mariadb
import time


#MQTT broker attributes
MQTT_broker_ip = "142.93.105.52"
MQTT_broker_port = 1883
MQTT_username = "entranceDoor"
MQTT_password = "VMN2rLa8aloFFQ3"

#SQL database attributes
DB_user="phusr"
DB_password="7yLCTYetfQha4k5t"
DB_host="localhost"
DB_database="sus"

def fetchAcccessListFromDB():
    conn = mariadb.connect(
        user=DB_user,
        password=DB_password,
        host=DB_host,
        database=DB_database)
    cur = conn.cursor() 
    cur.execute("SELECT employees.pin, employees.nuid FROM dooraccess LEFT JOIN employees ON (dooraccess.employeeID = employees.id)") 
    accessList = str(cur.rowcount).zfill(2)
    for pin, nuid in cur: 
        print(f"pin: {pin}, nuid: {nuid}")
        accessList += nuid + str(pin)
    cur.close()
    conn.close()
    return accessList

def insertLogEntryToDB(logEntry):
    conn = mariadb.connect(
        user=DB_user,
        password=DB_password,
        host=DB_host,
        database=DB_database)
    conn.autocommit = True
    cur = conn.cursor() 
    success = logEntry[0]
    nuid = logEntry[1:]
    cur.execute("INSERT INTO logs (employeeID,success,time) SELECT id,?,CURRENT_TIMESTAMP() FROM employees WHERE nuid=?", (int(success),str(nuid))) 
    cur.close()
    conn.close()

#Handler for received messages from MQTT Broker
def message_handling(client, userdata, msg):
    print(f"{msg.topic}: {msg.payload.decode()}")
    if (msg.topic == "sus/hello"):
        print("Received Hello message")
        acl = fetchAcccessListFromDB()
        print("Sending ACL: " + acl)
        client.publish("sus/accessList", acl)
    if (msg.topic == "sus/logEntry"):
        print("Received log entry: "+ msg.payload.decode())
        insertLogEntryToDB(msg.payload.decode())

#Continue running the code if any errors occur
while True:
    try:
        print("Trying to connect...")
        client = paho.Client()
        client.on_message = message_handling
        client.username_pw_set(username=MQTT_username,password=MQTT_password)
        if client.connect(MQTT_broker_ip, MQTT_broker_port, 60) != 0:
            print("Couldn't connect to the mqtt broker")
            sys.exit(1)
        print("Connected to the MQTT broker")
        client.subscribe("sus/hello")
        client.subscribe("sus/logEntry")
        client.loop_forever()
    except Exception as e: 
        print(e)
        time.sleep(10)