#include "MWSwitchModule.h"
#include "AgMWSwitch.h"
#include "qtextcodec.h"
#include "rpc_loger.h"

MWSwitchModule::MWSwitchModule()
{
	renewDevices();

	
}


void MWSwitchModule::renewDevices()
{

//	searchDevices<MWSwitchDevice>("MWSwitch", 0x010D);
	addDevice<MWSwitchDevice>("MWSwitch1", "PXI17::8::0::INSTR");
	addDevice<MWSwitchDevice>("MWSwitch2", "PXI19::10::0::INSTR");
	addDevice<MWSwitchDevice>("MWSwitch3", "PXI19::15::0::INSTR");
}


void MWSwitchDevice::connect_channel(QString first_chan, QString second_chan)
{
	AgMWSwitch_Connect(handler, first_chan.toLocal8Bit(), second_chan.toLocal8Bit());
}

void MWSwitchDevice::close_channel(QString chan_name)
{
	SRPCSignalClass::Instance().toLog("switch close");
	status = AgMWSwitch_RouteCloseChannel(handler, chan_name.toLocal8Bit());
	if (status)
		format_message("AGSwitch: Ошибка подключения канала %i (%s)", status);
}

void MWSwitchDevice::init()
{
	//SRPCSignalClass::Instance().toLog("switch init");
	status = AgMWSwitch_init(get_rsrc_addr(), VI_TRUE, VI_TRUE, &handler);
	if (status)
		format_message("AGSwitch: Ошибка инициализации %i (%s)", status);
	//set_dev_num(1,)

}


void MWSwitchDevice::deinit()
{
	AgMWSwitch_close(handler);
}


void MWSwitchDevice::format_message(const char* msg, int err_code)
{
	char tmp[max_buf_size] = { 0 };
	AgMWSwitch_error_message(handler, err_code, tmp);
	static QTextCodec* codec = QTextCodec::codecForName("CP1251");
	throw AGSWITCH_Err((boost::format(msg) % err_code % codec->toUnicode(tmp).toLocal8Bit().data()).str().c_str(), err_code);
}