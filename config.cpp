#include "config.hpp"

int rotationWaitTimes[3] = {
    1000,
    1500,
    2000
};

Config::Config() {
    rotationSpeed = 1;
};

int Config::rotationWaitTime() {
    return rotationWaitTimes[rotationSpeed];
}