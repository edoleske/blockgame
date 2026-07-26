#include "settings.h"

Settings* Settings::_instance = nullptr;

Settings::Settings() {
    _instance = this;
}

Settings::~Settings() {
    if (_instance == this) {
        _instance = nullptr;
    }
}

Settings* Settings::getInstance() {
    return _instance;
}
