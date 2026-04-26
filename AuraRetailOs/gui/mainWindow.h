#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QString>
#include "core/Kiosk.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRoleCustomerClicked();
    void onRoleAdminClicked();
    void onBuyClicked();
    void onRefundClicked();
    void onRestockClicked();
    void onBackToMainClicked();

private:
    Ui::MainWindow *ui;

    Kiosk* kiosk;
    QString currentUserId;
    
    // UI Elements managed programmatically
    QStackedWidget* mainStack;
    
    // Role Menu
    QWidget* roleMenuWidget;
    QComboBox* kioskTypeDropdown;
    QLineEdit* userIdInput;
    QPushButton* btnCustomer;
    QPushButton* btnAdmin;

    // Customer View
    QWidget* customerWidget;
    QComboBox* productDropdown;
    QPushButton* btnBuy;
    QLineEdit* transactionInput;
    QLineEdit* refundProductIdInput;
    QPushButton* btnRefund;
    QPushButton* btnCustBack;
    QTextEdit* custLogBox;

    // Admin View
    QWidget* adminWidget;
    QLineEdit* restockProductIdInput;
    QSpinBox* restockQtyInput;
    QPushButton* btnRestock;
    QPushButton* btnAdminBack;
    QTextEdit* adminLogBox;
    
    void setupProgrammaticUI();
};

#endif