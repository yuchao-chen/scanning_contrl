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
#include "attribute_table.h"
namespace ic {
	namespace command {
		const std::string SetStageStyle("SetStageStyle Y,T");//设置台子类型
		const std::string SetUnit("SetUnit Y,u");//运行轴使用单位
		const std::string SetStageStepsRev("SetStageStepsRev Y,1600");//设置每转脉冲数
		const std::string SetStagePitch("SetStagePitch Y,4");//设置台子丝杆导程
		const std::string SetStageWorkState("SetStageWorkState Y,O");//设置台子开闭环状态
		const std::string SetOffset("SetOffset Y,45000");//设置回程值,20mm
		const std::string SetSpeed("SetSpeed Y,15000");//设置运行轴常速度
		const std::string SetInitSpeed("SetInitSpeed Y,6000");//设置运行轴初速度
		const std::string SetAccSpeed("SetAccSpeed Y,10000");	//设置运行轴加速度
		const std::string SetHomeSpeed("SetHomeSpeed Y,4000");//设置运行轴回原点速度
		const std::string Setorigin("Setorigin Y,P");//设置归零模式； P=物理方式；N=用户原点
		const std::string SetSechHomemode("Setsechhomemode Y,2");//设置归零方式；1=机械开关；2=光电开关
		const std::string GoHome("GoHome Y");
		const std::string GoOrigion("GoOrigion Y");
		const std::string Stop("Stop Y");
		const std::string GoPosition("GoPosition Y,O,R,P,");
	}
	class DevContrl: public QObject {
		Q_OBJECT
	public:
		
		DevContrl();
		~DevContrl();
		void set(data::AttributeTablePtr config);
		data::AttributeTablePtr get(data::AttributeTablePtr config);

	private:
		std::vector<std::string> FindAvailablePorts();
		
		int InitPort(std::string port);
		int InitDevSetting();
		int GoHome();
		int GoOrigin();
		int Stop();
		int GoTo(std::string pos);
		int Write(std::string command, int waittime);
		QString SyncRead(int waittime);
		QSerialPort serial_port_;
		data::AttributeTablePtr config_;
		QMutex mutex_;
	};
}
#endif