
#include <painlessMesh.h>
#include "Fs.h"
#include "Config.h"
#include "ConfigWifi.h"
#include "ConfigMesh.h"
#include "ConfigSwitch.h"
#include "ConfigTimer.h"

painlessMesh  mesh;
ConfigMesh configMesh = ConfigMesh();
ConfigSwitch configSwitch = ConfigSwitch();
ConfigTimer configTimer = ConfigTimer();

#ifdef ROOT_NODE
#include "WebConfig.h"
//#include "WebUi.h"
#endif

bool onFlag = false;
String myName = "";
bool calc_delay = false;
SimpleList<uint32_t> nodes;

// Prototypes
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
void delayReceivedCallback(uint32_t from, int32_t delay);

Scheduler userScheduler; // to control your personal task
// Task to blink the number of nodes
Task blinkNoNodes;


uint8_t GetChannel() {

    if (MESH_CHANNEL > 0) {
        return MESH_CHANNEL;
    }

    int scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

    if (scanResult == 0) {
        return 1;
    }

    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t* bssid;
    int32_t channel;
    bool hidden;
    int32_t bestChannel = 1;
    int32_t lastRssi = 0;

    for (int8_t i = 0; i < scanResult; i++) {
        WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

        Serial.printf(PSTR("[CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %s\n"),
                    channel,
                    bssid[0], bssid[1], bssid[2],
                    bssid[3], bssid[4], bssid[5],
                    rssi,
                    (encryptionType == ENC_TYPE_NONE) ? ' ' : '*',
                    hidden ? 'H' : 'V',
                    ssid.c_str());

        if (MESH_CHANNEL == -1 &&
            String(MESH_SSID) == ssid) {
                return channel;
        }

        if (rssi < lastRssi){
            // select best channel
            bestChannel = channel;
            lastRssi = rssi;
        }

        delay(0);
    }

    for (uint8_t ch = 1; ch <= 13; ch++) {

        bool isFreeCh = true;

        for (int8_t sr = 0; sr < scanResult; sr++) {
            WiFi.getNetworkInfo(sr, ssid, encryptionType, rssi, bssid, channel, hidden);

            if (ch == channel) {
                isFreeCh = false;
            }
        }

        if (isFreeCh) {
            return ch;
        }
    }

    return bestChannel;
}

void MeshSetup() {

    uint8_t channel = GetChannel();
    //Serial.println("AP at channel " + String(channel));

    mesh.setDebugMsgTypes(ERROR | STARTUP);  // set before init() so that you can see error messages

    mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, channel);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.onNodeDelayReceived(&delayReceivedCallback);
    mesh.initOTAReceive(FIRMWARE);

    String nodeId = String(mesh.getNodeId());
    myName = configMesh.GetNodeName(mesh.getNodeId());

    if (myName == "") {
        myName = nodeId;
    }

    Serial.println("NodeID " + nodeId);
    Serial.println("NodeName " + myName);
}

void newConnectionCallback(uint32_t nodeId) {
    // Reset blink task
    onFlag = false;
    blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
    blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);

    Serial.printf("New Connection, nodeId = %u\n", nodeId);
    Serial.printf("New Connection, %s\n", mesh.subConnectionJson(true).c_str());

    mesh.sendSingle(nodeId, configMesh.Json);
    Serial.println("Send to " + String(nodeId) + " msg=" + configMesh.Json);
    mesh.sendSingle(nodeId, configSwitch.Json);
    Serial.println("Send to " + String(nodeId) + " msg=" + configSwitch.Json);
}

void changedConnectionCallback() {
    Serial.printf("Changed connections\n");
    // Reset blink task
    onFlag = false;
    blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
    blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);

    nodes = mesh.getNodeList();

    Serial.printf("Num nodes: %d\n", nodes.size());
    Serial.printf("Connection list:");

    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end()) {
    Serial.printf(" %u", *node);
    node++;
    }
    Serial.println();
    calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
    Serial.printf("Delay to node %u is %d us\n", from, delay);
}

void ConfigReceivedCallback(String type, String msg) {

    if (type == CONFIG_MESH_FILE) {

        configMesh.Update(msg);

    } else if (type == CONFIG_SWITCH_FILE) {

        configSwitch.Update(msg);

    } else if (type == CONFIG_TIMER_FILE) {

        configTimer.Update(msg);
    }
}