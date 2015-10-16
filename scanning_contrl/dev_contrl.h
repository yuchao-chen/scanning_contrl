#ifndef DEV_CONTRL_H
#define DEV_CONTRL_H
#include <QStringList>
#include <QString>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
namespace ic {
	class DevContrl {
	public:
		DevContrl();
		~DevContrl();
		QStringList available_ports();
	private:
		QSerialPort serial_port_;
	};
}
#endif