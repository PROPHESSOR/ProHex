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
    if(json.isEmpty()) {
        qWarning("Config file doesn't contain settings! Recreating config.json...");
        return save();
    }

    if(json.contains("view") && json["view"].isObject()) {
        QJsonObject view = json["view"].toObject();
        if(view.contains("showAddress") && view["showAddress"].isBool())            v_show_address  = view["showAddress"].toBool();
        if(view.contains("showHex") && view["showHex"].isBool())                    v_show_hex      = view["showHex"].toBool();
        if(view.contains("showAscii") && view["showAscii"].isBool())                v_show_ascii    = view["showAscii"].toBool();
    }

    if(json.contains("application") && json["application"].isObject()) {
        QJsonObject application = json["application"].toObject();
        if(application.contains("language") && application["language"].isString())  a_language      = application["language"].toString();
        if(application.contains("windowTheme") && application["windowTheme"].isDouble()) a_window_theme = application["windowTheme"].toInt();
        if(application.contains("largelist") && application["largelist"].isDouble()) a_largelist = application["largelist"].toInt();
    }

    if(json.contains("colorscheme") && json["colorscheme"].isObject()) {
        QJsonObject colorscheme = json["colorscheme"].toObject();
        if(colorscheme.contains("addressArea") && colorscheme["addressArea"].isString())
            c_address_area  = QColor(colorscheme["addressArea"].toString());
        if(colorscheme.contains("selection") && colorscheme["selection"].isString())
            c_selection     = QColor(colorscheme["selection"].toString());
        if(colorscheme.contains("cursor") && colorscheme["cursor"].isString())
            c_cursor        = QColor(colorscheme["cursor"].toString());
        if(colorscheme.contains("hex") && colorscheme["hex"].isString())
            c_hex           = QColor(colorscheme["hex"].toString());
        if(colorscheme.contains("address") && colorscheme["address"].isString())
            c_address       = QColor(colorscheme["address"].toString());
        if(colorscheme.contains("ascii") && colorscheme["ascii"].isString())
            c_ascii         = QColor(colorscheme["ascii"].toString());
        if(colorscheme.contains("activeWindow") && colorscheme["activeWindow"].isString())
            c_active_window = QColor(colorscheme["activeWindow"].toString());
    }

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

    application["language"]     = a_language;
    application["windowTheme"]  = a_window_theme;
    application["largelist"]    = a_largelist;

    json["application"]         = application;

    QJsonObject colorscheme;

    colorscheme["addressArea"]  = c_address_area.name();
    colorscheme["selection"]    = c_selection.name();
    colorscheme["cursor"]       = c_cursor.name();
    colorscheme["hex"]          = c_hex.name();
    colorscheme["address"]      = c_address.name();
    colorscheme["ascii"]        = c_ascii.name();
    colorscheme["activeWindow"] = c_active_window.name();

    json["colorscheme"] = colorscheme;

    return true;
}

void Config::reset() {
    v_show_address  = true;
    v_show_hex      = true;
    v_show_ascii    = true;

    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));
    a_language      = locale;
    a_window_theme  = 0; // Light
    a_largelist     = 10000000;

    c_address_area  = QColor(0xd4, 0xd4, 0xd4, 0xff);
    c_selection     = QColor(0x6d, 0x9e, 0xff, 0xff);
    c_cursor        = QColor(0xcc, 0xcc, 0xcc, 0xff);

    c_hex           = QColor(0x00, 0x00, 0x00, 0xff);
    c_address       = QColor(0x00, 0x00, 0x00, 0xff);
    c_ascii         = QColor(0x00, 0x00, 0x00, 0xff);

    c_active_window = QColor(0x00, 0x00, 0xff, 0xff);
}

QString Config::getConfigFilePath() {
    // return QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)).absoluteFilePath("prohex_config.json");
    return QDir("./prohex_config.json").absolutePath();
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

uint8_t Config::getWindowTheme() {
    return a_window_theme;
}

QString Config::getLanguage() {
    return a_language;
}

int Config::getLargelist() {
    return a_largelist;
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

void Config::setWindowTheme(uint8_t theme) {
    a_window_theme = theme;
}

void Config::setLanguage(QString lang) {
    if(lang == "en" || lang == "ru" || lang == "uk") {
        a_language = lang;
    } else qDebug() << "Config::setLanguage ERROR: Unknown language " << lang;
}

void Config::setLargelist(int value) {
    a_largelist = value;
}
