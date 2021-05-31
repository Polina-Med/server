#pragma once

#pragma warning (push, 0) 

#include <winsock2.h>

#include<qthread.h>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QVariantList>
#include <QFile>
#include <QTextStream>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>


#pragma warning (pop)

#include "PXIModule.h"


class AGSWITCH_Err : public std::exception
{
public:
	/**
	*	\brief Конструктор класса MFSK_Err
	*	\param msg - текст сообщения
	*	\param err_code - код ошибки
	*/
	AGSWITCH_Err(const char* msg, int err_code) : std::exception(msg), code(err_code) {}
	///Код ошибки
	int code;
};

/**
*	\class HWMezonine
*	\brief Класс для мезонинов
*/
class MWSwitchDevice : public HWPXIDevice
{
public:
	/**
	*	\brief Конструктор HWMezonine
	*	\param _addr - логический адрес устройства на шине VXI
	*/
	MWSwitchDevice(const char* _addr) : HWPXIDevice(_addr) { init(); }
	void connect_channel(QString first_chan, QString second_chan);
	void close_channel(QString chan_name);
	virtual void init();
	virtual void deinit();

private:
	void format_message(const char* msg, int err_code);
	friend class SGModule;

};


class MWSwitchModule : public PXIDereferencer<MWSwitchDevice>
{
public:
	static MWSwitchModule& Instance()
	{
		static MWSwitchModule inst;
		return inst;
	}
	MWSwitchModule(MWSwitchModule const&) = delete;
	MWSwitchModule& operator= (MWSwitchModule const&) = delete;
	/**
	*	\brief Возвращает текстовое имя модуля для его идентификации
	*	\return Имя модуля
	*/
	static QString getNamePrefix()
	{
		return "MWSwitch";
	}
	MWSwitchModule();
	void renewDevices();

};

