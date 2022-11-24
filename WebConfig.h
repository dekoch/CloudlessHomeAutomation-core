
#include "ESPAsyncWebServer.h"

const char* API_CONFIG_MESH_WRITE = "/api/config/mesh/write";
const char* API_CONFIG_SWITCH_WRITE = "/api/config/switch/write";
const char* API_CONFIG_TIMER_WRITE = "/api/config/timer/write";
const char* PARAM_JSON = "json";
const char* PARAM_PAGE = "page";

AsyncWebServer* serverConfig;
String lastPage = "";


String GetRedirectToPage(String page) {
    return "<meta http-equiv=\"refresh\" content=\"0; url=/?page=" + page + "\" />";
}

String GetTextarea(String title, String action, String content) {

    String ret = title + "<br>\
    " + action + "\
    <form method=\"post\" enctype=\"text/plain\" action=\"" + action + "\">\
        <div style=\"width: 100%; height: 75%;\">\
            <textarea name=\'json\' style=\"width: 100%; height: 100%;\">";
    ret += content;
    ret += "</textarea>\
        </div>\
        <br>\
        <button class=\"button buttonGrey\" type=\"submit\">Submit</button>\
    </form>";

    return ret;
}

String GetPage(String page){

    String body = "";

    if (page == "home") {
        body = "<h1>Home</h1><br>";
    } else if (page == "mesh") {
        body = GetTextarea("Mesh", API_CONFIG_MESH_WRITE, configMesh.Json);
    } else if (page == "switch") {
        body = GetTextarea("Switch", API_CONFIG_SWITCH_WRITE, configSwitch.Json);
    } else if (page == "timer") {
        body = GetTextarea("Timer", API_CONFIG_TIMER_WRITE, configTimer.Json);
    }

    String content = "<html>\
    <head>\
        <title>";
    content += MESH_SSID;
    content += "</title>\
        <style>\
        html { font-family: Courier New; display: inline-block; margin: 0px auto; text-align: center;}\
        .button {border: none; color: white; padding: 10px 20px; text-align: center;\
        text-decoration: none; font-size: 25px; margin: 2px; cursor: pointer;}\
        .button1 {background-color: #13B3F0;}\
        .button2 {background-color: #3342FF;}\
        .buttonGrey {background-color: #b3b3b3;}\
        </style>\
    </head>\
    <body>\
        <a href=\"/\"><button class=\"button buttonGrey\">Home</button></a>\
        <a href=\"/?page=mesh\"><button class=\"button buttonGrey\">Mesh</button></a>\
        <a href=\"/?page=switch\"><button class=\"button buttonGrey\">Switch</button></a>\
        <a href=\"/?page=timer\"><button class=\"button buttonGrey\">Timer</button></a><br>";
    content += body;
    content += "</body>\
    </html>";

    return content;
}

void NotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void WebConfigSetup() {

    configMesh.Read();
    configSwitch.Read();
    configTimer.Read();

    serverConfig = new AsyncWebServer(CONFIG_PORT);

    // <IP>?page=home
    serverConfig->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String page;
        if (request->hasParam(PARAM_PAGE)) {
            page = request->getParam(PARAM_PAGE)->value();
        } else {
            page = "No page sent";
        }

        Serial.println(page);
        lastPage = page;

        request->send(200, "text/html", GetPage(page));
    });

    serverConfig->on(API_CONFIG_MESH_WRITE, HTTP_POST, [](AsyncWebServerRequest *request){
        String json;
        if (request->hasParam(PARAM_JSON, true)) {
            json = request->getParam(PARAM_JSON, true)->value();
        } else {
            json = "No json sent";
        }

        //Serial.println(json);

        configMesh.Update(json);
        mesh.sendBroadcast(json);
        Serial.println("Send msg=" + json);

        request->send(200, "text/html", GetRedirectToPage(lastPage));
    });

    serverConfig->on(API_CONFIG_SWITCH_WRITE, HTTP_POST, [](AsyncWebServerRequest *request){
        String json;
        if (request->hasParam(PARAM_JSON, true)) {
            json = request->getParam(PARAM_JSON, true)->value();
        } else {
            json = "No json sent";
        }

        //Serial.println(json);

        configSwitch.Update(json);
        mesh.sendBroadcast(json);
        Serial.println("Send msg=" + json);

        request->send(200, "text/html", GetRedirectToPage(lastPage));
    });

    serverConfig->on(API_CONFIG_TIMER_WRITE, HTTP_POST, [](AsyncWebServerRequest *request){
        String json;
        if (request->hasParam(PARAM_JSON, true)) {
            json = request->getParam(PARAM_JSON, true)->value();
        } else {
            json = "No json sent";
        }

        //Serial.println(json);

        configTimer.Update(json);
        mesh.sendBroadcast(json);
        Serial.println("Send msg=" + json);

        request->send(200, "text/html", GetRedirectToPage(lastPage));
    });

    serverConfig->onNotFound(NotFound);
    serverConfig->begin();
}