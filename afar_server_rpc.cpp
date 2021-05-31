#include "afar_server_rpc.h"

void RPC_afar_server_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_afar_server_SLOT_Object>(new RPC_afar_server_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_afar_server_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_afar_server_SIGNAL_Object>(new RPC_afar_server_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_afar_server_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_afar_server_SIGNAL_Object::toLog)) {
		connect_signal("toLog(QString)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_afar_server_SIGNAL_Object::toProtocol)) {
		connect_signal("toProtocol(QString)", true);
	}
}

void RPC_afar_server_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_afar_server_SIGNAL_Object::toLog)) {
		connect_signal("toLog(QString)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_afar_server_SIGNAL_Object::toProtocol)) {
		connect_signal("toProtocol(QString)", false);
	}
}

void RPC_afar_server_SIGNAL_Object::read_data()
{
	int tmp_size;
	while (_sock->bytesAvailable())
	{
		int tmp_bytes = _sock->bytesAvailable();
		while (tmp_bytes < 4)
		{
			_sock->waitForReadyRead(100);
			tmp_bytes = _sock->bytesAvailable();
		}
		QByteArray tmp_size_arr = _sock->read(4);
		QDataStream tmp_size_stream(&tmp_size_arr, QIODevice::ReadOnly);
		tmp_size_stream >> tmp_size;
		while (1)
		{
			tmp_bytes = _sock->bytesAvailable();
			if (tmp_bytes >= tmp_size)
				break;
			_sock->waitForReadyRead(500);
		}
		SRPCSignalClass::Instance().toLog(QString("%1 signal new data %2 bytes").arg(objectName()).arg(tmp_size));
		QByteArray tmp_arr = _sock->read(tmp_size);
		QDataStream tmp_stream(&tmp_arr, QIODevice::ReadOnly);
		while(!tmp_stream.atEnd())
		{
			QString op_name;
			tmp_stream >> op_name;
			int call_number;
			tmp_stream >> call_number;

			SRPCSignalClass::Instance().toLog("afar_server new signal " + op_name);

			if (op_name == "toLog(QString)")
			{
				QString message;
				tmp_stream >> message;
				SRPCSignalClass::Instance().toLog("afar_server " + op_name +" call_number "+ QString::number(call_number) + " message = "+RPCSignalClass::QVariantToString(message));
				emit toLog(message);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("afar_server signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "toProtocol(QString)")
			{
				QString message;
				tmp_stream >> message;
				SRPCSignalClass::Instance().toLog("afar_server " + op_name +" call_number "+ QString::number(call_number) + " message = "+RPCSignalClass::QVariantToString(message));
				emit toProtocol(message);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("afar_server signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

QString RPC_afar_server_SLOT_Object::getXML()
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call getXML %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("getXML()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished getXML %1").arg(tmp_ret_params));
	return res.toString();
}
void RPC_afar_server_SLOT_Object::OPROS_IP(QString IMYA_PRIBORA)
{
	QVariantList tmp_list;
	tmp_list << QVariant(IMYA_PRIBORA);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call OPROS_IP %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("OPROS_IP(QString)", tmp_list);
	SRPCSignalClass::Instance().toLog("afar_server dynamic_call finished OPROS_IP");
}
int RPC_afar_server_SLOT_Object::DOBAVIT_PRIBOR_AFAR(QString IMYA_PRIBORA, QString SETEVOJ_ADRES, int PORT)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(IMYA_PRIBORA);
	tmp_list << QVariant(SETEVOJ_ADRES);
	tmp_list << QVariant(PORT);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call DOBAVIT_PRIBOR_AFAR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("DOBAVIT_PRIBOR_AFAR(QString, QString, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished DOBAVIT_PRIBOR_AFAR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::UDALIT_PRIBOR_AFAR(QString IMYA_PRIBORA)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(IMYA_PRIBORA);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call UDALIT_PRIBOR_AFAR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("UDALIT_PRIBOR_AFAR(QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished UDALIT_PRIBOR_AFAR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::PODKLYUCHIT_PRIBOR_AFAR(QString IMYA_PRIBORA)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(IMYA_PRIBORA);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call PODKLYUCHIT_PRIBOR_AFAR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("PODKLYUCHIT_PRIBOR_AFAR(QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished PODKLYUCHIT_PRIBOR_AFAR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::OTKLYUCHIT_PRIBOR_AFAR(QString IMYA_PRIBORA)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(IMYA_PRIBORA);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call OTKLYUCHIT_PRIBOR_AFAR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("OTKLYUCHIT_PRIBOR_AFAR(QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished OTKLYUCHIT_PRIBOR_AFAR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::ZAGRUZIT_KOMANDU_P_AFAR(QString IMYA_PRIBORA, QString KOMANDA, int VREMYA_OZHIDANIYA)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(IMYA_PRIBORA);
	tmp_list << QVariant(KOMANDA);
	tmp_list << QVariant(VREMYA_OZHIDANIYA);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call ZAGRUZIT_KOMANDU_P_AFAR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ZAGRUZIT_KOMANDU_P_AFAR(QString, QString, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished ZAGRUZIT_KOMANDU_P_AFAR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::POLUCHIT_SOOBSSHENIE_P_AFAR(QString IMYA_PRIBORA, QVariant& IMYA_PEREMENNOJ)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(IMYA_PRIBORA);
	tmp_list << QVariant(IMYA_PEREMENNOJ);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call POLUCHIT_SOOBSSHENIE_P_AFAR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("POLUCHIT_SOOBSSHENIE_P_AFAR(QString, QVariant&)", tmp_list);
	IMYA_PEREMENNOJ = tmp_list.at(1);
	tmp_ret_params += " IMYA_PEREMENNOJ="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished POLUCHIT_SOOBSSHENIE_P_AFAR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::UPRAVLENIE_SPDT_KOMMUTATORAMI(QVariantList mass)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(mass);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call UPRAVLENIE_SPDT_KOMMUTATORAMI %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("UPRAVLENIE_SPDT_KOMMUTATORAMI(QVariantList)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished UPRAVLENIE_SPDT_KOMMUTATORAMI %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::VKL_TSIKL_OPROS_PRIBORA(QString IMYA_PRIBORA, QString PARAM_PRIBORA, double PERIOD, QString ED_IZM)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(IMYA_PRIBORA);
	tmp_list << QVariant(PARAM_PRIBORA);
	tmp_list << QVariant(PERIOD);
	tmp_list << QVariant(ED_IZM);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call VKL_TSIKL_OPROS_PRIBORA %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("VKL_TSIKL_OPROS_PRIBORA(QString, QString, double, QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished VKL_TSIKL_OPROS_PRIBORA %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::OTKL_TSIKL_OPROS_PRIBORA(QString IMYA_PRIBORA, QString PARAM_PRIBORA)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(IMYA_PRIBORA);
	tmp_list << QVariant(PARAM_PRIBORA);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call OTKL_TSIKL_OPROS_PRIBORA %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("OTKL_TSIKL_OPROS_PRIBORA(QString, QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished OTKL_TSIKL_OPROS_PRIBORA %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_START_HZ(double hz)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(hz);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_START_HZ %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_START_HZ(double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_START_HZ %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_STOP_HZ(double hz)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(hz);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_STOP_HZ %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_STOP_HZ(double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_STOP_HZ %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_AVG(int _avg)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_avg);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_AVG %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_AVG(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_AVG %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_SET_ATN(double _val)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_val);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_SET_ATN %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_SET_ATN(double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_SET_ATN %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::ZAGRUZIT_KOMANDU(QString ustr, QString KOMANDA)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(ustr);
	tmp_list << QVariant(KOMANDA);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call ZAGRUZIT_KOMANDU %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ZAGRUZIT_KOMANDU(QString, QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished ZAGRUZIT_KOMANDU %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_SET_MARK(double _hz, int _mark_id)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_hz);
	tmp_list << QVariant(_mark_id);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_SET_MARK %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_SET_MARK(double, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_SET_MARK %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_SET_MARK_TO_MAX_VALUE(int _mark_id)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_mark_id);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_SET_MARK_TO_MAX_VALUE %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_SET_MARK_TO_MAX_VALUE(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_SET_MARK_TO_MAX_VALUE %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_DETECTOR_RESET()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_DETECTOR_RESET %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_DETECTOR_RESET()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_DETECTOR_RESET %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_SETUP(double _start_hz, double _stop_hz, int _avg)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_start_hz);
	tmp_list << QVariant(_stop_hz);
	tmp_list << QVariant(_avg);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_SETUP %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_SETUP(double, double, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_SETUP %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_AFAR(int _mode)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_mode);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_AFAR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_AFAR(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_AFAR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_GET_PWR(double _hz, double& _pwr)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_hz);
	tmp_list << QVariant(_pwr);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_GET_PWR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_GET_PWR(double, double&)", tmp_list);
	_pwr = tmp_list.at(1).toDouble();
	tmp_ret_params += " _pwr="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_GET_PWR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_GET_MARK_VALUE(int _mark_id, double& _pwr)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_mark_id);
	tmp_list << QVariant(_pwr);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_GET_MARK_VALUE %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_GET_MARK_VALUE(int, double&)", tmp_list);
	_pwr = tmp_list.at(1).toDouble();
	tmp_ret_params += " _pwr="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_GET_MARK_VALUE %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_GET_DETECTOR_PEAK_TRACE(QVariantList& _out_result, double _start_freq, double _stop_freq, int _points, QString _detector)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_out_result);
	tmp_list << QVariant(_start_freq);
	tmp_list << QVariant(_stop_freq);
	tmp_list << QVariant(_points);
	tmp_list << QVariant(_detector);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_GET_DETECTOR_PEAK_TRACE %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_GET_DETECTOR_PEAK_TRACE(QVariantList&, double, double, int, QString)", tmp_list);
	_out_result = tmp_list.at(0).toList();
	tmp_ret_params += " _out_result="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_GET_DETECTOR_PEAK_TRACE %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_SPC_GET_MARK_POS(int _mark_id, double& _out_hz)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_mark_id);
	tmp_list << QVariant(_out_hz);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_SPC_GET_MARK_POS %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_SPC_GET_MARK_POS(int, double&)", tmp_list);
	_out_hz = tmp_list.at(1).toDouble();
	tmp_ret_params += " _out_hz="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_SPC_GET_MARK_POS %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_GEN_SET(double _hz, double _dBm)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_hz);
	tmp_list << QVariant(_dBm);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_GEN_SET %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_GEN_SET(double, double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_GEN_SET %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_GEN_SET_FREQUENCY(double _hz)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_hz);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_GEN_SET_FREQUENCY %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_GEN_SET_FREQUENCY(double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_GEN_SET_FREQUENCY %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_GEN_SET_POWER(double _dBm)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_dBm);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_GEN_SET_POWER %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_GEN_SET_POWER(double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_GEN_SET_POWER %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_GEN_TURN_ON(int _onoff)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_onoff);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_GEN_TURN_ON %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_GEN_TURN_ON(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_GEN_TURN_ON %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_AFAR(int _mode)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_mode);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_AFAR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_AFAR(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_AFAR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_AFAR_NO_AMP(int _mode)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_mode);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_AFAR_NO_AMP %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_AFAR_NO_AMP(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_AFAR_NO_AMP %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_TURN_ON(int _onoff)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_onoff);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_TURN_ON %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_TURN_ON(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_TURN_ON %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_SET_PWR(double _dbm)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_dbm);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_SET_PWR %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_SET_PWR(double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_SET_PWR %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_SET_FREQ(QString _meas, double _hz_beg, double _hz_end, int _points)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_meas);
	tmp_list << QVariant(_hz_beg);
	tmp_list << QVariant(_hz_end);
	tmp_list << QVariant(_points);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_SET_FREQ %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_SET_FREQ(QString, double, double, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_SET_FREQ %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_SET_PHASE_OFFSET(QString _meas, double _deg)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_meas);
	tmp_list << QVariant(_deg);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_SET_PHASE_OFFSET %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_SET_PHASE_OFFSET(QString, double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_SET_PHASE_OFFSET %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_SET_MEAS_FORMAT(QString _meas, QString _format)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_meas);
	tmp_list << QVariant(_format);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_SET_MEAS_FORMAT %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_SET_MEAS_FORMAT(QString, QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_SET_MEAS_FORMAT %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_SET_MATH_FUNC(QString _meas, QString _func)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_meas);
	tmp_list << QVariant(_func);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_SET_MATH_FUNC %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_SET_MATH_FUNC(QString, QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_SET_MATH_FUNC %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_MEMORIZE(QString _meas)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_meas);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_MEMORIZE %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_MEMORIZE(QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_MEMORIZE %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_RETRIEVE(QString _meas, QVariantList& _trace)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_meas);
	tmp_list << QVariant(_trace);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_RETRIEVE %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_RETRIEVE(QString, QVariantList&)", tmp_list);
	_trace = tmp_list.at(1).toList();
	tmp_ret_params += " _trace="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_RETRIEVE %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_MEM_TRACE(QString _meas, QString _format, QString _math)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_meas);
	tmp_list << QVariant(_format);
	tmp_list << QVariant(_math);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_MEM_TRACE %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_MEM_TRACE(QString, QString, QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_MEM_TRACE %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_GET_TRACE(QVariantList& _trace, QString _meas, QString _format, QString _math)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_trace);
	tmp_list << QVariant(_meas);
	tmp_list << QVariant(_format);
	tmp_list << QVariant(_math);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_GET_TRACE %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_GET_TRACE(QVariantList&, QString, QString, QString)", tmp_list);
	_trace = tmp_list.at(0).toList();
	tmp_ret_params += " _trace="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_GET_TRACE %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::KS_VEC_REFRESH_ERROR_LIST()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call KS_VEC_REFRESH_ERROR_LIST %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KS_VEC_REFRESH_ERROR_LIST()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished KS_VEC_REFRESH_ERROR_LIST %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_afar_server_SLOT_Object::ZAPISAT_ZNACH_SIGNALA_BD(int code, QVariant& val)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(code);
	tmp_list << QVariant(val);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call ZAPISAT_ZNACH_SIGNALA_BD %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ZAPISAT_ZNACH_SIGNALA_BD(int, QVariant&)", tmp_list);
	val = tmp_list.at(1);
	tmp_ret_params += " val="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("afar_server dynamic_call finished ZAPISAT_ZNACH_SIGNALA_BD %1").arg(tmp_ret_params));
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

