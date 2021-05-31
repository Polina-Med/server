#ifndef AFAR_SERVER_CLIENT_VISUALIZE_PLUGIN_H
#define AFAR_SERVER_CLIENT_VISUALIZE_PLUGIN_H

#include <qvariant.h>
#include <qobject.h>
#include <QWidget>
#include <QLabel>
#include "ClientVisualizePluginInterface.hpp"
//#include "common_classes.h"

#include "TMDBController.hpp"
#include "DBThread.h"

class DBCreator : public QObject
{
	Q_OBJECT
public:

	void convertData(const std::vector<QVariant>& msg_list);
signals:
	void needProcessData(const std::vector<QVariant>& msg_list);
};

class AfarWidgetVisualize :public QWidget
{
	Q_OBJECT

public:
	static QString GreySt;
	static QString GreenSt;
	static QString Grey;
	AfarWidgetVisualize(QWidget* parent = 0);


private:
	DBCreator db_cr;
	DBReaderThread db_th;

	QLabel *test_lab;
	QLabel *rez_mko;
	QLabel *ist_pit;
	QLabel *afar;
	QLabel *power;
	QLabel *rez;
	int p_pow1;
	int p_pow2;
	QLabel *pow1;
	QLabel *pow2;
	QLabel *vol1;
	QLabel *vol2;
	QLabel *amp1;
	QLabel *amp2;
	bool sum_power;
	void updateAfar();
public slots:

	int MsgFromDB(const std::vector<QVariant>& msg_list);
};

class AfarClientVisualizePlugin : public QObject, public ClientVisualizePluginInterface
{
	Q_OBJECT
		Q_PLUGIN_METADATA(IID "AfarServerClientVisualizePlugin");
	Q_INTERFACES(ClientVisualizePluginInterface)

public:
	virtual QWidget* getWidget();
	virtual void clearWidget();
private:
	static AfarWidgetVisualize* widg;

};


#endif
//AFAR_SERVER_CLIENT_VISUALIZE_PLUGIN_H