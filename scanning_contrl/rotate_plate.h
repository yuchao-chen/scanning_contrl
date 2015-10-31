#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <math.h>
#include <QDebug>
#include <boost/lexical_cast.hpp>
#include "timeout_serial.h"
const std::string DefaultTermination("\x0A");
const std::string Space("\x20");
const std::string DeviceReadyStatus("\x07");
const std::string Ready("\xB1");
const std::string NotReady("\xB0");
const std::string Reboot("RBT" + DefaultTermination);
const std::string RequestCloseLoopState("SVO?" + Space + "1" + DefaultTermination);
const std::string CloseLoop("SVO" + Space + "1" + Space + "1" + DefaultTermination);
const std::string MoveToReferenceSwitch("FRF" + Space + "1" + DefaultTermination);
const std::string RequestReferenceResult("FRF?" + Space + "1" + DefaultTermination);
const std::string RequestMotionStatus("\x05");
const std::string Move("MOV");

class PIController {
public:
	PIController() {
	}

	~PIController() {
		serial_.close();
	}

	void open(std::string port, int baud) {
		try {
			serial_.close();
			serial_.open(port, baud);
			serial_.setTimeout(boost::posix_time::seconds(5));
			std::cout << " [+] Open Serial OK. " << port << " " << baud << std::endl;
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Open Serial Failed:" << e.what() << std::endl;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Open Serial Failed:" << e.what() << std::endl;
		}
	}

	void identification() {
		std::string reply;
		if (serial_.isOpen()) {
			try {
				serial_.writeString("*IDN?\n");
				reply = serial_.readStringUntil(DefaultTermination);
				qDebug() << " [+] Get Device Identification OK. " << QString::fromStdString(reply);
			} catch (boost::system::system_error& e) {
				std::cout << "Error: " << e.what() << std::endl;
				std::cout << " [-] Get Device Identification Failed. " << e.what() << std::endl;
				return;
			} catch (const std::runtime_error& e) {
				std::cout << e.what() << std::endl;
				std::cout << " [-] Get Device Identification Failed. " << std::endl;
				return;
			}
		}
		return;
	}

	bool ready() {
		try {
			serial_.writeString(DeviceReadyStatus);
			std::string reply = serial_.readStringUntil(DefaultTermination);
			unsigned short r = reply[0];
			qDebug() << " [+] Request Motion Status OK. " << std::hex << r;
			if (reply[0] == 0xFFFFFFB1 || reply[0] == 0xB1) {
				return true;
			} else {
				return false;
			}
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Request Motion Status Failed. " << e.what() << std::endl;
			return NULL;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Request Motion Status Failed. " << e.what() << std::endl;
			return NULL;
		}
	}

	void close_loop() {
		try {
			//serial_.writeString(RequestCloseLoopState);
			//std::cout << serial_.readStringUntil(DefaultTermination);
			serial_.writeString(CloseLoop);
			std::cout << " [+] CloseLoop OK. " << std::endl;
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] CloseLoop Failed. " << e.what() << std::endl;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] CloseLoop Failed. " << e.what() << std::endl;
		}
	}

	void reboot() {
		try {
			serial_.writeString(Reboot);
			std::cout << " [+] Reboot OK. " << std::endl;
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Reboot Failed. " << e.what() << std::endl;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Reboot Failed. " << e.what() << std::endl;
		}
	}
	void wait_for_pos(double pos) {
		std::cout << "Waiting for Pos" << pos << std::endl;
		double d0 = 0.0;
		int count = 0;
		try {
			while (true) {
				//boost::this_thread::sleep(boost::posix_time::milliseconds(80));
				serial_.writeString("POS?\n");
				std::string sentence = serial_.readStringUntil("\n");
				if (!sentence.empty()) {
					qDebug() << QString::fromStdString(sentence);
					break;
				}
				
				//if (!sentence.empty()) {
				//	std::vector<std::string> results;
				//	std::string delimiters = "=";
				//	if (NVST::Utils::ParseString(sentence, delimiters, results) != 0 || results.empty()) {
				//		std::cout << "Oops, ParseString in Halpha ControllerI Dispatch Error.\n";
				//	} else {
				//		double p = abs(pos - boost::lexical_cast<double>(results[1]));
				//		std::cout << "D: " << p << std::endl;
				//		if (p != d0) {
				//			d0 = p;
				//		} else {
				//			count++;
				//		}
				//		if ((p < 0.001) || (count > 0)) {
				//			break;
				//		}
				//	}
				//}
			}
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Move To " << pos << " Failed. " << e.what() << std::endl;
			return;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Move To " << pos << " Failed. " << e.what() << std::endl;
			return;
		}
	}
	void move_to_reference_switch() {
		try {
			serial_.writeString("RON 1 0\n");
			serial_.writeString(MoveToReferenceSwitch);
			wait_for_pos(0);
			std::cout << " [+] Move To Reference Switch OK. " << std::endl;
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Move To Reference Switch Failed. " << e.what() << std::endl;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Move To Reference Switch Failed. " << e.what() << std::endl;
		}
	}

	void move_to(double pos) {
		try {
			std::ostringstream ss;
			ss << std::fixed << std::setprecision(3);
			ss << pos;
			std::string command = "MOV 1 " + ss.str() + DefaultTermination;
			serial_.writeString(command);
			std::cout << "Sending Command: " << command << std::endl;
			wait_for_pos(pos);
			std::cout << " [+] Move To " << pos << " OK." << std::endl;
			//getchar();
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Move To " << pos << " Failed. " << e.what() << std::endl;
			return;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Move To " << pos << " Failed. " << e.what() << std::endl;
			return;
		}
	}

	int motion_status() {
		try {
			serial_.writeString(RequestMotionStatus);
			std::string reply = serial_.readStringUntil(DefaultTermination);
			unsigned short r = reply[0];
			std::cout << " [+] Request Motion Status OK." << std::hex << r << std::endl;
			if (reply[0] == 0x0 || reply[0] == 0xFFFFFFF0) {
				return 0;
			} else {
				return 1;
			}
		} catch (boost::system::system_error& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << " [-] Request Motion Status Failed. " << e.what() << std::endl;
			return -1;
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			std::cout << " [-] Request Motion Status Failed. " << e.what() << std::endl;
			return -1;
		}
	}

	void Test() {
		identification();
		//reboot();
		std::cout << ready() << std::endl;
		close_loop();
		getchar();
		move_to_reference_switch();
		//move_to(0);
		getchar();
		move_to(122.5);
		std::cout << motion_status() << " " << std::endl;
		getchar();
		move_to_reference_switch();
	}
private:
	TimeoutSerial serial_;
};