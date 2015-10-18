#include "scanning_contrl.h"

namespace ic {
	ScanningContrl::ScanningContrl(QWidget *parent)
		: QMainWindow(parent) {
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
	}
}