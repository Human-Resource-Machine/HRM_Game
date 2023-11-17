#pragma once
#include "ui_HRM_GUI.h"
#include <QMainWindow>

class HRM_GUI : public QMainWindow {
    Q_OBJECT
    
public:
    HRM_GUI(QWidget* parent = nullptr);
    ~HRM_GUI();

private:
    Ui_HRM_GUI* ui;
};