#include "dev_contrl.h"
#include <iostream>
namespace ic {
	DevContrl::DevContrl()
		: config_(data::AttributeTable::create()) {
			serial_port_.close();
			//InitPort("COM6", 19200);
			//InitDevSetting();
			//GoTo("3000");
	}

	DevContrl::~DevContrl() {
		serial_port_.close();
	}

	std::vector<std::string> DevContrl::FindAvailablePorts() {
		std::vector<std::string> ports;
		foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
			ports.push_back(serial_port_info.portName().toStdString());
		}
		return ports;
	}

	void DevContrl::set(data::AttributeTablePtr config) {
		std::string port_name = config->get_string("PORTNAME");
		std::string cur_port_name = config_->get_string("PORTNAME");
		if (!serial_port_.isOpen() || port_name != cur_port_name) {
			InitPort(port_name, 19200);
			InitDevSetting();
		}
		qDebug() << ".............................................";
		if (config->contains("COMMAND")) {
			std::string command = config->get_string("COMMAND");
			//qDebug() << QString::fromStdString(command);
			if (command == "GOHOME") {
				GoHome();
			} else if (command == "GOTO") {
				GoTo(config->get_long("POS"));
			}
		}
	}

	data::AttributeTablePtr DevContrl::get(data::AttributeTablePtr config) {
		data::AttributeTablePtr r = data::AttributeTable::create();
		if (config->get_string("NAME") == "AVAILABLEPORTS") {
			std::vector<std::string> ports = FindAvailablePorts();
			r->insert("AVAILABLEPORTS", ports);
		}
		return r;
	}

	int DevContrl::InitPort(std::string port_name, int baud) {
		if (port_name.empty()) {
			return -1;
		}
		QMutexLocker locker(&mutex_);

		try {
			serial_port_.close();
			serial_port_.open(port_name, baud);
			serial_port_.setTimeout(boost::posix_time::seconds(10));
			config_->insert("PORTNAME", port_name);
			qDebug() << " [+] Open Serial OK. " << QString::fromStdString(port_name) << " " << baud;
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Open Serial Failed:" << e.what() << std::endl;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Open Serial Failed:" << e.what() << std::endl;
		}
		return 0;
	}

	int DevContrl::InitDevSetting() {
		Write(command::SetStageStyle);WaitForReady();
		Write(command::SetUnit);WaitForReady();
		Write(command::SetStageStepsRev);WaitForReady();
		Write(command::SetStagePitch);WaitForReady();
		Write(command::SetStageWorkState);WaitForReady();
		Write(command::SetOffset);WaitForReady();
		Write(command::SetSpeed);WaitForReady();
		Write(command::SetInitSpeed);WaitForReady();
		Write(command::SetAccSpeed);WaitForReady();
		Write(command::SetHomeSpeed);WaitForReady();
		Write(command::Setorigin);WaitForReady();
		Write(command::SetSechHomemode);WaitForReady();
		//GoHome();WaitForReady(50);WaitForPosition("0");
		return 0;
	}

	int DevContrl::GoHome() {
		Write(command::GoHome);WaitForReady();
		qDebug() << "**********************************************";
		//qDebug() << QString::fromStdString(SyncRead());
		//qDebug() << QString::fromStdString(SyncRead());
		//WaitForPosition("0");
		try {
			std::string r = SyncRead();
			if (r.find("OK") != std::string::npos) {
				emit UpdateStaus(QString::fromStdString(r));
				return 0;
			}
		} catch(...) {
			QThread::currentThread()->sleep(2);
		}
		return -1;
	}
	
	int DevContrl::GoOrigin() {
		Write(command::GoOrigion);SyncRead();
		return 0;
	}
	
	int DevContrl::Stop() {
		Write(command::Stop);SyncRead();
		return 0;
	}

	int DevContrl::GoTo(int pos) {
		std::string c;
		//c = command::Stop + "\r";
		//Write(c);
		//WaitForReady(50);
		//std::string cs = RequestPosition();
		//qDebug() << "1: " << QString::fromStdString(cs);
		//QThread::currentThread()->msleep(50);
		//int cp = 0;
		//if (!cs.empty()) {
		//	std::stringstream s0(cs);
		//	s0 >> cp;
		//}

		//qDebug() << cp << "....................." << pos;

		//QString s = QString::fromStdString(pos);
		//if (pos == "0" || pos == "-0" ) {
		//	c = command::GoHome + "\r";
		//} else if (s.contains("-")) {
		//	s.remove("-");
		//	c = command::GoNegativePosition + s.toStdString() + "\r";
		//} else {
		//	c = command::GoPositivePosition + pos + "\r";
		//}
		if (pos == 0) {
			return 0;
		} else if (pos > 0) {
			std::stringstream ss;
			ss << pos;
			c = command::GoPositivePosition + ss.str() + "\r";
			//qDebug() << QString::fromStdString(c);
		} else {
			//pos.erase(std::remove(pos.begin(), pos.end(), "-"), pos.end());
			std::stringstream ss;
			ss << -1 * pos;
			c = command::GoNegativePosition + ss.str() + "\r";
		}
		//int p = 0;
		//std::stringstream ss( pos );
		//ss >> p;
		//
		//if (p < 0) {
		//	
		//} else {
		//	
		//}
		Write(c);WaitForReady();
		qDebug() << "=========================================================";
		std::string r = SyncRead();
		if (r.find("OK") != std::string::npos) {
			emit UpdateStaus(QString::fromStdString(r));
			return 0;
		}
		//std::stringstream s1(pos + cp);
		//WaitForPosition(s1.str());
		return -1;
	}

	int DevContrl::Write(std::string m) {
		try {
			serial_port_.writeString(m);
			qDebug() << " [+] Send Message. " << QString::fromStdString(m);
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Reboot Failed. " << e.what() << std::endl;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Reboot Failed. " << e.what() << std::endl;
		}
		return 0;
	}

	std::string DevContrl::SyncRead() {
		std::string response;
		try {
			response = serial_port_.readStringUntil("\r");
			qDebug() << "....................." << QString::fromStdString(response);
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			//std::cout << " [-] Move To " << pos << " Failed. " << e.what() << std::endl;
			return response;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			//std::cout << " [-] Move To " << pos << " Failed. " << e.what() << std::endl;
			return response;
		}
		return response;
	}
	
	int DevContrl::WaitForReady() {
		//for (int i = 0; i < waittime/50; i++) {
		//	std::string r = SyncRead();
		//	if (r.find("OK") != std::string::npos || r.find("READY") != std::string::npos ) {
		//		return 0;
		//	}
		//	QThread::currentThread()->sleep(0.2);
		//}
		std::string r = SyncRead();
		if (r.empty()) {
			return -1;
		}
		if (r.find("OK") != std::string::npos || r.find("READY") != std::string::npos ) {
			return 0;
		}
		return -1;
	}

	int DevContrl::WaitForPosition(std::string value) {
		//	std::string c = command::RequestPosition + "\r";
		//	Write(c);
		//SyncRead();
		QThread::currentThread()->msleep(50);
		for (int i = 0; i < 10; i++) {
			std::string r = RequestPosition();
			if (r.find("POSITION? Y") != std::string::npos &&
				r.find("OK") != std::string::npos &&
				r.find(value) != std::string::npos) {
				break;
			}
			if (value == "0") {
				QThread::currentThread()->sleep(2);
			} else {
				QThread::currentThread()->msleep(100);
			}
		}
		return 0;
	}

	std::string DevContrl::RequestPosition() {
		std::string c = command::RequestPosition + "\r";
		Write(c);
		std::string pos = SyncRead();
		if (pos.empty()) {
			return pos;
		}
		//QString qs = QString::fromStdString(pos);
		//QStringList qsl = qs.split(",");
		//if (qsl.count() == 3 && 
		//	qsl[0] == QString::fromStdString(command::RequestPosition).toUpper() &&
		//	qsl[2] == "OK") {
		//		pos = qsl[1].toStdString();
		//} else {
		//	pos = "";
		//}
		
		return pos;
	}
}