#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QInputDialog>

// include your builder
#include "core/KioskBuilder.h"
#include "factory/KioskFactorySimple.h"
#include "factory/KioskFactory.h"
#include "payment/WalletAdapter.h"
#include "payment/UPIAdapter.h"
#include "payment/CardAdapter.h"
#include "inventory/Inventory.h"
#include "command/PurchaseItemCommand.h"
#include "command/RefundCommand.h"
#include "command/RestockCommand.h"
#include "persistence/TransactionLog.h"
#include "persistence/ConfigStore.h"
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // Load configuration to avoid stod parsing crashes
    ConfigStore config("data/config.json");
    config.load();

    kiosk = nullptr;
    
    // We are discarding the .ui layout to implement the new UI programmatically
    if (this->centralWidget() && this->centralWidget()->layout()) {
        QWidget* emptyWidget = new QWidget(this);
        this->setCentralWidget(emptyWidget);
    }
    
    setupProgrammaticUI();
}

MainWindow::~MainWindow() {
    delete ui;
    if (kiosk) {
        delete kiosk;
    }
}

void MainWindow::setupProgrammaticUI() {
    mainStack = new QStackedWidget(this);
    setCentralWidget(mainStack);

    QFont titleFont("Arial", 16, QFont::Bold);
    QFont logFont("Consolas", 10);
    logFont.setStyleHint(QFont::Monospace);

    // ==========================================
    // Page 0: Role Selection Menu
    // ==========================================
    roleMenuWidget = new QWidget();
    QVBoxLayout* roleLayout = new QVBoxLayout(roleMenuWidget);
    
    QLabel* titleLabel = new QLabel("Aura Kiosk System - Login");
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* idLabel = new QLabel("Enter ID (Cust/Admin):");
    userIdInput = new QLineEdit();
    userIdInput->setPlaceholderText("Enter your ID here...");

    QLabel* typeLabel = new QLabel("Select Kiosk Type:");
    kioskTypeDropdown = new QComboBox();
    kioskTypeDropdown->addItems({"food", "pharmacy", "emergency"});

    btnCustomer = new QPushButton("Login as Customer");
    btnAdmin = new QPushButton("Login as Admin");

    roleLayout->addWidget(titleLabel);
    roleLayout->addSpacing(20);
    roleLayout->addWidget(typeLabel);
    roleLayout->addWidget(kioskTypeDropdown);
    roleLayout->addSpacing(10);
    roleLayout->addWidget(idLabel);
    roleLayout->addWidget(userIdInput);
    roleLayout->addSpacing(20);
    roleLayout->addWidget(btnCustomer);
    roleLayout->addWidget(btnAdmin);
    roleLayout->addStretch();

    // ==========================================
    // Page 1: Customer View
    // ==========================================
    customerWidget = new QWidget();
    QVBoxLayout* custLayout = new QVBoxLayout(customerWidget);

    QLabel* custTitle = new QLabel("Customer Portal");
    custTitle->setFont(titleFont);
    custTitle->setAlignment(Qt::AlignCenter);

    // Buy section
    QHBoxLayout* buyLayout = new QHBoxLayout();
    productDropdown = new QComboBox();
    
    // Populated dynamically on login
    
    btnBuy = new QPushButton("Buy Item");
    buyLayout->addWidget(new QLabel("Select Product:"));
    buyLayout->addWidget(productDropdown);
    buyLayout->addWidget(btnBuy);

    // Refund section
    QHBoxLayout* refundLayout = new QHBoxLayout();
    transactionInput = new QLineEdit();
    transactionInput->setPlaceholderText("Txn ID");
    refundProductIdInput = new QLineEdit();
    refundProductIdInput->setPlaceholderText("Product ID");
    btnRefund = new QPushButton("Refund");
    refundLayout->addWidget(new QLabel("Refund:"));
    refundLayout->addWidget(transactionInput);
    refundLayout->addWidget(refundProductIdInput);
    refundLayout->addWidget(btnRefund);

    custLogBox = new QTextEdit();
    custLogBox->setReadOnly(true);
    custLogBox->setFont(logFont);
    custLogBox->setStyleSheet("QTextEdit { background-color: #1e1e1e; color: #d4d4d4; }");

    btnCustBack = new QPushButton("Logout (Back to Main Menu)");

    custLayout->addWidget(custTitle);
    custLayout->addLayout(buyLayout);
    custLayout->addLayout(refundLayout);
    custLayout->addWidget(custLogBox);
    custLayout->addWidget(btnCustBack);

    // ==========================================
    // Page 2: Admin View
    // ==========================================
    adminWidget = new QWidget();
    QVBoxLayout* adminLayout = new QVBoxLayout(adminWidget);

    QLabel* adminTitle = new QLabel("Admin Portal");
    adminTitle->setFont(titleFont);
    adminTitle->setAlignment(Qt::AlignCenter);

    // Restock section
    QHBoxLayout* restockLayout = new QHBoxLayout();
    restockProductIdInput = new QLineEdit();
    restockProductIdInput->setPlaceholderText("Product ID");
    restockQtyInput = new QSpinBox();
    restockQtyInput->setRange(1, 1000);
    btnRestock = new QPushButton("Restock Item");

    restockLayout->addWidget(new QLabel("Product ID:"));
    restockLayout->addWidget(restockProductIdInput);
    restockLayout->addWidget(new QLabel("Qty:"));
    restockLayout->addWidget(restockQtyInput);
    restockLayout->addWidget(btnRestock);

    adminLogBox = new QTextEdit();
    adminLogBox->setReadOnly(true);
    adminLogBox->setFont(logFont);
    adminLogBox->setStyleSheet("QTextEdit { background-color: #1e1e1e; color: #d4d4d4; }");

    btnAdminBack = new QPushButton("Logout (Back to Main Menu)");

    adminLayout->addWidget(adminTitle);
    adminLayout->addLayout(restockLayout);
    adminLayout->addWidget(adminLogBox);
    adminLayout->addWidget(btnAdminBack);

    // ==========================================
    // Add pages to StackedWidget
    // ==========================================
    mainStack->addWidget(roleMenuWidget); // Index 0
    mainStack->addWidget(customerWidget); // Index 1
    mainStack->addWidget(adminWidget);    // Index 2

    // Connect signals
    connect(btnCustomer, &QPushButton::clicked, this, &MainWindow::onRoleCustomerClicked);
    connect(btnAdmin, &QPushButton::clicked, this, &MainWindow::onRoleAdminClicked);
    connect(btnBuy, &QPushButton::clicked, this, &MainWindow::onBuyClicked);
    connect(btnRefund, &QPushButton::clicked, this, &MainWindow::onRefundClicked);
    connect(btnRestock, &QPushButton::clicked, this, &MainWindow::onRestockClicked);
    connect(btnCustBack, &QPushButton::clicked, this, &MainWindow::onBackToMainClicked);
    connect(btnAdminBack, &QPushButton::clicked, this, &MainWindow::onBackToMainClicked);
}

