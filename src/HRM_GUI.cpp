#include "HRM_GUI.h"
#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <QIcon>
#include <QHBoxLayout>

HRM_GUI::HRM_GUI(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_HRM_GUI)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("人类资源机器");
    QIcon icon("://images/icon.png");
    setWindowIcon(icon);

    // 创建一个 QLabel 控件
    QLabel *imageLabel = new QLabel(this);

    // 加载要显示的图片
    QPixmap image("://images/home.png"); // 替换为您的图片路径

    // 将图片设置到 QLabel 中
    imageLabel->setPixmap(image);

    // 调整 QLabel 的大小以适应图片大小
    imageLabel->setScaledContents(true);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    setCentralWidget(imageLabel);
}

HRM_GUI::~HRM_GUI()
{
    delete ui; 
}