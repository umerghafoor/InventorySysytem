#include "InventorySysytemGUI.h"
#include <QMessageBox>

InventorySysytemGUI::InventorySysytemGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

InventorySysytemGUI::~InventorySysytemGUI()
{}

void InventorySysytemGUI::on_logIn_button_clicked()
{
    try 
    {
        std::stoi(ui.ID->text().toStdString());
    }
    catch (std::exception& e) 
    {
        QMessageBox::warning(this, "Login Error", "ID must be Integer");
        return;
    }

    std::string userID = ui.ID->text().toStdString();
    std::string password = ui.password->text().toStdString();

    if(ui.userType->currentIndex() == 1)
        userPtr = new Customer(stoi(userID), password);
    else if(ui.userType->currentIndex() == 0)
        userPtr = new Admin(stoi(userID), password);

    if (userPtr->getID() == -1)
    {
        QMessageBox ::warning(this, "Login Error", "Invalid User ID or Password");
        return;
    }
    else if(ui.userType->currentIndex() == 1)
    {
        this->hide();
        customerPtr = new Customer(stoi(userID), password);
        customerMenu=new CustomerView(this,customerPtr);
        customerMenu->show();
    }
    else if(ui.userType->currentIndex() == 0)
    {
        this->hide();
        adminPtr = new Admin(stoi(userID),password);
        adminMenu=new AdminMenu(this,adminPtr);
        adminMenu->show();
    }
    else
    {
        QMessageBox ::warning(this, "Login Type Error", "Please select the valid type");
    }
}

