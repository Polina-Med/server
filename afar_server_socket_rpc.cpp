	#include "afar_server_socket_rpc.h"

int afar_server_Socket_RPC_SLOT_Object::obj_num = 0;
int afar_server_Socket_RPC_SIGNAL_Object::obj_num = 0;
int afar_server_Socket_RPC_SIGNAL_Object::call_number = 0;
int afar_server_Socket_RPC_SLOT_Thread::obj_num = 0;

	afar_server_Socket_RPC_SIGNAL_Thread::afar_server_Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("afar_server_Socket_RPC_SIGNAL_Thread");
	}

	void afar_server_Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new afar_server_Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("afar_server signal thread started");
		exec();
	}

	afar_server_Socket_RPC_SIGNAL_Server::afar_server_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void afar_server_Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("afar_server signal client connected");
		std::shared_ptr<afar_server_Socket_RPC_SIGNAL_Object> tmp_obj(new afar_server_Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	afar_server_Socket_RPC_SLOT_Server_Thread::afar_server_Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	afar_server_Socket_RPC_SLOT_Server::afar_server_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, AfarSrvAppl* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void afar_server_Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new afar_server_Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("afar_server slot thread started");
		exec();
	}

	void afar_server_Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<afar_server_Socket_RPC_SLOT_Object>(new afar_server_Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	afar_server_Socket_RPC_SLOT_Thread::afar_server_Socket_RPC_SLOT_Thread(AfarSrvAppl* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{
	setObjectName(QString("afar_server_Socket_RPC_SLOT_Thread_%1").arg(obj_num++));
	}

	void afar_server_Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("afar_server slot client connected");
		std::shared_ptr<afar_server_Socket_RPC_SLOT_Thread> tmp_obj(new afar_server_Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	afar_server_Socket_RPC_SLOT_Object::afar_server_Socket_RPC_SLOT_Object(AfarSrvAppl* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("afar_server_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &afar_server_Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["getXML()"] = &afar_server_Socket_RPC_SLOT_Object::getXML;
		operators_map["OPROS_IP(QString)"] = &afar_server_Socket_RPC_SLOT_Object::OPROS_IP;
		operators_map["DOBAVIT_PRIBOR_AFAR(QString, QString, int)"] = &afar_server_Socket_RPC_SLOT_Object::DOBAVIT_PRIBOR_AFAR;
		operators_map["UDALIT_PRIBOR_AFAR(QString)"] = &afar_server_Socket_RPC_SLOT_Object::UDALIT_PRIBOR_AFAR;
		operators_map["PODKLYUCHIT_PRIBOR_AFAR(QString)"] = &afar_server_Socket_RPC_SLOT_Object::PODKLYUCHIT_PRIBOR_AFAR;
		operators_map["OTKLYUCHIT_PRIBOR_AFAR(QString)"] = &afar_server_Socket_RPC_SLOT_Object::OTKLYUCHIT_PRIBOR_AFAR;
		operators_map["ZAGRUZIT_KOMANDU_P_AFAR(QString, QString, int)"] = &afar_server_Socket_RPC_SLOT_Object::ZAGRUZIT_KOMANDU_P_AFAR;
		operators_map["POLUCHIT_SOOBSSHENIE_P_AFAR(QString, QVariant&)"] = &afar_server_Socket_RPC_SLOT_Object::POLUCHIT_SOOBSSHENIE_P_AFAR;
		operators_map["UPRAVLENIE_SPDT_KOMMUTATORAMI(QVariantList)"] = &afar_server_Socket_RPC_SLOT_Object::UPRAVLENIE_SPDT_KOMMUTATORAMI;
		operators_map["VKL_TSIKL_OPROS_PRIBORA(QString, QString, double, QString)"] = &afar_server_Socket_RPC_SLOT_Object::VKL_TSIKL_OPROS_PRIBORA;
		operators_map["OTKL_TSIKL_OPROS_PRIBORA(QString, QString)"] = &afar_server_Socket_RPC_SLOT_Object::OTKL_TSIKL_OPROS_PRIBORA;
		operators_map["KS_SPC_START_HZ(double)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_START_HZ;
		operators_map["KS_SPC_STOP_HZ(double)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_STOP_HZ;
		operators_map["KS_SPC_AVG(int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_AVG;
		operators_map["KS_SPC_SET_ATN(double)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_SET_ATN;
		operators_map["ZAGRUZIT_KOMANDU(QString, QString)"] = &afar_server_Socket_RPC_SLOT_Object::ZAGRUZIT_KOMANDU;
		operators_map["KS_SPC_SET_MARK(double, int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_SET_MARK;
		operators_map["KS_SPC_SET_MARK_TO_MAX_VALUE(int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_SET_MARK_TO_MAX_VALUE;
		operators_map["KS_SPC_DETECTOR_RESET()"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_DETECTOR_RESET;
		operators_map["KS_SPC_SETUP(double, double, int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_SETUP;
		operators_map["KS_SPC_AFAR(int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_AFAR;
		operators_map["KS_SPC_GET_PWR(double, double&)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_GET_PWR;
		operators_map["KS_SPC_GET_MARK_VALUE(int, double&)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_GET_MARK_VALUE;
		operators_map["KS_SPC_GET_DETECTOR_PEAK_TRACE(QVariantList&, double, double, int, QString)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_GET_DETECTOR_PEAK_TRACE;
		operators_map["KS_SPC_GET_MARK_POS(int, double&)"] = &afar_server_Socket_RPC_SLOT_Object::KS_SPC_GET_MARK_POS;
		operators_map["KS_GEN_SET(double, double)"] = &afar_server_Socket_RPC_SLOT_Object::KS_GEN_SET;
		operators_map["KS_GEN_SET_FREQUENCY(double)"] = &afar_server_Socket_RPC_SLOT_Object::KS_GEN_SET_FREQUENCY;
		operators_map["KS_GEN_SET_POWER(double)"] = &afar_server_Socket_RPC_SLOT_Object::KS_GEN_SET_POWER;
		operators_map["KS_GEN_TURN_ON(int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_GEN_TURN_ON;
		operators_map["KS_VEC_AFAR(int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_AFAR;
		operators_map["KS_VEC_AFAR_NO_AMP(int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_AFAR_NO_AMP;
		operators_map["KS_VEC_TURN_ON(int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_TURN_ON;
		operators_map["KS_VEC_SET_PWR(double)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_PWR;
		operators_map["KS_VEC_SET_FREQ(QString, double, double, int)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_FREQ;
		operators_map["KS_VEC_SET_PHASE_OFFSET(QString, double)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_PHASE_OFFSET;
		operators_map["KS_VEC_SET_MEAS_FORMAT(QString, QString)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_MEAS_FORMAT;
		operators_map["KS_VEC_SET_MATH_FUNC(QString, QString)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_MATH_FUNC;
		operators_map["KS_VEC_MEMORIZE(QString)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_MEMORIZE;
		operators_map["KS_VEC_RETRIEVE(QString, QVariantList&)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_RETRIEVE;
		operators_map["KS_VEC_MEM_TRACE(QString, QString, QString)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_MEM_TRACE;
		operators_map["KS_VEC_GET_TRACE(QVariantList&, QString, QString, QString)"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_GET_TRACE;
		operators_map["KS_VEC_REFRESH_ERROR_LIST()"] = &afar_server_Socket_RPC_SLOT_Object::KS_VEC_REFRESH_ERROR_LIST;
		operators_map["ZAPISAT_ZNACH_SIGNALA_BD(int, QVariant&)"] = &afar_server_Socket_RPC_SLOT_Object::ZAPISAT_ZNACH_SIGNALA_BD;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	afar_server_Socket_RPC_SIGNAL_Object::afar_server_Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("afar_server_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void afar_server_Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void afar_server_Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void afar_server_Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void afar_server_Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
		disconnect(app, SIGNAL(toLog(QString)), this, SLOT(toLog(QString)));
		disconnect(app, SIGNAL(toProtocol(QString)), this, SLOT(toProtocol(QString)));
	}
	void afar_server_Socket_RPC_SIGNAL_Object::set_app(AfarSrvAppl* _app)
	{
		app = _app;
		connect(app, SIGNAL(toLog(QString)), this, SLOT(toLog(QString)), Qt::DirectConnection);
		data_map.insert("toLog(QString)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(toProtocol(QString)), this, SLOT(toProtocol(QString)), Qt::DirectConnection);
		data_map.insert("toProtocol(QString)", std::shared_ptr<SignalData>(new SignalData()));

	}

	void afar_server_Socket_RPC_SLOT_Object::read_data()
	{
		//LARGE_INTEGER _freq;
		//LARGE_INTEGER tmp1;
		//LARGE_INTEGER tmp2;
		int tmp_size;
		int tmp_bytes = rpc_socket->bytesAvailable();
		if (tmp_bytes < 4)
			return;
		QByteArray tmp_size_arr = rpc_socket->read(4);
		QDataStream tmp_size_stream(&tmp_size_arr, QIODevice::ReadOnly);
		tmp_size_stream >> tmp_size;
		while (1)
		{
			tmp_bytes = rpc_socket->bytesAvailable();
			if (tmp_bytes >= tmp_size)
			break;
			rpc_socket->waitForReadyRead(500);
		}
		SRPCSignalClass::Instance().toLog(QString(" %1 new data %2 bytes").arg(objectName()).arg(tmp_size));
		QByteArray tmp_arr = rpc_socket->read(tmp_size);
		QDataStream tmp_stream(&tmp_arr, QIODevice::ReadOnly);
		int call_n;
		tmp_stream >> call_n;
		QString op_name;
		tmp_stream >> op_name;
		SRPCSignalClass::Instance().toLog(QString("%1 op_name %2 call_n %3").arg(objectName()).arg(op_name).arg(call_n));
		OPERATORS_MAP::iterator op_itr = operators_map.find(op_name);
		QVariant res = 1;
		with_return = false;
		QVariantList var_list;
		tmp_stream >> var_list;
		if (op_itr != operators_map.end())
			res = (this->*op_itr.value())(var_list);
		//QueryPerformanceCounter(&tmp2);
		//double tmp_time = (tmp2.QuadPart-tmp1.QuadPart)*1e6/_freq.QuadPart;
		//qDebug() << tmp_time;
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		if (!with_return)
		{
			tmp_stream2 << QString("response");
			tmp_stream2 << res;
		}
		else
		{
			tmp_stream2 << QString("response_return");
			tmp_stream2 << res;
			tmp_stream2 << var_list;
		}
		QByteArray tmp_arr3;
		QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
		tmp_stream3 << tmp_arr2.size();
		rpc_socket->write(tmp_arr3+tmp_arr2);
		for (int i = 0; i<3; ++i)
		{
			if (rpc_socket->waitForBytesWritten(3000))
				break; 
			else
				SRPCSignalClass::Instance().toLog(QString(" == == == == == == == == == == == == == = %1 response wait failed %2").arg(objectName()).arg(rpc_socket->errorString()));
		}
		SRPCSignalClass::Instance().toLog(QString(" %1 %2 call_n %3 response sent %4 %5 %6").arg(objectName()).arg(op_name).arg(call_n).arg(tmp_arr2.size()).arg(tmp_arr3.size()).arg(tmp_arr3.data())); 
	}

	void afar_server_Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void afar_server_Socket_RPC_SIGNAL_Object::read_data()
	{
		int tmp_size;
		while (rpc_socket->bytesAvailable())
		{
			int tmp_bytes = rpc_socket->bytesAvailable();
			while (tmp_bytes < 4)
			{
				rpc_socket->waitForReadyRead(100);
				tmp_bytes = rpc_socket->bytesAvailable();
			}
			QByteArray tmp_size_arr = rpc_socket->read(4);
			QDataStream tmp_size_stream(&tmp_size_arr, QIODevice::ReadOnly);
			tmp_size_stream >> tmp_size;
			while (1)
			{
				tmp_bytes = rpc_socket->bytesAvailable();
				if (tmp_bytes >= tmp_size)
					break;
				rpc_socket->waitForReadyRead(100);
			}
			QByteArray tmp_arr = rpc_socket->read(tmp_size);
			QDataStream tmp_stream(&tmp_arr, QIODevice::ReadOnly);
			QVariantList signal_data;
			QString signal_name;
			tmp_stream >> signal_name;
			if (signal_name == "connect")
			{
				tmp_stream >> signal_name;
				if (!data_map.contains(signal_name))
					return;
				SRPCSignalClass::Instance().toLog(QString("%1 connect signal %2").arg(objectName()).arg(signal_name));
				data_map[signal_name]->signal_needed = true;
			}
			else
			if (signal_name == "disconnect")
			{
				tmp_stream >> signal_name;
				if (!data_map.contains(signal_name))
					return;
				SRPCSignalClass::Instance().toLog(QString("%1 disconnect signal %2").arg(objectName()).arg(signal_name));
				data_map[signal_name]->signal_needed = false;
			}
			else
			{
				if (!data_map.contains(signal_name))
					return;
				auto& descriptor = *data_map[signal_name].get();
				SRPCSignalClass::Instance().toLog(QString("%1 signal %2 received data").arg(objectName()).arg(signal_name));
				if (signal_name.toLocal8Bit().size() != tmp_size)
					tmp_stream >> signal_data;
				descriptor.signal_data = signal_data;
//				data_map[signal_name]->cond.wakeAll();
				descriptor.mutex.unlock();
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	void afar_server_Socket_RPC_SIGNAL_Object::toLog(QString message)
	{
		auto& descriptor = *data_map["toLog(QString)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("toLog(QString)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal toLog  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << message;
		SRPCSignalClass::Instance().toLog(QString("toLog  call_number %2 message =  %1").arg(RPCSignalClass::QVariantToString(message)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal toLog sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal toLog finished").arg(objectName()));
	}
	void afar_server_Socket_RPC_SIGNAL_Object::toProtocol(QString message)
	{
		auto& descriptor = *data_map["toProtocol(QString)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("toProtocol(QString)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal toProtocol  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << message;
		SRPCSignalClass::Instance().toLog(QString("toProtocol  call_number %2 message =  %1").arg(RPCSignalClass::QVariantToString(message)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal toProtocol sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal toProtocol finished").arg(objectName()));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVariant afar_server_Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant afar_server_Socket_RPC_SLOT_Object::getXML(QVariantList& _values)
	{
		try
		{
			QString res = app->getXML();
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return QString();
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return QString();
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::OPROS_IP(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			app->OPROS_IP(IMYA_PRIBORA);
			return 0;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 0;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 0;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::DOBAVIT_PRIBOR_AFAR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			QString SETEVOJ_ADRES = _values.at(1).value<QString>();
			int PORT = _values.at(2).value<int>();
			int res = app->DOBAVIT_PRIBOR_AFAR(IMYA_PRIBORA, SETEVOJ_ADRES, PORT);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::UDALIT_PRIBOR_AFAR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			int res = app->UDALIT_PRIBOR_AFAR(IMYA_PRIBORA);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::PODKLYUCHIT_PRIBOR_AFAR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			int res = app->PODKLYUCHIT_PRIBOR_AFAR(IMYA_PRIBORA);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::OTKLYUCHIT_PRIBOR_AFAR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			int res = app->OTKLYUCHIT_PRIBOR_AFAR(IMYA_PRIBORA);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::ZAGRUZIT_KOMANDU_P_AFAR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			QString KOMANDA = _values.at(1).value<QString>();
			int VREMYA_OZHIDANIYA = _values.at(2).value<int>();
			int res = app->ZAGRUZIT_KOMANDU_P_AFAR(IMYA_PRIBORA, KOMANDA, VREMYA_OZHIDANIYA);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::POLUCHIT_SOOBSSHENIE_P_AFAR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			QVariant IMYA_PEREMENNOJ = _values.at(1).value<QVariant>();
			int res = app->POLUCHIT_SOOBSSHENIE_P_AFAR(IMYA_PRIBORA, IMYA_PEREMENNOJ);
			_values[1] = IMYA_PEREMENNOJ;
			SRPCSignalClass::Instance().toLog(QString("%1 IMYA_PEREMENNOJ = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::UPRAVLENIE_SPDT_KOMMUTATORAMI(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList mass = _values.at(0).value<QVariantList>();
			int res = app->UPRAVLENIE_SPDT_KOMMUTATORAMI(mass);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::VKL_TSIKL_OPROS_PRIBORA(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			QString PARAM_PRIBORA = _values.at(1).value<QString>();
			double PERIOD = _values.at(2).value<double>();
			QString ED_IZM = _values.at(3).value<QString>();
			int res = app->VKL_TSIKL_OPROS_PRIBORA(IMYA_PRIBORA, PARAM_PRIBORA, PERIOD, ED_IZM);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::OTKL_TSIKL_OPROS_PRIBORA(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString IMYA_PRIBORA = _values.at(0).value<QString>();
			QString PARAM_PRIBORA = _values.at(1).value<QString>();
			int res = app->OTKL_TSIKL_OPROS_PRIBORA(IMYA_PRIBORA, PARAM_PRIBORA);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_START_HZ(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double hz = _values.at(0).value<double>();
			int res = app->KS_SPC_START_HZ(hz);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_STOP_HZ(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double hz = _values.at(0).value<double>();
			int res = app->KS_SPC_STOP_HZ(hz);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_AVG(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _avg = _values.at(0).value<int>();
			int res = app->KS_SPC_AVG(_avg);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_SET_ATN(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double _val = _values.at(0).value<double>();
			int res = app->KS_SPC_SET_ATN(_val);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::ZAGRUZIT_KOMANDU(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString ustr = _values.at(0).value<QString>();
			QString KOMANDA = _values.at(1).value<QString>();
			int res = app->ZAGRUZIT_KOMANDU(ustr, KOMANDA);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_SET_MARK(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double _hz = _values.at(0).value<double>();
			int _mark_id = _values.at(1).value<int>();
			int res = app->KS_SPC_SET_MARK(_hz, _mark_id);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_SET_MARK_TO_MAX_VALUE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _mark_id = _values.at(0).value<int>();
			int res = app->KS_SPC_SET_MARK_TO_MAX_VALUE(_mark_id);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_DETECTOR_RESET(QVariantList& _values)
	{
		try
		{
			int res = app->KS_SPC_DETECTOR_RESET();
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_SETUP(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double _start_hz = _values.at(0).value<double>();
			double _stop_hz = _values.at(1).value<double>();
			int _avg = _values.at(2).value<int>();
			int res = app->KS_SPC_SETUP(_start_hz, _stop_hz, _avg);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_AFAR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _mode = _values.at(0).value<int>();
			int res = app->KS_SPC_AFAR(_mode);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_GET_PWR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double _hz = _values.at(0).value<double>();
			double _pwr = _values.at(1).value<double>();
			int res = app->KS_SPC_GET_PWR(_hz, _pwr);
			_values[1] = _pwr;
			SRPCSignalClass::Instance().toLog(QString("%1 _pwr = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_GET_MARK_VALUE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _mark_id = _values.at(0).value<int>();
			double _pwr = _values.at(1).value<double>();
			int res = app->KS_SPC_GET_MARK_VALUE(_mark_id, _pwr);
			_values[1] = _pwr;
			SRPCSignalClass::Instance().toLog(QString("%1 _pwr = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_GET_DETECTOR_PEAK_TRACE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList _out_result = _values.at(0).value<QVariantList>();
			double _start_freq = _values.at(1).value<double>();
			double _stop_freq = _values.at(2).value<double>();
			int _points = _values.at(3).value<int>();
			QString _detector = _values.at(4).value<QString>();
			int res = app->KS_SPC_GET_DETECTOR_PEAK_TRACE(_out_result, _start_freq, _stop_freq, _points, _detector);
			_values[0] = _out_result;
			SRPCSignalClass::Instance().toLog(QString("%1 _out_result = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_SPC_GET_MARK_POS(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _mark_id = _values.at(0).value<int>();
			double _out_hz = _values.at(1).value<double>();
			int res = app->KS_SPC_GET_MARK_POS(_mark_id, _out_hz);
			_values[1] = _out_hz;
			SRPCSignalClass::Instance().toLog(QString("%1 _out_hz = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_GEN_SET(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double _hz = _values.at(0).value<double>();
			double _dBm = _values.at(1).value<double>();
			int res = app->KS_GEN_SET(_hz, _dBm);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_GEN_SET_FREQUENCY(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double _hz = _values.at(0).value<double>();
			int res = app->KS_GEN_SET_FREQUENCY(_hz);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_GEN_SET_POWER(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double _dBm = _values.at(0).value<double>();
			int res = app->KS_GEN_SET_POWER(_dBm);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_GEN_TURN_ON(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _onoff = _values.at(0).value<int>();
			int res = app->KS_GEN_TURN_ON(_onoff);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_AFAR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _mode = _values.at(0).value<int>();
			int res = app->KS_VEC_AFAR(_mode);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_AFAR_NO_AMP(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _mode = _values.at(0).value<int>();
			int res = app->KS_VEC_AFAR_NO_AMP(_mode);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_TURN_ON(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _onoff = _values.at(0).value<int>();
			int res = app->KS_VEC_TURN_ON(_onoff);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_PWR(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double _dbm = _values.at(0).value<double>();
			int res = app->KS_VEC_SET_PWR(_dbm);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_FREQ(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _meas = _values.at(0).value<QString>();
			double _hz_beg = _values.at(1).value<double>();
			double _hz_end = _values.at(2).value<double>();
			int _points = _values.at(3).value<int>();
			int res = app->KS_VEC_SET_FREQ(_meas, _hz_beg, _hz_end, _points);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_PHASE_OFFSET(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _meas = _values.at(0).value<QString>();
			double _deg = _values.at(1).value<double>();
			int res = app->KS_VEC_SET_PHASE_OFFSET(_meas, _deg);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_MEAS_FORMAT(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _meas = _values.at(0).value<QString>();
			QString _format = _values.at(1).value<QString>();
			int res = app->KS_VEC_SET_MEAS_FORMAT(_meas, _format);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_SET_MATH_FUNC(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _meas = _values.at(0).value<QString>();
			QString _func = _values.at(1).value<QString>();
			int res = app->KS_VEC_SET_MATH_FUNC(_meas, _func);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_MEMORIZE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _meas = _values.at(0).value<QString>();
			int res = app->KS_VEC_MEMORIZE(_meas);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_RETRIEVE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _meas = _values.at(0).value<QString>();
			QVariantList _trace = _values.at(1).value<QVariantList>();
			int res = app->KS_VEC_RETRIEVE(_meas, _trace);
			_values[1] = _trace;
			SRPCSignalClass::Instance().toLog(QString("%1 _trace = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_MEM_TRACE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _meas = _values.at(0).value<QString>();
			QString _format = _values.at(1).value<QString>();
			QString _math = _values.at(2).value<QString>();
			int res = app->KS_VEC_MEM_TRACE(_meas, _format, _math);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_GET_TRACE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList _trace = _values.at(0).value<QVariantList>();
			QString _meas = _values.at(1).value<QString>();
			QString _format = _values.at(2).value<QString>();
			QString _math = _values.at(3).value<QString>();
			int res = app->KS_VEC_GET_TRACE(_trace, _meas, _format, _math);
			_values[0] = _trace;
			SRPCSignalClass::Instance().toLog(QString("%1 _trace = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::KS_VEC_REFRESH_ERROR_LIST(QVariantList& _values)
	{
		try
		{
			int res = app->KS_VEC_REFRESH_ERROR_LIST();
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
	QVariant afar_server_Socket_RPC_SLOT_Object::ZAPISAT_ZNACH_SIGNALA_BD(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int code = _values.at(0).value<int>();
			QVariant val = _values.at(1).value<QVariant>();
			int res = app->ZAPISAT_ZNACH_SIGNALA_BD(code, val);
			_values[1] = val;
			SRPCSignalClass::Instance().toLog(QString("%1 val = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &err)
		{
			app->toProtocol_send(err.what());
			return 1;
		}
		catch(...)
		{
			app->toProtocol_send("Неизвестное исключение");
			return 1;
		}
	}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

