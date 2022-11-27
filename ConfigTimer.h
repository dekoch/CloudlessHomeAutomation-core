
#define CONFIG_TIMER_FILE  "config_timer.json"

/*
config_timer.json
{
  "t":"config_timer.json",
  "rev":2,
  "nCnt":1,
  "n":[
    {
      "name":"D1 2137932489 AP_STA",
      "timCnt":2,
      "tim":[
        {
          "name":"Licht AUS",
          "t":"DO0",
          "ts":"S",
          "d":1,
          "u":"s",
          "do":0,
          "s":"R"
        },
        {
          "name":"Licht AN",
          "t":"DO0",
          "ts":"R",
          "d":1,
          "u":"s",
          "do":0,
          "s":"S"
        }
      ]
    }
  ]
}
*/

class TimerItem {
  public:
    String Name;
    String Trigger;
    String TriggerState;
    uint32_t Duration;
    String Unit;
    int Output;
    String SetState;

    TimerItem() {}

    TimerItem(DynamicJsonDocument doc) {
      Name = doc["name"].as<String>();
      Trigger = doc["t"].as<String>();
      TriggerState = doc["ts"].as<String>();
      Duration = doc["d"].as<uint32_t>();
      Unit = doc["u"].as<String>();
      Output = doc["do"].as<int>();
      SetState = doc["s"].as<String>();
    }

    void CheckTrigger(bool state) {

      if (Trigger == "") {
        return;
      }

      if (_edgeStart.rising(state && TriggerState == "S" ||
                            state == false && TriggerState == "R")) {

        Start();
      }
    }

    void Start() {

      if (Trigger == "") {
        return;
      }

      // millis
      unsigned long interval = Duration;

      if (Unit == "s") {
        interval *= 1000;
      } else if (Unit == "m") {
        interval *= 60 * 1000;
      } else if (Unit == "h") {
        interval *= 60 * 60 * 1000;
      }

      _delay.Interval(interval);
      _delay.Reset();
      _delay.Start();

      Serial.println("Trigger " + Name + " (" + Duration + Unit + ")");
    }

    bool Elapsed() {

      if (Trigger == "") {
        return false;
      }

      bool elapsed = _delay.Elapsed();

      if (elapsed) {
        _delay.Stop();
      }

      return elapsed;
    }

  private:
    Timer _delay = Timer();
    Edge _edgeStart = Edge();
};

class ConfigTimer {
  public:
    // initial value
    String Json = "{\"t\":\"" + String(CONFIG_TIMER_FILE) + "\",\"rev\":2,\"nCnt\":1,\"n\":[{\"name\":\"D1 2137932489 AP_STA\",\"timCnt\":2,\"tim\":[{\"name\":\"Licht AN\",\"t\":\"DO0\",\"ts\":\"R\",\"d\":7,\"u\":\"s\",\"do\":0,\"s\":\"S\"},{\"name\":\"Licht AUS\",\"t\":\"DO0\",\"ts\":\"S\",\"d\":5,\"u\":\"s\",\"do\":0,\"s\":\"R\"}]}]}";

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

    TimerItem GetItems(String node, int index) {

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, Json);

      for (int n = 0; n < doc["nCnt"].as<int>(); n++) {
        // search node name
        if (doc["n"][n]["name"].as<String>() == node) {
          
          for (int t = 0; t < doc["n"][n]["timCnt"].as<int>(); t++) {

            if (t != index) {
              continue;
            }

            Serial.println("timer" + String(index) + ": " + doc["n"][n]["tim"][t].as<String>());

            deserializeJson(doc, doc["n"][n]["tim"][t].as<String>());
            return TimerItem(doc);
          }
        }
      }

      Serial.println("timer" + String(index) + ": not set");
      return TimerItem();
    }
};
