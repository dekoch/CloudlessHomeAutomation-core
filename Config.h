
uint32_t GetRevision(String json) {

  if (json == ""){
    return 0;
  }

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  return doc["rev"].as<uint32_t>();
}

// return true if newJson is newer
bool CompareRevision(String oldJson, String newJson) {

  uint32_t oldRev = GetRevision(oldJson);
  uint32_t newRev = GetRevision(newJson);

  //Serial.println("oldRev=" + String(oldRev) + " newRev=" + String(newRev));

  if (oldRev >= newRev) {
    //Serial.println("keep oldJson " + oldJson);
    return false;
  }

  return true;
}