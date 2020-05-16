#include "systemd_manager.h"

SystemdManager::SystemdManager()
{
    sd_bus_open_system(&m_bus);
}

SystemdManager::~SystemdManager()
{
    sd_bus_unref(m_bus);
}

int SystemdManager::startService(std::string unit_name, bool stop = false)
{
    int ret = -1;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *reply = NULL;
    char *object = nullptr;

    if (!m_bus)
    {
        goto finish;
    }

    ret = sd_bus_call_method(m_bus,
                             SYSTEMD_SERVICE_NAME,                              /* service to contact */
                             SYSTEMD_OBJECT_PATH_NAME,                          /* object path */
                             SYSTEMD_MANAGER_INTERFACE_NAME,                    /* interface name */
                             stop ? SYSTEMD_STOP_METHOD : SYSTEMD_START_METHOD, /* method name */
                             &error,                                            /* object to return error in */
                             &reply,
                             "ss",
                             unit_name.c_str(),
                             "replace");

    /* input signature */
    /* first argument */
    /* second argument */
    if (ret < 0)
    {
        std::cerr << "Failed to issue method call: " << error.message << std::endl;
        goto finish;
    }

    /* Parse the response message */
    ret = sd_bus_message_read(reply, "o", &object);
    if (ret < 0)
    {
        std::cerr << "Failed to parse response message: " << std::strerror(-ret) << std::endl;
        goto finish;
    }

    std::cout << "Object " << object << std::endl;

finish:
    sd_bus_error_free(&error);
    sd_bus_message_unref(reply);

    return ret;
}

bool SystemdManager::isActive(std::string unit_name)
{
    int ret;
    bool state = false;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *reply = NULL;
    char *unit_path = nullptr;
    char *active_state = nullptr;

    if (!m_bus)
    {
        goto finish;
    }

    ret = sd_bus_call_method(m_bus,
                             SYSTEMD_SERVICE_NAME, /* service to contact */
                             SYSTEMD_OBJECT_PATH_NAME, /* object path */
                             SYSTEMD_MANAGER_INTERFACE_NAME, /* interface name */
                             SYSTEMD_LOAD_UNIT_METHOD, /* method name */
                             &error,/* object to return error in */
                             &reply,
                             "s",
                             unit_name.c_str()
                             );

    /* input signature */
    /* first argument */
    /* second argument */
    if (ret < 0) {
        std::cerr << "Failed to issue method call: " << error.message << std::endl;
        goto finish;
    }

    /* Parse the response message */
    ret = sd_bus_message_read(reply, "o", &unit_path);
    if (ret < 0) {
        std::cerr <<  "Failed to parse response message: " << std::strerror(-ret) << std::endl;
        goto finish;
    }

    std::cout << "Unit path :" << unit_path << std::endl;

    ret = sd_bus_get_property_string(m_bus,
                             SYSTEMD_SERVICE_NAME, /* service to contact */
                             unit_path, /* object path */
                             SYSTEMD_UNIT_INTERFACE_NAME, /* interface name */
                             "ActiveState",
                             &error,/* object to return error in */
                             &active_state
                             );

    if (ret < 0) {
        std::cerr << "Failed to issue method call: " << error.message << std::endl;
        goto finish;
    }

    if (strcmp(active_state, "active") == 0)
    {
        state = true;
    }

finish:
    sd_bus_error_free(&error);
    sd_bus_message_unref(reply);
    if (active_state) {
        free(active_state);
    }

    return state;
}


int main(int argc, char *argv[])
{
    //SystemdManager s;
    //s.startService("foo.service");
}
