
/*
https://github.com/s00500/ESPUI
*/

#include <ESPUI.h>

uint16_t button1;

void buttonExample(Control* sender, int type, void* param)
{
    Serial.println(String("param: ") + String(int(param)));
    switch (type)
    {
    case B_DOWN:
        Serial.println("Status: Start");
        /*ESPUI.updateControlValue(status, "Start");

        ESPUI.getControl(button1)->color = ControlColor::Carrot;
        ESPUI.updateControl(button1);*/
        break;

    case B_UP:
        Serial.println("Status: Stop");
       /* ESPUI.updateControlValue(status, "Stop");

        ESPUI.getControl(button1)->color = ControlColor::Peterriver;
        ESPUI.updateControl(button1);*/
        break;
    }
}

void textCall(Control* sender, int type)
{
    Serial.print("Text: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void WebUiSetup(){

    uint16_t tabHome = ESPUI.addControl(ControlType::Tab, "Home", "Home");
    uint16_t tabConfigMesh = ESPUI.addControl(ControlType::Tab, "Config Mesh", "Config Mesh");
        
    uint16_t tabConfigSwitch = ESPUI.addControl(ControlType::Tab, "Config Switch", "Config Switch");

    ESPUI.addControl(ControlType::Label, "Millis:", "0", ControlColor::Emerald, tabConfigMesh);
    //button1 = ESPUI.addControl(ControlType::Button, "Push Button", "Press", ControlColor::Peterriver, tabConfigMesh, &buttonCallback);
    
    uint16_t file = ESPUI.text("Date", textCall, ControlColor::Dark, "2022-05-24");
    ESPUI.setInputType(file, "file");

    ESPUI.addControl(ControlType::Text, "", "", ControlColor::None, file);

    uint16_t submitFile = ESPUI.addControl(ControlType::Button, "Other Button", "submit", ControlColor::Wetasphalt, tabConfigMesh, &buttonExample, (void*)19);
    ESPUI.setInputType(submitFile, "submit");


    ESPUI.addControl(ControlType::Button, "Other Button", "Press", ControlColor::Wetasphalt, tabConfigMesh, &buttonExample, (void*)19);

    ESPUI.begin(MESH_SSID);
}

void WebUiLoop(){
    
}