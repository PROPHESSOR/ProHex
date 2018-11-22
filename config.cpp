#include "config.h"

Config::Config() {
    v_show_address  = true;
    v_show_hex      = true;
    v_show_ascii    = true;
}

bool Config::load() {
    QFile saveFile(Config::getConfigFilePath());

    if(!saveFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open config.json file for reading... Trying to create new...");
        return save();
    }

    QByteArray jsonData = saveFile.readAll();

    QJsonDocument parsedJson(QJsonDocument::fromJson(jsonData));

    return loadFromJson(parsedJson.object());
}

bool Config::loadFromJson(QJsonObject json) {
    if(!json.contains("view") || !json["view"].isObject()) {
        qWarning("Config file doesn't contain view settings! Recreating config.json...");
        return save();
    }

    QJsonObject view = json["view"].toObject();

    if(view.contains("showAddress") && view["showAddress"].isBool())    v_show_address  = view["showAddress"].toBool();
    if(view.contains("showHex") && view["showHex"].isBool())            v_show_hex      = view["showHex"].toBool();
    if(view.contains("showAscii") && view["showAscii"].isBool())        v_show_ascii    = view["showAscii"].toBool();

    return true;
}

bool Config::save() {
    QFile saveFile(Config::getConfigFilePath());

    if(!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open config.json file for writing!");
        return false;
    }

    QJsonObject json;

    if(!saveToJson(json)) {
        qWarning("Couldn't save local settings to json view!");
        return false;
    };

    QJsonDocument jsonDocument(json);

    saveFile.write(jsonDocument.toJson());

    return true;
}

bool Config::saveToJson(QJsonObject &json) {
    QJsonObject view;

    view["showAddress"] = v_show_address;
    view["showHex"]     = v_show_hex;
    view["showAscii"]   = v_show_ascii;

    json["view"] = view;

    return true;
}

QString Config::getConfigFilePath() {
    return "config.json";
}

// Getters

bool Config::getViewShowAddress() {
    return v_show_address;
}

bool Config::getViewShowHex() {
    return v_show_hex;
}

bool Config::getViewShowAscii() {
    return v_show_ascii;
}

// Setters

void Config::setViewShowAddress(bool val) {
    v_show_address = val;
}

void Config::setViewShowHex(bool val) {
    v_show_hex = val;
}

void Config::setViewShowAscii(bool val) {
    v_show_ascii = val;
}
