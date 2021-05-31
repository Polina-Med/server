#include "afar_server_client_visualize_plugin.h"
#include <qdatetime.h>
#include <qfileinfo.h>
#include <qdatastream.h>
#include <qboxlayout.h>
#include <QStackedLayout>
#include <qlayout.h>
#include <qgroupbox.h>
AfarWidgetVisualize* AfarClientVisualizePlugin::widg = NULL;

QString AfarWidgetVisualize::GreySt = "QLabel {"
"border-style: solid;"
"border-width: 1px;"
"border-color: black; "
"text-alignment:left;"
"vertical-align:baseline;"
"font-size:14px"
										
	"}";


QString AfarWidgetVisualize::Grey = "QLabel {"

"text-alignment:left;"
"vertical-align:baseline;"
"font-size:14px"
"}";

QString AfarWidgetVisualize::GreenSt = "QLabel { background-color : qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(172, 209, 156), stop: 1 rgb(138, 191, 115)); color : black;}";

QWidget* AfarClientVisualizePlugin::getWidget()
{
	if (widg== NULL)
		widg = new AfarWidgetVisualize();


	return widg;
}

AfarWidgetVisualize::AfarWidgetVisualize(QWidget* parent) : QWidget(parent), db_th("ТМ АФАР afar")
{
	QVBoxLayout* main_lay = new QVBoxLayout(this);
	QGridLayout* Af = new QGridLayout();
	QGroupBox *box = new QGroupBox;

	pow1 = new QLabel;
	pow1->setStyleSheet(Grey);
	pow2 = new QLabel;
	pow2->setStyleSheet(Grey);
	vol1 = new QLabel;
	vol1->setStyleSheet(Grey);
	vol2 = new QLabel;
	vol2->setStyleSheet(Grey);
	amp1 = new QLabel;
	amp1->setStyleSheet(Grey);
	amp2 = new QLabel;
	amp2->setStyleSheet(Grey);
	rez_mko = new QLabel;
	
	rez_mko->setStyleSheet(GreySt);
	rez_mko->setFixedHeight(30);
	ist_pit = new QLabel;
	ist_pit->setStyleSheet(GreySt);
	ist_pit->setFixedHeight(30);
	QLabel *afar1 = new QLabel("ИП АФАР1");
	afar1->setStyleSheet(Grey);
	QLabel *afar2 = new QLabel("ИП АФАР2");
	afar2->setStyleSheet(Grey);
	power = new QLabel;
	power->setFixedHeight(50);
	power->setStyleSheet(GreySt);
	rez = new QLabel;
	rez->setFixedHeight(50);
	rez->setStyleSheet(GreySt);
	updateAfar();
	Af->addWidget(afar1, 0, 0);
	Af->addWidget(afar2, 0, 1);
	Af->addWidget(amp1, 2, 0);
	Af->addWidget(amp2, 2, 1);
	Af->addWidget(vol1, 1, 0);
	Af->addWidget(vol2, 1, 1);
	Af->addWidget(pow1, 3, 0);
	Af->addWidget(pow2, 3, 1);
	Af->setSpacing(0);
	box->setLayout(Af);
	
	main_lay->addWidget(rez_mko);
	main_lay->addWidget(ist_pit);
	main_lay->addWidget(box);
	box->setStyleSheet("QGroupBox { border: 1px solid black}");
	main_lay->addWidget(power);
	main_lay->addWidget(rez);
	main_lay->setSpacing(1);

	DBCallback cr_f = std::bind(&DBCreator::convertData, &db_cr, std::placeholders::_1);

	db_th.addCallBackFunction(cr_f);
	db_th.start();
	connect(&db_cr, &DBCreator::needProcessData, this, &AfarWidgetVisualize::MsgFromDB, Qt::BlockingQueuedConnection);
	QThread::currentThread()->sleep(2);
	db_th.startRead();
}

void AfarWidgetVisualize::updateAfar()
{
	pow1 -> setText("- Вт");
	pow2->setText("- Вт");
	vol1 ->setText("- В");
	vol2->setText("- В");
	amp1->setText("- A");
	amp2->setText("- A");
	rez_mko->setText("MKO 'O'");
	ist_pit->setText("Источники питания");
	power ->setText("Мощность потребления\n -Bт");
	rez ->setText("Режим работы  \n ");
	p_pow1 = 0;
	p_pow2 = 0;
}

void DBCreator::convertData(const std::vector<QVariant>& msg_list)
{
	emit needProcessData(msg_list);
}

int AfarWidgetVisualize::MsgFromDB(const std::vector<QVariant>& msg_list)
{
	foreach(const QVariant& msg_q, msg_list)
	{
		MsgFromTMDB msg = qvariant_cast<MsgFromTMDB>(msg_q);
		int tmp_code = msg.tm_code;
		QVariant data = msg.value;

		if (tmp_code==3661)
			if (data.toInt() == 0)
			{
				updateAfar();
				rez_mko->setStyleSheet(GreySt);
			}
			else 
				rez_mko->setStyleSheet(GreySt+ GreenSt);

		if (tmp_code == 1554)
		{
			QString msg = (QString("MKO '%1'").arg(data.toString()));
			rez_mko->setText(msg);
		}
		if (tmp_code == 1)

			if (data.toInt() == 0)
			{
				QString msg = ("Режим работы \nвекторного анализатора");
				rez->setText(msg);
			}
			else
			{
				QString msg = ("Режим работы \nгенератора сигналов");
				rez->setText(msg);
			}
	
		if (tmp_code == 43)
		{
			p_pow1 = data.toInt();
			QString msg = (QString("%1 Bт").arg(p_pow1));
			pow1->setText(msg);
			sum_power = 1;
		}

		if (tmp_code == 53)
		{
			p_pow2 = data.toInt();
			pow2->setText(QString("%1 Bт").arg(p_pow2));
			sum_power = 1;
		}

		if (sum_power == 1)
		{
			int tmp = p_pow1 + p_pow2;
			power->setText(QString("Мощность потребления\n %1 Bт").arg(tmp));
			sum_power = 0;
		}

		if (tmp_code == 41)
		{
			QString msg = (QString("%1 A").arg(data.toInt()));
			amp1->setText(msg);
		}

		if (tmp_code == 51)
		{
			QString msg = (QString("%1 A").arg(data.toInt()));
			amp2->setText(msg);
		}

		if (tmp_code == 42)
		{
			QString msg = (QString("%1 B").arg(data.toInt()));
			vol1->setText(msg);
		}

		if (tmp_code == 52)
		{
			QString msg = (QString("%1 B").arg(data.toInt()));
			vol2->setText(msg);
		}
	}

	return 0;

}

void AfarClientVisualizePlugin::clearWidget()
{
	if (widg != NULL)
		delete widg;
}


