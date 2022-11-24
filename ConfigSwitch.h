
#define CONFIG_SWITCH_FILE  "config_switch.json"

/*
config_switch.json
{
  "t":"config_switch.json",
  "rev":13,
  "gCnt":1,
  "g":[
    {
      "name":"Toggle",
      "tCnt":2,
      "t":[
        {
          "node":"KeWkGaSw",
          "di":0,
          "state":false
        },
        {
          "node":"KeWkGaSw",
          "di":0,
          "state":true
        }
      ],
      "nCnt":1,
      "n":[
        {
          "node":"KeGaLi",
          "do":0,
          "setState":"Tog"
        }
      ]
    }
  ]
}
*/

class ConfigSwitch {
  public:
    // initial value
    String Json = "{\"t\":\"" + String(CONFIG_SWITCH_FILE) + "\",\"rev\":13,\"gCnt\":1,\"g\":[{\"name\":\"Toggle\",\"tCnt\":2,\"t\":[{\"node\":\"KeWkGaSw\",\"di\":0,\"state\":false},{\"node\":\"KeWkGaSw\",\"di\":0,\"state\":true}],\"nCnt\":1,\"n\":[{\"node\":\"KeGaLi\",\"do\":0,\"setState\":\"Tog\"}]}]}";

    ConfigSwitch() {}

    void Read() {
      String temp = ReadFile(CONFIG_SWITCH_FILE);

      if (temp == "") {
        WriteFile(CONFIG_SWITCH_FILE, Json);
      } else {
        Serial.println(String(CONFIG_SWITCH_FILE) + " " + temp);
        Update(temp);
      }
    }

    bool Update(String newConfig) {

      if (CompareRevision(Json, newConfig) == false) {
        return false;
      }

      Json = newConfig;
      Serial.println("updated configSwitch " + Json);

      String temp = ReadFile(CONFIG_SWITCH_FILE);
      
      if (CompareRevision(temp, newConfig) == false) {
        return false;
      }

      WriteFile(CONFIG_SWITCH_FILE, newConfig);
      Serial.println("updated " + String(CONFIG_SWITCH_FILE) + " " + newConfig);
      return true;
    }
};
