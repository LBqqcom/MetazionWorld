#include "PacketHandlerLM.hpp"

#include <Metazion/Share/Misc/MemoryOutputStream.hpp>

#include "Common/Packet/PacketML.hpp"
#include "Common/Packet/PacketLM.hpp"

#include "ServerApp.hpp"

void PacketHandlerLM::Handle(int command, const void* data, int length) {
    ::printf("Command[%d] data[%p] length[%d]\n", command, data, length);

    switch (command) {
    case COMMAND_LM_CONNECTED:
        HandleConnected(data, length);
        break;
    case COMMAND_LM_DISCONNECTED:
        HandleDisconnected(data, length);
        break;
    default: break;
    }
}

void PacketHandlerLM::HandleConnected(const void* data, int length) {
    NS_MZ_SHARE::MemoryOutputStream<> outputStream;

    auto& masterConfig = g_serverApp->m_serverConfigManager.GetMasterConfig();
    outputStream.WriteInt8(masterConfig.m_id);

    const auto gatewayInfoSize = g_serverApp->m_serverConfigManager.GetGatewayInfoSize();
    outputStream.WriteInt8(gatewayInfoSize);

    const auto& gatewayInfoMap = g_serverApp->m_serverConfigManager.GetAllGatewayInfo();
    for (auto pair : gatewayInfoMap) {
        const auto& gatewayInfo = pair.second;
        const auto address = gatewayInfo.m_address;

        outputStream.WriteUint32(address.m_ip);
        outputStream.WriteUint16(address.m_port);
    }
    g_serverApp->m_socketML->SendData(COMMAND_ML_SERVERREGISTER
        , outputStream.GetBuffer(), outputStream.GetLength());
}

void PacketHandlerLM::HandleDisconnected(const void* data, int length) {
    
}