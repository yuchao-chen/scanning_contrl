#include <dev_contrl.h>
namespace ic {

	QStringList DevContrl::available_ports() {
		QStringList ports;
		foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
			ports.push_back(serial_port_info.portName());
		}
		return ports;
	}
}