void MainWindow::onRoleCustomerClicked() {
    currentUserId = userIdInput->text().trimmed();
    if (currentUserId.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter your Customer ID.");
        return;
    }
    
    // Dynamically build kiosk based on selection
    if (kiosk) {
        delete kiosk;
    }
    QString type = kioskTypeDropdown->currentText();
    KioskFactory* factory = KioskFactorySimple::createFactory(type.toStdString());
    kiosk = KioskBuilder()
        .addDispenser(factory->createDispenser())
        .addPayment(factory->createPayment())
        .addInventory(factory->createInventory())
        .addPricingPolicy(factory->createPricingPolicy())
        .build();
    delete factory;
    
    // Refresh product list in case it changed
    productDropdown->clear();
    std::vector<std::string> products = kiosk->getInventory()->getAllProductIds();
    for (const auto& p : products) {
        productDropdown->addItem(QString::fromStdString(p));
    }

    mainStack->setCurrentIndex(1); // Go to Customer Page
    custLogBox->append(QString("<span style=\"color:#888888;\">[%1]</span> Logged in as Customer: %2")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"), currentUserId));
}

void MainWindow::onRoleAdminClicked() {
    currentUserId = userIdInput->text().trimmed();
    if (currentUserId.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter your Admin ID.");
        return;
    }

    // Dynamically build kiosk based on selection
    if (kiosk) {
        delete kiosk;
    }
    QString type = kioskTypeDropdown->currentText();
    KioskFactory* factory = KioskFactorySimple::createFactory(type.toStdString());
    kiosk = KioskBuilder()
        .addDispenser(factory->createDispenser())
        .addPayment(factory->createPayment())
        .addInventory(factory->createInventory())
        .addPricingPolicy(factory->createPricingPolicy())
        .build();
    delete factory;

    mainStack->setCurrentIndex(2); // Go to Admin Page
    adminLogBox->append(QString("<span style=\"color:#888888;\">[%1]</span> Logged in as Admin: %2")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"), currentUserId));
}

void MainWindow::onBackToMainClicked() {
    currentUserId.clear();
    userIdInput->clear();
    mainStack->setCurrentIndex(0);
}

void MainWindow::onBuyClicked() {
    QString product = productDropdown->currentText();
    if (product.isEmpty()) return;

    // Ask for payment method
    QStringList paymentMethods;
    paymentMethods << "Wallet" << "UPI" << "Card";
    bool ok;
    QString method = QInputDialog::getItem(this, "Payment Method", 
                                           "Select payment method:", paymentMethods, 0, false, &ok);
    if (!ok || method.isEmpty()) return; // User cancelled

    if (method == "Wallet") {
        kiosk->setPayment(new WalletAdapter(currentUserId.toStdString()));
    } else if (method == "UPI") {
        QString mobile = QInputDialog::getText(this, "UPI Payment", "Enter mobile number for UPI:", QLineEdit::Normal, "", &ok);
        if (!ok || mobile.isEmpty()) return;
        kiosk->setPayment(new UPIAdapter());
    } else if (method == "Card") {
        QString cardLast4 = QInputDialog::getText(this, "Card Payment", "Enter card last 4 digits:", QLineEdit::Normal, "", &ok);
        if (!ok || cardLast4.isEmpty()) return;
        kiosk->setPayment(new CardAdapter());
    }

    PurchaseItemCommand cmd(product.toStdString(),
                            kiosk->getInventory(),
                            kiosk->getPayment(),
                            kiosk->getDispenser(),
                            kiosk->getPricingPolicy());
    cmd.execute();
    
    std::string logResult = cmd.getLog();
    TransactionLog txLog("data/transactions.csv");
    txLog.append(logResult);

    QString timeStr = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString color = (logResult.find("SUCCESS") != std::string::npos) ? "#4caf50" : "#f44336";
    QString status = (logResult.find("SUCCESS") != std::string::npos) ? "SUCCESS" : "FAILED";

    QString logMsg = QString("<span style=\"color:#888888;\">[%1]</span> "
                             "<span style=\"color:%2; font-weight:bold;\">%3</span>: %4")
                         .arg(timeStr, color, status, QString::fromStdString(logResult));

    custLogBox->append(logMsg);
}

void MainWindow::onRefundClicked() {
    QString txn = transactionInput->text().trimmed();
    QString pid = refundProductIdInput->text().trimmed();
    if (txn.isEmpty() || pid.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both Transaction ID and Product ID.");
        return;
    }

    RefundCommand cmd(txn.toStdString(), pid.toStdString(), kiosk->getPayment(), kiosk->getInventory());
    cmd.execute();

    std::string logResult = cmd.getLog();
    TransactionLog txLog("data/transactions.csv");
    txLog.append(logResult);

    QString timeStr = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString color = (logResult.find("REFUNDED") != std::string::npos) ? "#ff9800" : "#f44336";
    QString status = (logResult.find("REFUNDED") != std::string::npos) ? "REFUND" : "FAILED";

    QString logMsg = QString("<span style=\"color:#888888;\">[%1]</span> "
                             "<span style=\"color:%2; font-weight:bold;\">%3</span>: %4")
                         .arg(timeStr, color, status, QString::fromStdString(logResult));

    custLogBox->append(logMsg);
    transactionInput->clear();
    refundProductIdInput->clear();
}

void MainWindow::onRestockClicked() {
    QString product = restockProductIdInput->text().trimmed();
    int qty = restockQtyInput->value();

    if (product.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a product ID.");
        return;
    }

    RestockCommand cmd(product.toStdString(), qty, kiosk->getInventory());
    cmd.execute();

    std::string logResult = cmd.getLog();
    TransactionLog txLog("data/transactions.csv");
    txLog.append(logResult);

    QString timeStr = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString color = (logResult.find("RESTOCK") != std::string::npos) ? "#2196f3" : "#f44336";
    QString status = (logResult.find("RESTOCK") != std::string::npos) ? "RESTOCK" : "FAILED";

    QString logMsg = QString("<span style=\"color:#888888;\">[%1]</span> "
                             "<span style=\"color:%2; font-weight:bold;\">%3</span>: %4")
                         .arg(timeStr, color, status, QString::fromStdString(logResult));

    adminLogBox->append(logMsg);
    restockProductIdInput->clear();
    restockQtyInput->setValue(1);
}