#ifndef SYSTEMD_MANAGER_H
#define SYSTEMD_MANAGER_H


#include <systemd/sd-daemon.h>
#include <systemd/sd-bus.h>
#include <stdbool.h>
#include <cstring>
#include <string>
#include <iostream>

#define SYSTEMD_UNIT_INTERFACE_NAME "org.freedesktop.systemd1.Unit"
#define SYSTEMD_SERVICE_INTERFACE_NAME  "org.freedesktop.systemd1.Service"

#define SYSTEMD_SERVICE_NAME "org.freedesktop.systemd1"
#define SYSTEMD_OBJECT_PATH_NAME "/org/freedesktop/systemd1"
#define SYSTEMD_MANAGER_INTERFACE_NAME "org.freedesktop.systemd1.Manager"

#define SYSTEMD_START_METHOD "StartUnit"

class SystemdManager{
public:
    SystemdManager();
    ~SystemdManager();
    int startService(std::string unit_name);
    int stopService(std::string unit_name);
    bool isActive(std::string unit_name);
    bool isFailed(std::string unit_name);
private:
    sd_bus *m_bus = nullptr;
};


#endif // SYSTEMD_MANAGER_H