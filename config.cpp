#include "config.h"

Config::Config() {
    reset();
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

    if(view.contains("showAddress") && view["showAddress"].isBool())            v_show_address  = view["showAddress"].toBool();
    if(view.contains("showHex") && view["showHex"].isBool())                    v_show_hex      = view["showHex"].toBool();
    if(view.contains("showAscii") && view["showAscii"].isBool())                v_show_ascii    = view["showAscii"].toBool();

    QJsonObject application = json["application"].toObject();

    if(application.contains("language") && application["language"].isString())  a_language      = application["language"].toString();

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

    view["showAddress"]     = v_show_address;
    view["showHex"]         = v_show_hex;
    view["showAscii"]       = v_show_ascii;

    json["view"]            = view;

    QJsonObject application;

    application["language"] = a_language;

    json["application"]     = application;

    return true;
}

void Config::reset() {
    v_show_address  = true;
    v_show_hex      = true;
    v_show_ascii    = true;

    a_language      = "English";

    c_address_area  = QColor(0xd4, 0xd4, 0xd4, 0xff);
    c_selection     = QColor(0x6d, 0x9e, 0xff, 0xff);
    c_cursor        = QColor(0xcc, 0xcc, 0xcc, 0xff);

    c_hex           = QColor(0x00, 0x00, 0x00, 0xff);
    c_address       = QColor(0x00, 0x00, 0x00, 0xff);
    c_ascii         = QColor(0x00, 0x00, 0x00, 0xff);

    c_active_window = QColor(0x00, 0x00, 0xff, 0xff);
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

QColor Config::getColorAddressArea() {
    return c_address_area;
}

QColor Config::getColorSelection() {
    return c_selection;
}

QColor Config::getColorCursor() {
    return c_cursor;
}

QColor Config::getColorHex() {
    return c_hex;
}

QColor Config::getColorAddress() {
    return c_address;
}

QColor Config::getColorAscii() {
    return c_ascii;
}

QColor Config::getColorActiveWindow() {
    return c_active_window;
}

QString Config::getLanguage() {
    return a_language;
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

void Config::setLanguage(QString lang) {
    if(lang == "English" || lang == "Русский") {
        a_language = lang;
    } else qDebug() << "Config::setLanguage ERROR: Unknown language " << lang;
}
