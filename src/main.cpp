#include "OemBinding.hpp"

#include <CLI/CLI.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/steady_timer.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <phosphor-logging/log.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/bus.hpp>
#include <sstream>
#include <xyz/openbmc_project/MCTP/Base/server.hpp>
#include <xyz/openbmc_project/MCTP/Endpoint/server.hpp>
#include <xyz/openbmc_project/MCTP/SupportedMessageTypes/server.hpp>

using json = nlohmann::json;

std::shared_ptr<sdbusplus::asio::connection> bus;
std::string endpointDataFile = "/usr/share/mctp-emulator/endpoints.json";

int main()
{
    std::string mctpBaseObj = "/xyz/openbmc_project/mctp";
    std::string binding;

    std::string configPath = "/usr/share/mctp-emulator/binding_config.json";

    std::ifstream jsonfile(configPath);
    if (!jsonfile.is_open())
    {
        std::cerr << "The config file could not be opened " << std::endl;
        return -1;
    }

    json jsonConfig = json::parse(jsonfile, nullptr, false);
    if (jsonConfig.size() == 0 || !jsonConfig.contains("bindtype"))
    {
        std::cerr << "The config file is invalid " << std::endl;
        return -1;
    }

    binding = jsonConfig["bindtype"];

    boost::asio::io_context ioc;
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
        [&ioc](const boost::system::error_code&, const int&) { ioc.stop(); });

    bus = std::make_shared<sdbusplus::asio::connection>(ioc);

    std::string mctpServiceName = "xyz.openbmc_project.mctp-emulator";
    auto objectServer = std::make_shared<sdbusplus::asio::object_server>(bus);
    bus->request_name(mctpServiceName.c_str());

    auto objManager = std::make_shared<sdbusplus::server::manager::manager>(
        *bus, mctpBaseObj.c_str());

    // Create a virtual binding
    bindType val = bindType::undefined;
    if (!binding.compare("smbus"))
    {
        val = bindType::smbus;
    }
    else if (!binding.compare("pcie"))
    {
        val = bindType::pcie;
    }
    else
    {
        // default binding incase not known/undefined.
        val = bindType::vendorDefined;
    }

    OemBinding oemInstance(objectServer, mctpBaseObj, val);

    // Initialize endpoints using endpointDataFile json file
    oemInstance.addEndpoints(endpointDataFile);
    ioc.run();

    return 0;
}
