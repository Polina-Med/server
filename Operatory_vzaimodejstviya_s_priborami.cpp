#include "afar_server_application.h"
#include "SAModule/SAModule.h"
#include "SGModule/SGModule.h"
#include "NAModule/NAModule.h"
#include "MWSwitchModule/MWSwitchModule.h"
#include "instruments.h"

int  AfarSrvAppl::DOBAVIT_PRIBOR_AFAR(QString IMYA_PRIBORA, QString SETEVOJ_ADRES, int PORT)
{
	return addInstrument(IMYA_PRIBORA, SETEVOJ_ADRES, PORT);
}

int  AfarSrvAppl::UDALIT_PRIBOR_AFAR(QString IMYA_PRIBORA)
{
	if (!(pribor_map.contains(IMYA_PRIBORA)))
	{
		toProtocol(QString("Невозможно удалить%1").arg(IMYA_PRIBORA));
		return LOGIC_ERROR;

	}
	else
		return removeInstrument(IMYA_PRIBORA);
}

int  AfarSrvAppl::PODKLYUCHIT_PRIBOR_AFAR(QString IMYA_PRIBORA)
{
	if (!pribor_map.contains(IMYA_PRIBORA))
	{
		toProtocol(QString("Ошибка подключения прибора \"%1\", прибора с таким именем нет в списке").arg(IMYA_PRIBORA));
		return LOGIC_ERROR;
	}
	pribor_map.value(IMYA_PRIBORA)->start();
	return SUCCESS;
}

int  AfarSrvAppl::OTKLYUCHIT_PRIBOR_AFAR(QString IMYA_PRIBORA)
{
	if (!pribor_map.contains(IMYA_PRIBORA))
	{
		toProtocol(QString("Ошибка отключения прибора \"%1\", прибора с таким именем нет в списке").arg(IMYA_PRIBORA));
		return LOGIC_ERROR;
	}
	pribor_map.value(IMYA_PRIBORA)->quit();
	return SUCCESS;
}

int  AfarSrvAppl::ZAGRUZIT_KOMANDU_P_AFAR(QString IMYA_PRIBORA, QString KOMANDA, int VREMYA_OZHIDANIYA)
{
	if (VREMYA_OZHIDANIYA == 0)
		VREMYA_OZHIDANIYA = 3;

	if (pribor_map.contains(IMYA_PRIBORA))
	{
		if (pribor_map.value(IMYA_PRIBORA)->isRunning())
		{
			if (KOMANDA.contains("?"))
			{
				pribor_map[IMYA_PRIBORA]->queryCommand(KOMANDA, VREMYA_OZHIDANIYA * 1000);
				toProtocol(QString("%1").arg(pribor_map[IMYA_PRIBORA]->last_res));
			}
			else
				pribor_map[IMYA_PRIBORA]->sendCommand(KOMANDA);
			return SUCCESS;
		}
		else
		{
			toProtocol(QString("Ошибка! \"%1\" не подключен").arg(IMYA_PRIBORA));
			return LOGIC_ERROR;
		}
	}
	

}

int  AfarSrvAppl::POLUCHIT_SOOBSSHENIE_P_AFAR(QString IMYA_PRIBORA, QVariant& IMYA_PEREMENNOJ)
{
	if (pribor_map.contains(IMYA_PRIBORA))
	{
		IMYA_PEREMENNOJ.clear();
		IMYA_PEREMENNOJ = pribor_map[IMYA_PRIBORA]->last_res;
		return SUCCESS;
	}
	else 
	{
		
		toProtocol(QString("Ошибка  команды \"%1\", прибора с таким именем нет в списке").arg(IMYA_PRIBORA));
		return LOGIC_ERROR;
		
	}
}



int AfarSrvAppl::UPRAVLENIE_SPDT_KOMMUTATORAMI(QVariantList mass)
{
	int num_sw;
	int bee;
	int ch;
	int count1 = mass.count();
	for (int i = 0; i <count1; i++)
	{
		QVariantList massofmass = mass[i].toList();
		num_sw = massofmass[0].toInt();
		bee = massofmass[1].toInt();
		ch = massofmass[2].toInt();
		getModuleReference<MWSwitchModule>().switchDevice(QString("MWSwitch%1").arg(num_sw));
		
		QString chan = (QString("b%1ch%2").arg(bee).arg(ch));
		(*getModuleReference<MWSwitchModule>())->close_channel(chan);
		
	}

	return SUCCESS;
}


int AfarSrvAppl::addInstrument(QString IMYA, QString IP, int PORT)
{
	if (pribor_map.contains(IMYA))
	{
		toProtocol(QString("Ошибка добавления прибора \"%1\": такой прибор уже есть в списке").arg(IMYA));
		return LOGIC_ERROR;
	}
	else
	{
		QString _addr = "";
		_addr = QString("%1:%2:").arg(IP).arg(PORT);
		SCPIDevice* tmp_device = new SCPIDevice(_addr.toStdString().c_str());
		pribor_map.insert(IMYA, tmp_device);

	}
	return SUCCESS;
}

int AfarSrvAppl::removeInstrument(QString IMYA)
{
	if (!pribor_map.contains(IMYA))
	{
		toProtocol(QString("Ошибка удаления прибора \"%1\", прибора с таким именем нет в списке").arg(IMYA));
		return LOGIC_ERROR;
	}
	else

	{
		pribor_map.value(IMYA)->deleteLater();
		pribor_map.remove(IMYA);
	}
	return SUCCESS;
}

