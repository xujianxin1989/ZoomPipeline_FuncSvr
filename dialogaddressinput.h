﻿#ifndef DIALOGADDRESSINPUT_H
#define DIALOGADDRESSINPUT_H

#include <QDialog>

namespace Ui
{
	class DialogAddressInput;
}

class DialogAddressInput : public QDialog
{
	Q_OBJECT

public:
    explicit DialogAddressInput(QWidget* parent = 0);
	~DialogAddressInput();
    void setAddr(const QString& addr, const QString& port);
	QString addr();
	QString port();

private:
    Ui::DialogAddressInput* ui;
};

#endif // DIALOGADDRESSINPUT_H
