#ifndef SCANNING_CONTRL_H
#define SCANNING_CONTRL_H

#include <QtWidgets/QMainWindow>
#include "ui_scanning_contrl.h"

class ScanningContrl : public QMainWindow
{
	Q_OBJECT

public:
	ScanningContrl(QWidget *parent = 0);
	~ScanningContrl();

private:
	Ui::ScanningContrlClass ui;
};

#endif // SCANNING_CONTRL_H
