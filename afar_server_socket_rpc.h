#ifndef AFAR_SERVER_SOCKET_RPC_H
#define AFAR_SERVER_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "afar_server_application.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class afar_server_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	afar_server_Socket_RPC_SIGNAL_Object();
	~afar_server_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(AfarSrvAppl* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void toLog(QString message);
	void toProtocol(QString message);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	AfarSrvAppl* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class afar_server_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	afar_server_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(AfarSrvAppl* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	AfarSrvAppl* app;
	QList<std::shared_ptr<afar_server_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class afar_server_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	afar_server_Socket_RPC_SIGNAL_Thread();
	void set_app(AfarSrvAppl* _app)
	{
		app = _app;
	}
	void set_params(QString _conn_ip, int _conn_port)
	{
		conn_ip = _conn_ip;
		conn_port = _conn_port;
	}
	void run();
private:
	afar_server_Socket_RPC_SIGNAL_Server* rpc_srv;
	AfarSrvAppl* app;
	QString conn_ip;
	int conn_port;
};

class afar_server_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	afar_server_Socket_RPC_SLOT_Object(AfarSrvAppl* _app, int socketDescriptor);
	~afar_server_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (afar_server_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant getXML(QVariantList& _values);
	QVariant OPROS_IP(QVariantList& _values);
	QVariant DOBAVIT_PRIBOR_AFAR(QVariantList& _values);
	QVariant UDALIT_PRIBOR_AFAR(QVariantList& _values);
	QVariant PODKLYUCHIT_PRIBOR_AFAR(QVariantList& _values);
	QVariant OTKLYUCHIT_PRIBOR_AFAR(QVariantList& _values);
	QVariant ZAGRUZIT_KOMANDU_P_AFAR(QVariantList& _values);
	QVariant POLUCHIT_SOOBSSHENIE_P_AFAR(QVariantList& _values);
	QVariant UPRAVLENIE_SPDT_KOMMUTATORAMI(QVariantList& _values);
	QVariant VKL_TSIKL_OPROS_PRIBORA(QVariantList& _values);
	QVariant OTKL_TSIKL_OPROS_PRIBORA(QVariantList& _values);
	QVariant KS_SPC_START_HZ(QVariantList& _values);
	QVariant KS_SPC_STOP_HZ(QVariantList& _values);
	QVariant KS_SPC_AVG(QVariantList& _values);
	QVariant KS_SPC_SET_ATN(QVariantList& _values);
	QVariant ZAGRUZIT_KOMANDU(QVariantList& _values);
	QVariant KS_SPC_SET_MARK(QVariantList& _values);
	QVariant KS_SPC_SET_MARK_TO_MAX_VALUE(QVariantList& _values);
	QVariant KS_SPC_DETECTOR_RESET(QVariantList& _values);
	QVariant KS_SPC_SETUP(QVariantList& _values);
	QVariant KS_SPC_AFAR(QVariantList& _values);
	QVariant KS_SPC_GET_PWR(QVariantList& _values);
	QVariant KS_SPC_GET_MARK_VALUE(QVariantList& _values);
	QVariant KS_SPC_GET_DETECTOR_PEAK_TRACE(QVariantList& _values);
	QVariant KS_SPC_GET_MARK_POS(QVariantList& _values);
	QVariant KS_GEN_SET(QVariantList& _values);
	QVariant KS_GEN_SET_FREQUENCY(QVariantList& _values);
	QVariant KS_GEN_SET_POWER(QVariantList& _values);
	QVariant KS_GEN_TURN_ON(QVariantList& _values);
	QVariant KS_VEC_AFAR(QVariantList& _values);
	QVariant KS_VEC_AFAR_NO_AMP(QVariantList& _values);
	QVariant KS_VEC_TURN_ON(QVariantList& _values);
	QVariant KS_VEC_SET_PWR(QVariantList& _values);
	QVariant KS_VEC_SET_FREQ(QVariantList& _values);
	QVariant KS_VEC_SET_PHASE_OFFSET(QVariantList& _values);
	QVariant KS_VEC_SET_MEAS_FORMAT(QVariantList& _values);
	QVariant KS_VEC_SET_MATH_FUNC(QVariantList& _values);
	QVariant KS_VEC_MEMORIZE(QVariantList& _values);
	QVariant KS_VEC_RETRIEVE(QVariantList& _values);
	QVariant KS_VEC_MEM_TRACE(QVariantList& _values);
	QVariant KS_VEC_GET_TRACE(QVariantList& _values);
	QVariant KS_VEC_REFRESH_ERROR_LIST(QVariantList& _values);
	QVariant ZAPISAT_ZNACH_SIGNALA_BD(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	AfarSrvAppl* app;
	bool with_return;
	static int obj_num;
};

class afar_server_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	afar_server_Socket_RPC_SLOT_Thread(AfarSrvAppl* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<afar_server_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<afar_server_Socket_RPC_SLOT_Object> rpc_obj;
	AfarSrvAppl* app;
	int socketDescriptor;
	static int obj_num;
};

class afar_server_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	afar_server_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, AfarSrvAppl* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	AfarSrvAppl* app;
	QList<std::shared_ptr<afar_server_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class afar_server_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	afar_server_Socket_RPC_SLOT_Server_Thread();
	void set_app(AfarSrvAppl* _app)
	{
		app = _app;
	}
	void set_params(QString _conn_ip, int _conn_port)
	{
		conn_ip = _conn_ip;
		conn_port = _conn_port;
	}
	void run();
private:
	afar_server_Socket_RPC_SLOT_Server* rpc_srv;
	AfarSrvAppl* app;
	QString conn_ip;
	int conn_port;

};
#endif
