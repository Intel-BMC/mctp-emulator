#pragma once

#include <libmctp.h>

#include <sdbusplus/asio/object_server.hpp>

using EndpointInterfaceMap =
    std::unordered_map<mctp_eid_t,
                       std::shared_ptr<sdbusplus::asio::dbus_interface>>;

extern std::shared_ptr<sdbusplus::asio::connection> bus;


// TODO:Use the hpp from D-Bus interface
enum class bindType
{
    undefined = 0x0,
    smbus = 0x01,
    pcie = 0x02,
    usb = 0x03,
    kcs = 0x04,
    serial = 0x05,
    vendorDefined = 0xFF
};

class MctpBinding
{
  public:
    MctpBinding(std::shared_ptr<sdbusplus::asio::object_server>& objServer,
                std::string& objPath);
    MctpBinding() = delete;
    void addEndpoints(std::string file, std::optional<uint8_t> destId = std::nullopt);
    ~MctpBinding();

  private:
    uint8_t eid;
    std::shared_ptr<sdbusplus::asio::object_server> objectServer;
    EndpointInterfaceMap endpointInterface;
    EndpointInterfaceMap msgInterfaces;
    EndpointInterfaceMap vendorInterfaces;
    EndpointInterfaceMap uuidInterfaces;
    void getSystemAppUuid(void);
    bool removeInterface(mctp_eid_t dstEid, EndpointInterfaceMap& interfaces);
};
