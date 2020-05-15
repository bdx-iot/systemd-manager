#include "systemd_manager.h"

SystemdManager::SystemdManager()
{
    sd_bus_open_system(&m_bus);
}

SystemdManager::~SystemdManager()
{
    sd_bus_unref(m_bus);
}

int SystemdManager::startService(std::string unit_name)
{
    int ret = -1;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *reply = NULL;
    char *object = nullptr;

    if (!m_bus) {
        goto finish;
    }

    ret = sd_bus_call_method(m_bus,
            SYSTEMD_SERVICE_NAME, /* service to contact */
            SYSTEMD_OBJECT_PATH_NAME, /* object path */
            SYSTEMD_MANAGER_INTERFACE_NAME, /* interface name */
            SYSTEMD_START_METHOD, /* method name */
            &error,/* object to return error in */
            &reply,
            "ss",
            unit_name.c_str(),
            "replace"
    );

    /* input signature */
    /* first argument */
    /* second argument */
    if (ret < 0) {
        std::cerr << "Failed to issue method call: " << error.message << std::endl;
        goto finish;
    }

    /* Parse the response message */
    ret = sd_bus_message_read(reply, "o", &object);
    if (ret < 0) {
        std::cerr <<  "Failed to parse response message: " << std::strerror(-ret) << std::endl;
        goto finish;
    }

    std::cout << "Object " << object << std::endl;
    goto finish;

finish:
    sd_bus_error_free(&error);
    sd_bus_message_unref(reply);

    return ret;
}


int main(int argc, char *argv[])
{
    //SystemdManager s;
    //s.startService("foo.service");
}
