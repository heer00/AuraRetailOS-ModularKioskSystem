/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QHBoxLayout *buyLayout;
    QLabel *productLabel;
    QLineEdit *productInput;
    QPushButton *buyButton;
    QHBoxLayout *refundLayout;
    QLabel *transactionLabel;
    QLineEdit *transactionInput;
    QPushButton *refundButton;
    QTextEdit *logBox;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(444, 148);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        buyLayout = new QHBoxLayout();
        buyLayout->setObjectName("buyLayout");
        productLabel = new QLabel(centralwidget);
        productLabel->setObjectName("productLabel");

        buyLayout->addWidget(productLabel);

        productInput = new QLineEdit(centralwidget);
        productInput->setObjectName("productInput");

        buyLayout->addWidget(productInput);

        buyButton = new QPushButton(centralwidget);
        buyButton->setObjectName("buyButton");

        buyLayout->addWidget(buyButton);


        verticalLayout->addLayout(buyLayout);

        refundLayout = new QHBoxLayout();
        refundLayout->setObjectName("refundLayout");
        transactionLabel = new QLabel(centralwidget);
        transactionLabel->setObjectName("transactionLabel");

        refundLayout->addWidget(transactionLabel);

        transactionInput = new QLineEdit(centralwidget);
        transactionInput->setObjectName("transactionInput");

        refundLayout->addWidget(transactionInput);

        refundButton = new QPushButton(centralwidget);
        refundButton->setObjectName("refundButton");

        refundLayout->addWidget(refundButton);


        verticalLayout->addLayout(refundLayout);

        logBox = new QTextEdit(centralwidget);
        logBox->setObjectName("logBox");
        logBox->setReadOnly(true);

        verticalLayout->addWidget(logBox);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Aura Retail OS - Kiosk", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Aura Kiosk System", nullptr));
        productLabel->setText(QCoreApplication::translate("MainWindow", "Product ID:", nullptr));
        productInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter product ID to buy", nullptr));
        buyButton->setText(QCoreApplication::translate("MainWindow", "Buy Item", nullptr));
        transactionLabel->setText(QCoreApplication::translate("MainWindow", "Transaction ID:", nullptr));
        transactionInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter transaction ID to refund", nullptr));
        refundButton->setText(QCoreApplication::translate("MainWindow", "Refund", nullptr));
        logBox->setPlaceholderText(QCoreApplication::translate("MainWindow", "System logs will appear here...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
