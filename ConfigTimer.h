
#define CONFIG_TIMER_FILE  "config_timer.json"

/*
config_timer.json
{
  "t":"config_timer.json",
  "rev":1,
  "timCnt":1,
  "tim":[
    {
      "name":"Licht AUS",
      "n":"KeWkLi",
      "t":"autoOff",
      "d":1
    }
  ]
}
*/

class ConfigTimer {
  public:
    // initial value
    String Json = "{\"t\":\"" + String(CONFIG_TIMER_FILE) + "\",\"rev\":1,\"timCnt\":1,\"tim\":[{\"name\":\"Licht AUS\",\"n\":\"KeWkLi\",\"d\":1}]}";

    ConfigTimer() {}

    void Read() {
      String temp = ReadFile(CONFIG_TIMER_FILE);

      if (temp == "") {
        WriteFile(CONFIG_TIMER_FILE, Json);
      } else {
        Serial.println(String(CONFIG_TIMER_FILE) + " " + temp);
        Update(temp);
      }
    }

    bool Update(String newConfig) {
      
      if (CompareRevision(Json, newConfig) == false) {
        return false;
      }

      Json = newConfig;
      Serial.println("updated configTimer " + Json);

      String temp = ReadFile(CONFIG_TIMER_FILE);

      if (CompareRevision(temp, newConfig) == false) {
        return false;
      }

      WriteFile(CONFIG_TIMER_FILE, newConfig);
      Serial.println("updated " + String(CONFIG_TIMER_FILE) + " " + newConfig);
      return true;
    }

    String GetType(String node) {

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, Json);

      for (int i = 0; i < doc["timCnt"].as<int>(); i++) {

        if (doc["tim"][i]["n"].as<String>() == node) {
          return doc["tim"][i]["t"].as<String>();
        }
      }

      return "";
    }

    uint32_t GetDuration(String node) {

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, Json);

      for (int i = 0; i < doc["timCnt"].as<int>(); i++) {

        if (doc["tim"][i]["n"].as<String>() == node) {
          return doc["tim"][i]["d"].as<uint32_t>();
        }
      }

      return 0;
    }
};
