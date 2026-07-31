#include "settings.h"

#include <iostream>
#include <toml++/toml.h>

Settings* Settings::_instance = nullptr;

Settings::Settings() {
    _instance = this;
    load();
}

Settings::~Settings() {
    if (_instance == this) {
        _instance = nullptr;
    }
}

Settings* Settings::getInstance() {
    return _instance;
}

void Settings::load() {
    try {
        auto settings = toml::parse_file("settings.toml");
        renderDistance = settings["renderDistance"].value_or(renderDistance);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        save();
    }
}

void Settings::save() {
    auto table = toml::table{
        {"renderDistance", renderDistance},
    };

    std::ofstream out("settings.toml");
    if (out.is_open()) {
        out << table;
        out.close();
    } else {
        std::cerr << "Failed to open settings.toml" << std::endl;
    }
}
