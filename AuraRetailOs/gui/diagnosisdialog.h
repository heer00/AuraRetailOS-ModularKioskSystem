#ifndef DIAGNOSISDIALOG_H
#define DIAGNOSISDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "core/diagnosisservice.h"
#include "core/Kiosk.h"

class DiagnosisDialog : public QDialog {
    Q_OBJECT
public:
    explicit DiagnosisDialog(Kiosk* kiosk, QWidget *parent = nullptr);

private slots:
    void onRunAgain();

private:
    QLabel* reportLabel;
    QPushButton* btnRunAgain;
    QPushButton* btnClose;
    DiagnosisService service;
    Kiosk* kiosk;
    
    void loadReport();
};

#endif
