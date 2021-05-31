#ifndef AFAR_SERVER_RPC_H
#define AFAR_SERVER_RPC_H
#include "socket_rpc.h"


class RPC_afar_server_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_afar_server_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("afar_server_slot_obj");}
	////////////////////////////////////
public slots:
	QString getXML();
	void OPROS_IP(QString IMYA_PRIBORA);
	int DOBAVIT_PRIBOR_AFAR(QString IMYA_PRIBORA, QString SETEVOJ_ADRES, int PORT);
	int UDALIT_PRIBOR_AFAR(QString IMYA_PRIBORA);
	int PODKLYUCHIT_PRIBOR_AFAR(QString IMYA_PRIBORA);
	int OTKLYUCHIT_PRIBOR_AFAR(QString IMYA_PRIBORA);
	int ZAGRUZIT_KOMANDU_P_AFAR(QString IMYA_PRIBORA, QString KOMANDA, int VREMYA_OZHIDANIYA);
	int POLUCHIT_SOOBSSHENIE_P_AFAR(QString IMYA_PRIBORA, QVariant& IMYA_PEREMENNOJ);
	int UPRAVLENIE_SPDT_KOMMUTATORAMI(QVariantList mass);
	int VKL_TSIKL_OPROS_PRIBORA(QString IMYA_PRIBORA, QString PARAM_PRIBORA, double PERIOD, QString ED_IZM);
	int OTKL_TSIKL_OPROS_PRIBORA(QString IMYA_PRIBORA, QString PARAM_PRIBORA);
	int KS_SPC_START_HZ(double hz);
	int KS_SPC_STOP_HZ(double hz);
	int KS_SPC_AVG(int _avg);
	int KS_SPC_SET_ATN(double _val);
	int ZAGRUZIT_KOMANDU(QString ustr, QString KOMANDA);
	int KS_SPC_SET_MARK(double _hz, int _mark_id);
	int KS_SPC_SET_MARK_TO_MAX_VALUE(int _mark_id);
	int KS_SPC_DETECTOR_RESET();
	int KS_SPC_SETUP(double _start_hz, double _stop_hz, int _avg);
	int KS_SPC_AFAR(int _mode);
	int KS_SPC_GET_PWR(double _hz, double& _pwr);
	int KS_SPC_GET_MARK_VALUE(int _mark_id, double& _pwr);
	int KS_SPC_GET_DETECTOR_PEAK_TRACE(QVariantList& _out_result, double _start_freq, double _stop_freq, int _points, QString _detector);
	int KS_SPC_GET_MARK_POS(int _mark_id, double& _out_hz);
	int KS_GEN_SET(double _hz, double _dBm);
	int KS_GEN_SET_FREQUENCY(double _hz);
	int KS_GEN_SET_POWER(double _dBm);
	int KS_GEN_TURN_ON(int _onoff);
	int KS_VEC_AFAR(int _mode);
	int KS_VEC_AFAR_NO_AMP(int _mode);
	int KS_VEC_TURN_ON(int _onoff);
	int KS_VEC_SET_PWR(double _dbm);
	int KS_VEC_SET_FREQ(QString _meas, double _hz_beg, double _hz_end, int _points);
	int KS_VEC_SET_PHASE_OFFSET(QString _meas, double _deg);
	int KS_VEC_SET_MEAS_FORMAT(QString _meas, QString _format);
	int KS_VEC_SET_MATH_FUNC(QString _meas, QString _func);
	int KS_VEC_MEMORIZE(QString _meas);
	int KS_VEC_RETRIEVE(QString _meas, QVariantList& _trace);
	int KS_VEC_MEM_TRACE(QString _meas, QString _format, QString _math);
	int KS_VEC_GET_TRACE(QVariantList& _trace, QString _meas, QString _format, QString _math);
	int KS_VEC_REFRESH_ERROR_LIST();
	int ZAPISAT_ZNACH_SIGNALA_BD(int code, QVariant& val);
	////////////////////////////////////
};

class RPC_afar_server_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_afar_server_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("afar_server_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_afar_server_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_afar_server_SLOT_Object> get_afar_server_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_afar_server_SLOT_Object> rpc_obj;

};

class RPC_afar_server_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_afar_server_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("afar_server_signal_obj");
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void toLog(QString message);
	void toProtocol(QString message);
};

class RPC_afar_server_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_afar_server_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("afar_server_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_afar_server_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_afar_server_SIGNAL_Object> rpc_obj;
};

#endif
