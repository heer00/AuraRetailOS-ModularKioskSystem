#include "diagnosisdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

DiagnosisDialog::DiagnosisDialog(Kiosk* kiosk, QWidget *parent) 
    : QDialog(parent), kiosk(kiosk) {
    setWindowTitle("System Diagnosis");
    setMinimumSize(400, 500);
    setStyleSheet("QDialog { background-color: #121212; color: #ffffff; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    reportLabel = new QLabel();
    reportLabel->setTextFormat(Qt::RichText);
    reportLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    reportLabel->setFont(QFont("Consolas", 10));
    reportLabel->setStyleSheet("QLabel { background-color: #1e1e1e; color: #d4d4d4; padding: 10px; border-radius: 5px; }");
    
    mainLayout->addWidget(reportLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnRunAgain = new QPushButton("Run Again");
    btnClose = new QPushButton("Close");
    
    QString btnStyle = "QPushButton { background-color: #333333; color: white; padding: 8px 15px; border: none; border-radius: 4px; font-weight: bold; } "
                       "QPushButton:hover { background-color: #444444; } "
                       "QPushButton:pressed { background-color: #555555; }";
    
    btnRunAgain->setStyleSheet(btnStyle);
    btnClose->setStyleSheet(btnStyle);

    btnLayout->addStretch();
    btnLayout->addWidget(btnRunAgain);
    btnLayout->addWidget(btnClose);
    
    mainLayout->addLayout(btnLayout);

    connect(btnRunAgain, &QPushButton::clicked, this, &DiagnosisDialog::onRunAgain);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);

    loadReport();
}

void DiagnosisDialog::onRunAgain() {
    reportLabel->setText("<span style='color: #888;'>Running diagnosis...</span>");
    loadReport();
}

void DiagnosisDialog::loadReport() {
    DiagnosisReport report = service.runFullCheck(kiosk);
    QString htmlReport = "<pre style=\"font-family: Consolas; font-size: 10pt; color: #d4d4d4;\">" + QString::fromStdString(report.fullReport) + "</pre>";
    reportLabel->setText(htmlReport);
}
