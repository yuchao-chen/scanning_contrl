#ifndef SCANNING_CONTRL_H
#define SCANNING_CONTRL_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "ui_scanning_contrl.h"
#include "dev_contrl.h"
namespace ic {
	class ScanningContrl : public QMainWindow {
		Q_OBJECT
	public:
		ScanningContrl(QWidget *parent = 0);
		~ScanningContrl();
	private slots:
		void GoToSlot();
		void GoHomeSlot();
		void UpdateStatus(QString);
	private:
		void UpdateGUI();
		Ui::ScanningContrlClass ui;
		ic::DevContrl dev_;
		QTimer *run_dev_timer_;
	};
}
#endif // SCANNING_CONTRL_H
