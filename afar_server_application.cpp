#include "afar_server_application.h"
#include "afar_server_socket_rpc.h"
#include "instruments.h"
#include <fstream>
#include "rpc_ports.h"
#include "MWSwitchModule/MWSwitchModule.h"

#include "PCIMKOProvider.h"

AfarSrvAppl::AfarSrvAppl(QObject *parent) : QObject(parent)
{	
	
	//MessageBox(0, L"Crab", L"Crab", 32);
	connect(&SLogClass::Instance(), SIGNAL(toLogS(QString)), this, SIGNAL(toLog(QString)));
	connect(&SLogClass::Instance(), SIGNAL(toProtocolS(QString)), this, SIGNAL(toProtocol(QString)));
	
	
	//STmAikDBCtrl::Instance();

	QSettings sett(QCoreApplication::applicationDirPath() + "/afar_server_settings.ini", QSettings::IniFormat);
	QString ip_str = sett.value("afar_server_ip", instr::GetIpFromSettings("afar_server")).toString();
	int slot_port = sett.value("afar_server_slot_port", AFAR_SERVER_SLOT).toInt();
	int signal_port = sett.value("afar_server_signal_port", AFAR_SERVER_SIGNAL).toInt();

	afar_server_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new afar_server_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	afar_server_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new afar_server_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	try
	{
		SRMinit::Instance();

		addModule<MWSwitchModule>();

		toProtocol(QString("КПА готова к работе"));
	}
	catch (std::exception& err)
	{
		toProtocol(QString(err.what()));
	}

	db_ctrl = std::dynamic_pointer_cast<TMDBController>(std::shared_ptr<MongoTMDBController_v3>(new MongoTMDBController_v3("tm_afar_db", "ТМ АФАР")));
	db_ctrl->tryConnect("", instr::GetIpFromSettings("tm_afar_db"), false);
}

AfarSrvAppl::~AfarSrvAppl()
{


}


QString AfarSrvAppl::getXML()
{
	QFile file(":/afar_server_directives.xml");

	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		QString xml_string = file.readAll();
		return xml_string;
	}
	else
		return "";
}


int AfarSrvAppl::ZAPISAT_ZNACH_SIGNALA_BD(int code, QVariant& val)
{
	MsgFromTMDB tmp_msg;
	tmp_msg.dt = QDateTime::currentMSecsSinceEpoch();
	tmp_msg.tm_code = code;
	tmp_msg.value = val;
	std::vector<MsgFromTMDB> tmp_vect;
	tmp_vect.push_back(tmp_msg);
	db_ctrl->insertTMData(tmp_vect);
	return SUCCESS;
}

