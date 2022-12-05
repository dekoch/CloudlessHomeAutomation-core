
#define CONFIG_MESH_FILE  "config_mesh.json"

/*
config_mesh.json
{
  "t": "config_mesh.json",
  "rev": 9,
  "nCnt": 11,
  "n": [
    {
      "nodeId": 2503482707,
      "name": "KeWkLi",
      "cm": "AP_STA"
    },
    {
      "nodeId": 2503414767,
      "name": "KeWkGaSw",
      "cm": "STA"
    },
    {
      "nodeId": 432742653,
      "name": "E0FlLi",
      "cm": "AP_STA"
    },
    {
      "nodeId": 432770609,
      "name": "Shelly432770609",
      "cm": "AP_STA"
    },
    {
      "nodeId": 432736461,
      "name": "Shelly432736461",
      "cm": "AP_STA"
    },
    {
      "nodeId": 2141195262,
      "name": "SonoffS20",
      "cm": "AP_STA"
    },
    {
      "nodeId": 1416346201,
      "name": "Shelly2.5",
      "cm": "AP_STA"
    },
    {
      "nodeId": 2137932489,
      "name": "D1 2137932489 AP_STA",
      "cm": "AP_STA"
    },
    {
      "nodeId": 2137931765,
      "name": "D1 2137931765 STA",
      "cm": "STA"
    },
    {
      "nodeId": 2137930055,
      "name": "D1 2137930055 STA",
      "cm": "STA"
    },
    {
      "nodeId": 2137932350,
      "name": "D1 2137932350 STA",
      "cm": "STA"
    }
  ]
}
*/

class ConfigMesh {
  public:
    // initial value
    String Json = "{\"t\":\"" + String(CONFIG_MESH_FILE) + "\",\"rev\":3,\"nCnt\":2,\"n\":[{\"nodeId\":2503482707,\"name\":\"KeGaLi\",\"cm\":\"AP_STA\"},{\"nodeId\":2503414767,\"name\":\"KeWkGaSw\",\"cm\":\"AP_STA\"}]}";

    ConfigMesh() {}

    void Read() {
      String temp = ReadFile(CONFIG_MESH_FILE);

      if (temp == "") {
        WriteFile(CONFIG_MESH_FILE, Json);
      } else {
        Serial.println(String(CONFIG_MESH_FILE) + " " + temp);
        Update(temp);
      }
    }

    bool Update(String newConfig) {
      
      if (CompareRevision(Json, newConfig) == false) {
        return false;
      }

      Json = newConfig;
      Serial.println("updated configMesh " + Json);

      String temp = ReadFile(CONFIG_MESH_FILE);

      if (CompareRevision(temp, newConfig) == false) {
        return false;
      }

      WriteFile(CONFIG_MESH_FILE, newConfig);
      Serial.println("updated " + String(CONFIG_MESH_FILE) + " " + newConfig);
      return true;
    }

    uint32_t GetNodeId(String node) {

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, Json);

      for (int i = 0; i < doc["nCnt"].as<int>(); i++) {

        if (doc["n"][i]["name"].as<String>() == node) {
          return doc["n"][i]["nodeId"].as<uint32_t>();
        }
      }

      return 0;
    }

    String GetNodeName(uint32_t nodeId) {

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, Json);

      for (int i = 0; i < doc["nCnt"].as<int>(); i++) {

        if (doc["n"][i]["nodeId"].as<uint32_t>() == nodeId) {
          return doc["n"][i]["name"].as<String>();
        }
      }

      return String(nodeId);
    }

    // AP_STA (default) / STA / AP
    WiFiMode_t GetConnectMode(uint32_t nodeId) {

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, Json);

      for (int i = 0; i < doc["nCnt"].as<int>(); i++) {

        if (doc["n"][i]["nodeId"].as<uint32_t>() == nodeId) {

          String connectMode = doc["n"][i]["cm"].as<String>();

          Serial.println(connectMode);

          if (connectMode == "STA") {

            return WIFI_STA;

          } else if (connectMode == "AP") {
            
            return WIFI_AP;

          } else {
            
            return WIFI_AP_STA;
          }
        }
      }

      Serial.println(String(nodeId));
      Serial.println("AP_STA (default)");
      return WIFI_AP_STA;
    }
};
