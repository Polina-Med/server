#include "afar_server_plugin.h"
#include "afar_server_rpc.h"
#include "rpc_ports.h"

typedef RPC_afar_server_SLOT_Thread RMS_SLOT;
typedef RPC_afar_server_SIGNAL_Thread RMS_SIGNAL;

void AfarServerPlugin::connect(const QString& ip)
{
	_interface = new ServerInterface(std::shared_ptr<RMS_SLOT>(new RMS_SLOT),
		std::shared_ptr<RMS_SLOT>(new RMS_SLOT),
		std::shared_ptr<RMS_SIGNAL>(new RMS_SIGNAL));
	_interface->connect_to_srv(ip, AFAR_SERVER_SLOT, AFAR_SERVER_SIGNAL);
}