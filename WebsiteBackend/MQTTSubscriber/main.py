import sys
import paho.mqtt.client as paho
import mariadb
import time



def fetchAcccessListFromDB():
    conn = mariadb.connect(
        user="phusr",
        password="ENTER_PASSWORD_HERE",
        host="localhost",
        database="sus")
    cur = conn.cursor() 

    doorID = 1 
    cur.execute("SELECT employees.pin, employees.nuid FROM dooraccess LEFT JOIN employees ON (dooraccess.employeeID = employees.id) WHERE dooraccess.doorID=?", (str(doorID),)) 

    accessList = str(cur.rowcount).zfill(2)
    for pin, nuid in cur: 
        print(f"pin: {pin}, nuid: {nuid}")
        accessList += nuid + str(pin)

    cur.close()
    conn.close()
    return accessList

def addLogEntryToDB(logEntry):
    conn = mariadb.connect(
        user="phusr",
        password="ENTER_PASSWORD_HERE",
        host="localhost",
        database="sus")
    conn.autocommit = True
    cur = conn.cursor() 

    success = logEntry[0]
    doorID = 1
    nuid = logEntry[1:]
    print(nuid)
    cur.execute("INSERT INTO logs (employeeID,success,doorID,time) SELECT id,?,?,CURRENT_TIMESTAMP() FROM employees WHERE nuid=?", (int(success),doorID,str(nuid))) 
    #INSERT INTO logs (employeeID,success,doorID,time) SELECT id,1,1,CURRENT_TIMESTAMP() FROM employees WHERE nuid='ABCDEF'
    #(int(success),doorID,str(nuid))

    # print content
    print(cur.rowcount)

    cur.close()
    conn.close()


def message_handling(client, userdata, msg):
    print(f"{msg.topic}: {msg.payload.decode()}")
    if (msg.topic == "sus/hello"):
        print("Received Hello message")
        acl = fetchAcccessListFromDB()
        print("Sending ACL: " + acl)
        client.publish("sus/accessList", acl)
    if (msg.topic == "sus/logEntry"):
        print("Received log entry: "+ msg.payload.decode())
        addLogEntryToDB(msg.payload.decode())





while True:
    try:
        print("Trying to connect...")
        client = paho.Client()
        client.on_message = message_handling
        client.username_pw_set(username="entranceDoor",password="ENTER_PASSWORD_HERE")
        if client.connect("142.93.105.52", 1883, 60) != 0:
            print("Couldn't connect to the mqtt broker")
            sys.exit(1)
        print("Connected to the MQTT broker")
        client.subscribe("sus/hello")
        client.subscribe("sus/logEntry")
        client.loop_forever()
    except Exception:
        print("Caught an Exception, something went wrong...")
        time.sleep(10)
#finally:
#    print("Disconnecting from the MQTT broker")
#    client.disconnect()