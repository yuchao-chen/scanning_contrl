#include <dev_contrl.h>
namespace ic {
	DevContrl::DevContrl()
		: config_(data::AttributeTable::create()) {
			serial_port_.close();
	}

	DevContrl::~DevContrl() {
		serial_port_.close();
	}

	std::vector<std::string> DevContrl::FindAvailablePorts() {
		std::vector<std::string> ports;
		foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
			ports.push_back(serial_port_info.portName().toStdString());
		}
		if (!ports.empty()) {
			config_->insert("PORTNAME", ports[0]);
		}
		return ports;
	}

	void DevContrl::set(data::AttributeTablePtr config) {
	}

	data::AttributeTablePtr DevContrl::get(data::AttributeTablePtr config) {
		data::AttributeTablePtr r = data::AttributeTable::create();
		if (config->get_string("NAME") == "AVAILABLEPORTS") {
			std::vector<std::string> ports = FindAvailablePorts();
			r->insert("AVAILABLEPORTS", ports);
		}
		return r;
	}

	int DevContrl::InitPort(std::string port_name) {
		if (port_name.empty()) {
			return -1;
		}
		QMutexLocker locker(&mutex_);

		serial_port_.close();
		serial_port_.setPortName(QString::fromStdString(port_name));
		if (!serial_port_.open(QIODevice::ReadWrite)) {
			return -1;
		}
		if (!serial_port_.setBaudRate(QSerialPort::Baud4800)) {
			return -1;
		}
		if (!serial_port_.setParity(QSerialPort::NoParity)) {
			return -1;
		}

		if (!serial_port_.setStopBits(QSerialPort::OneStop)) {
			return -1;
		}
		if (!serial_port_.setDataBits(QSerialPort::Data8)) {
			return -1;
		}
		if (!serial_port_.setFlowControl(QSerialPort::NoFlowControl)) {
			return -1;
		}
		return 0;
	}

	int DevContrl::InitDevSetting() {
		Write(command::SetStageStyle,50);SyncRead(50);
		Write(command::SetUnit,50);SyncRead(50);
		Write(command::SetStageStepsRev,50);SyncRead(50);
		Write(command::SetStagePitch,50);SyncRead(50);
		Write(command::SetStageWorkState,50);SyncRead(50);
		Write(command::SetOffset,50);SyncRead(50);
		Write(command::SetSpeed,50);SyncRead(50);
		Write(command::SetInitSpeed,50);SyncRead(50);
		Write(command::SetAccSpeed,50);SyncRead(50);
		Write(command::SetHomeSpeed,50);SyncRead(50);
		Write(command::Setorigin,50);SyncRead(50);
		Write(command::SetSechHomemode,50);SyncRead(50);
		GoHome();
		SyncRead(1000);
		return 0;
	}

	int DevContrl::GoHome() {
		Write(command::GoHome,50);SyncRead(50);
		return 0;
	}
	int DevContrl::GoOrigin() {
		Write(command::GoOrigion,50);SyncRead(50);
		return 0;
	}
	int DevContrl::Stop() {
		Write(command::Stop,50);SyncRead(50);
		return 0;
	}
	int DevContrl::GoTo(std::string pos) {
		std::string c = command::Stop + pos;

		Write(c,50);SyncRead(50);
		return 0;
	}
	int DevContrl::Write(std::string m, int t) {
		if (!serial_port_.isOpen()) {
			std::string port_name = config_->get_string("PORTNAME");
			if (InitPort(port_name) < 0) {
				return -1;
			}
		}
		serial_port_.write(m.c_str());
		if (!serial_port_.waitForBytesWritten(t)) {
			return -1;
		}
		return 0;
	}
	QString DevContrl::SyncRead(int waittime) {
		QByteArray response;
		if (!serial_port_.isOpen()) {
			std::string port_name = config_->get_string("PORTNAME");
			if (InitPort(port_name) < 0) {
				return response;
			}
		}
		if (serial_port_.waitForReadyRead(waittime)) {
			response = serial_port_.readAll();
		}
		return response;
	}
}