#ifndef CLIENTVISUALIZEPLUGININTERFACE_H
#define CLIENTVISUALIZEPLUGININTERFACE_H

#include <qsize.h>
#include <qrect.h>
#include <QtPlugin>
#include <qpluginloader.h>
#include <qvector.h>
#include <qstringlist.h>
#include <qmutex.h>

class ClientVisualizePluginInterface
{
public:
	virtual ~ClientVisualizePluginInterface(){}
	virtual QWidget* getWidget() = 0;
	virtual void clearWidget() = 0;
};

Q_DECLARE_INTERFACE(ClientVisualizePluginInterface, "ClientVisualizeInterface_iid")

class VisualizePlugin
{
public:
	VisualizePlugin(const QString path = "");
	~VisualizePlugin();
	void unload();
	bool load();
	ClientVisualizePluginInterface* getInterface() const;
	const QString getLastError() const;
private:
	QString path;
	ClientVisualizePluginInterface* instance;
	QPluginLoader* loader;
	QString last_err;
};

#endif // CLIENTVISUALIZEPLUGININTERFACE_H
