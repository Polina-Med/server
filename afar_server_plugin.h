#ifndef AFAR_SERVER_PLUGIN_H
#define AFAR_SERVER_PLUGIN_H

#include <qobject.h>
#include "ServerPluginInterface.hpp"

class AfarServerPlugin : public QObject, public ServerPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "AfarServerPlugin");
	Q_INTERFACES(ServerPluginInterface)

public:
	virtual ~AfarServerPlugin() { if (_interface) delete _interface; }
	virtual void connect(const QString& ip);
	virtual QString get_name() { return "afar_server"; }
};

#endif // AFAR_SERVER_PLUGIN_H


