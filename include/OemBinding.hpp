#pragma once

#include "MCTPBinding.hpp"

using DBusInterfacePtr = std::shared_ptr<sdbusplus::asio::dbus_interface>;

class OemBinding : public MctpBinding
{
  public:
    OemBinding() = delete;
    OemBinding(std::shared_ptr<sdbusplus::asio::object_server>& objServer,
               std::string& objPath, bindType bind);
    ~OemBinding() = default;

  private:
    DBusInterfacePtr dbusInterface = nullptr;
};
