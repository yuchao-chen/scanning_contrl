#ifndef DEV_CONTRL_H
#define DEV_CONTRL_H
#include <string>
#include <vector>
#include <QStringList>
#include <QString>
#include <QMutex>
#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "timeout_serial.h"
#include "attribute_table.h"
namespace ic {
	namespace command {
		const std::string SetStageStyle("SetStageStyle Y,T\r");//设置台子类型
		const std::string SetUnit("SetUnit Y,u\r");//运行轴使用单位
		const std::string SetStageStepsRev("SetStageStepsRev Y,1600\r");//设置每转脉冲数
		const std::string SetStagePitch("SetStagePitch Y,4\r");//设置台子丝杆导程
		const std::string SetStageWorkState("SetStageWorkState Y,O\r");//设置台子开闭环状态
		const std::string SetOffset("SetOffset Y,45000\r");//设置回程值,20mm
		const std::string SetSpeed("SetSpeed Y,15000\r");//设置运行轴常速度
		const std::string SetInitSpeed("SetInitSpeed Y,6000\r");//设置运行轴初速度
		const std::string SetAccSpeed("SetAccSpeed Y,10000\r");	//设置运行轴加速度
		const std::string SetHomeSpeed("SetHomeSpeed Y,4000\r");//设置运行轴回原点速度
		const std::string Setorigin("Setorigin Y,P\r");//设置归零模式； P=物理方式；N=用户原点
		const std::string SetSechHomemode("Setsechhomemode Y,2\r");//设置归零方式；1=机械开关；2=光电开关
		const std::string GoHome("GoHome Y\r");
		const std::string GoOrigion("GoOrigion Y\r");
		const std::string Stop("Stop Y\r");
		const std::string GoPositivePosition("GoPosition Y,O,R,P,");
		const std::string GoNegativePosition("GoPosition Y,O,R,N,");
		const std::string RequestPosition("Position? Y");
	}
	class DevContrl: public QObject {
		Q_OBJECT
	public:
		
		DevContrl();
		~DevContrl();
		void set(data::AttributeTablePtr config);
		data::AttributeTablePtr get(data::AttributeTablePtr config);
	signals:
		void UpdateStaus(QString);
	private:
		std::vector<std::string> FindAvailablePorts();
		
		int InitPort(std::string port, int baud);
		int InitDevSetting();
		int GoHome();
		int GoOrigin();
		int Stop();
		int GoTo(int pos);
		int Write(std::string command);
		std::string SyncRead();
		int WaitForPosition(std::string v);
		int WaitForReady();
		std::string RequestPosition();

		TimeoutSerial serial_port_;
		data::AttributeTablePtr config_;
		QMutex mutex_;
	};
}
#endif