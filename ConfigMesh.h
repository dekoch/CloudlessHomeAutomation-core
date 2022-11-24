
#define CONFIG_MESH_FILE  "config_mesh.json"

/*
config_mesh.json
{
  "t":"config_mesh.json",
  "rev":6,
  "nCnt":5,
  "n":[
    {
      "nodeId":2503482707,
      "name":"KeWkLi"
    },
    {
      "nodeId":2503414767,
      "name":"KeWkGaSw"
    },
    {
      "nodeId":2141195262,
      "name":"SonoffS20"
    },
    {
      "nodeId":432742653,
      "name":"Shelly1PM"
    },
    {
      "nodeId":1416346201,
      "name":"Shelly2.5"
    }
  ]
}
*/

class ConfigMesh {
  public:
    // initial value
    String Json = "{\"t\":\"" + String(CONFIG_MESH_FILE) + "\",\"rev\":3,\"nCnt\":2,\"n\":[{\"nodeId\":2503482707,\"name\":\"KeGaLi\"},{\"nodeId\":2503414767,\"name\":\"KeWkGaSw\"}]}";

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
};
