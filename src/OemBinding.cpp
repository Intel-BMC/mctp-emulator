#include "OemBinding.hpp"

#include <sdbusplus/bus.hpp>

std::string mctpBindSMBus = "xyz.openbmc_project.MCTP.Binding.SMBus";
std::string mctpBindPCIe = "xyz.openbmc_project.MCTP.Binding.PCIe";
std::string mctpBindOem = "xyz.openbmc_project.MCTP.Binding.OEM";
std::string path = "/dev/i2c-8";
uint8_t arpMasterSupport = 0;
uint8_t bmcSlaveAddress = 0x12;
std::string pcieDiscoveredFlag = "xyz.openbmc_project.MCTP.Binding.PCIe.DiscoveryFlags.Discovered";

OemBinding::OemBinding(
    std::shared_ptr<sdbusplus::asio::object_server>& objServer,
    std::string& objPath, bindType bind) :
    MctpBinding(objServer, objPath)
{
    if (bind == bindType::smbus)
    {
        dbusInterface = objServer->add_interface(objPath, mctpBindSMBus);
        dbusInterface->register_property("ArpMasterSupport", arpMasterSupport);
        dbusInterface->register_property("BmcSlaveAddress", bmcSlaveAddress);
        dbusInterface->register_property("BusPath", path);
    }
    else if (bind == bindType::pcie)
    {
        dbusInterface = objServer->add_interface(objPath, mctpBindPCIe);
        dbusInterface->register_property("DiscoveredFlag", pcieDiscoveredFlag);
        dbusInterface->register_property("BDF", 512);
    }
    else
    {
        dbusInterface = objServer->add_interface(objPath, mctpBindOem);
    }
    dbusInterface->initialize();
}
