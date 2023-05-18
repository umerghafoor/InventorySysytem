#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_InventorySysytemGUI.h"

#include "customerview.h"
#include "adminmenu.h"

#include "Class/customer.h"
#include "Class/admin.h"
#include "Class/venderor.h"

class SignInMenu : public QMainWindow
{
    Q_OBJECT

public:
    SignInMenu(QWidget *parent = nullptr);
    ~SignInMenu();

private slots:
    void on_logIn_button_clicked();
    void onChildWindowClosed();

private:
    Ui::SignInMenu ui;
    Customer *customerPtr;
    Admin *adminPtr;
    user *userPtr;
    Vendor *vendorPtr;
    CustomerView *customerMenu;
    AdminMenu *adminMenu;

};


