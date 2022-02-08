#include "OemBinding.hpp"

#include <sdbusplus/bus.hpp>

std::string mctpBindSMBus = "xyz.openbmc_project.MCTP.Binding.SMBus";
std::string mctpBindPCIe = "xyz.openbmc_project.MCTP.Binding.PCIe";
std::string mctpBindOem = "xyz.openbmc_project.MCTP.Binding.OEM";

OemBinding::OemBinding(
    std::shared_ptr<sdbusplus::asio::object_server>& objServer,
    std::string& objPath, bindType bind) :
    MctpBinding(objServer, objPath)
{
    if (bind == bindType::smbus)
    {
        dbusInterface = objServer->add_interface(objPath, mctpBindSMBus);
    }
    else if (bind == bindType::pcie)
    {
        dbusInterface = objServer->add_interface(objPath, mctpBindPCIe);
    }
    else
    {
    	dbusInterface = objServer->add_interface(objPath, mctpBindOem);
    }
    dbusInterface->initialize();
}
