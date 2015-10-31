#include "scanning_contrl.h"

namespace ic {
	ScanningContrl::ScanningContrl(QWidget *parent)
		: QMainWindow(parent),
		run_dev_timer_(new QTimer(this)) {
			ui.setupUi(this);
			UpdateGUI();
	}

	ScanningContrl::~ScanningContrl() {

	}
	void ScanningContrl::UpdateGUI() {
		data::AttributeTablePtr config = data::AttributeTable::create();
		config->insert("NAME", "AVAILABLEPORTS");
		std::vector<std::string> ports = dev_.get(config)->get_string_array("AVAILABLEPORTS");
		for (int i = 0; i < ports.size(); i++) {
			ui.port_comobox->addItem(QString::fromStdString(ports[i]));
		}
		ui.goto_spinbox->setRange(-100000, 100000);
		ui.step_no_spinbox->setRange(1, 1000000);
		ui.step_no_spinbox->setValue(150);
		ui.step_size_spinbox->setValue(50);
		connect(ui.go_pushbutton, SIGNAL(clicked()), this, SLOT(GoToSlot()));
		connect(ui.go_home_pushbutton, SIGNAL(clicked()), this, SLOT(GoHomeSlot()));
		connect(&dev_, SIGNAL(UpdateStaus(QString)), this, SLOT(UpdateStatus(QString)));
	}

	void ScanningContrl::GoToSlot() {
		data::AttributeTablePtr config = data::AttributeTable::create();
		config->insert("PORTNAME", ui.port_comobox->currentText().toStdString());
		config->insert("COMMAND", "GOTO");
		config->insert("POS", ui.goto_spinbox->value()*2);
		dev_.set(config);
	}
	void ScanningContrl::GoHomeSlot() {
		data::AttributeTablePtr config = data::AttributeTable::create();
		config->insert("PORTNAME", ui.port_comobox->currentText().toStdString());
		config->insert("COMMAND", "GOHOME");
		dev_.set(config);
	}

	void ScanningContrl::UpdateStatus(QString status) {
		QString text = status;
		/*if (text.contains("Y") && text.contains("OK")) {
			
		}*/
		ui.status_label->setText(status);
	}
}