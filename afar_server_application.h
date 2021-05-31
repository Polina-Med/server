#ifndef AFAR_SERVER_APPLICATION_H
#define AFAR_SERVER_APPLICATION_H

#include <QObject>
#include <QApplication>
#include "PXIModule.h"
#include <QString>

#include "TMDBController.hpp"

enum ERROR_CODES
{
	SUCCESS = 0,
	OTHER_ERROR,
	NOT_IMPLEMENTED,
	LOGIC_ERROR,
	ARGUMENT_ERROR,
	DRIVER_ERROR,
};

class AfarSrvAppl : public QObject
{
	Q_OBJECT

public:
	///Конструктор класса AfarSrvAppl
	AfarSrvAppl(QObject *parent = 0);
	///Деструктор класса AfarSrvAppl
	~AfarSrvAppl();
	void toProtocol_send(QString message)
	{
		emit toProtocol(message);
	}

	/**
	*	\brief Добавление модуля к списку подключенных модулей
	*
	*	Каждый подлючаемый модуль является синглтоном, поэтому в списке модулей хранится укзатель но глобальный объект
	*	\param str - Описатель модуля
	*/
	template <class T>
	T* addModule(bool _show = true)
	{
		try
		{
			T* ptr = &T::Instance();
			lst_modules[T::getNamePrefix()] = ptr;
			ptr->setTestShow(_show);
			return ptr;
		}
		catch (const std::exception& e)
		{
			//startupErrors.push_back(QString(e.what()));
		}

		return NULL;
	}
	/**
	*	\brief Получение указателя на модуль с именем obj
	*
	*	При отсутствии искомого модуля выдается соответствующее исключение
	*	\param obj имя модуля
	*/
	template <class T>
	T& getModuleReference() const
	{
		MODULE_LIST::const_iterator itr = lst_modules.find(T::getNamePrefix());
		if (itr == lst_modules.end())
			throw std::runtime_error(QString("Модуль %1 не присоединен").arg(T::getNamePrefix()).toStdString().c_str());
		if (itr.value()->lst_device.isEmpty())
			throw std::runtime_error(QString("Список устройств модуля %1 пуст").arg(T::getNamePrefix()).toStdString().c_str());
		return *static_cast<T*>(itr.value());
	}
	
private:
	QMap<QString, SCPIDevice*> pribor_map;
	//добавление нового инструмента в карту
	int addInstrument(QString name, QString ip, int port);
	//отключение и удаление инструмента из карты
	int removeInstrument(QString name);
	QMap<QString, QTimer*> prib_timer;
	QTimer* tmp_timer1;
	QMap<QString, QString> last_pxi;
	/**
	 *	\brief Асинхронный сигнал, передающий клиенту сообщение в журнал
	 *	\param message - текст сообщения
	 */
signals:
	void toLog(QString message);
	//exception
	void toProtocol(QString message);
	
public slots:
	QString getXML();
	
	int DOBAVIT_PRIBOR_AFAR(QString IMYA_PRIBORA, QString SETEVOJ_ADRES, int PORT);
	int UDALIT_PRIBOR_AFAR(QString IMYA_PRIBORA);
	int PODKLYUCHIT_PRIBOR_AFAR(QString IMYA_PRIBORA);

	int OTKLYUCHIT_PRIBOR_AFAR(QString IMYA_PRIBORA);
	int ZAGRUZIT_KOMANDU_P_AFAR(QString IMYA_PRIBORA, QString KOMANDA, int VREMYA_OZHIDANIYA = 0);
	int POLUCHIT_SOOBSSHENIE_P_AFAR(QString IMYA_PRIBORA, QVariant& IMYA_PEREMENNOJ);
	int UPRAVLENIE_SPDT_KOMMUTATORAMI(QVariantList mass);
	
	int ZAPISAT_ZNACH_SIGNALA_BD(int code, QVariant& val);
private:
	///Тип списка подключенных модулей
	typedef QMap<QString, HWModule*> MODULE_LIST;
	///Список модулей
	MODULE_LIST lst_modules;
	std::shared_ptr<TMDBController> db_ctrl;
};
#endif
