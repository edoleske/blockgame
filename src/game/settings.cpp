#include "settings.h"

#include <toml++/toml.h>
#include "log.h"

Settings* Settings::_instance = nullptr;

Settings::Settings() {
    LOG_DEBUG("Initializing settings system");
    _instance = this;
    load();
}

Settings::~Settings() {
    LOG_DEBUG("Destructing settings system");
    if (_instance == this) {
        _instance = nullptr;
    }
}

Settings* Settings::getInstance() {
    return _instance;
}

void Settings::load() {
    if (!fs::exists("settings.toml")) {
        LOG_DEBUG("Creating default settings.toml file");
        save();
        return;
    }

    try {
        auto settings = toml::parse_file("settings.toml");
        renderDistance = settings["renderDistance"].value_or(renderDistance);
    } catch (std::exception& e) {
        LOG_ERROR("Error parsing settings: {}", e.what());
    }
}

void Settings::save() {
    toml::table table {
        {"renderDistance", renderDistance}
    };

    std::ofstream out("settings.toml");
    if (out.is_open()) {
        out << table;
        out.close();
    } else {
        LOG_ERROR("Unable to open settings.toml for writing");
    }
}
