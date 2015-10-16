#include "scanning_contrl.h"

namespace ic {
	ScanningContrl::ScanningContrl(QWidget *parent)
		: QMainWindow(parent) {
			ui.setupUi(this);
			QStringList available_ports = dev_.available_ports();
			ui.port_comobox->addItems(available_ports);
			//for (int i = 0; i < available_ports.size(); i++) {
			//	ui.port_comobox->addItem();
			//}
		//foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		//	ui.port_comobox->addItem(info.portName());
		//}
	}

	ScanningContrl::~ScanningContrl() {

	}
}