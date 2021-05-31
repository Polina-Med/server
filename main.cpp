#include "afar_server_application.h"
#include "afar_server_socket_rpc.h"
#include <QTextCodec>
#include <QApplication>
#include <qstring.h>
#include <fstream>


#ifdef WIN32

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
	int argc = __argc;
	char** argv = __argv;

#else
int main(int argc, char **argv)
{
#endif

	QApplication app(argc, argv);
	SMongoSingleton_v3::Instance();
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	//QMessageBox::critical(0, "crab", "<_^____^_>");
	LogWidget log_widget;
	log_widget.show();
	AfarSrvAppl appobject;
	return app.exec();
}
