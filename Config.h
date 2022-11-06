
uint32_t GetRevision(String json) {

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  return doc["rev"].as<uint32_t>();
}
