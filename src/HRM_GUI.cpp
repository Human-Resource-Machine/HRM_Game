#include "HRM_GUI.h"

HRM_GUI::HRM_GUI(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_HRM_GUI)
{
    ui->setupUi(this);
}

HRM_GUI::~HRM_GUI()
{
    delete ui; 
}