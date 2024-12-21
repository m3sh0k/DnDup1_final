#include "form.h"
#include "ui_form.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPixmap>
#include <QMouseEvent>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    imagePath("")  // Инициализируем путь к изображению пустым значением
{
    ui->setupUi(this);

    // Подключаем сигнал dataChanged к нужным виджетам
    connectWidgets();
}

Form::~Form()
{
    delete ui;
}

void Form::connectWidgets()
{
    connect(ui->lineEdit_name, &QLineEdit::textChanged, this, &Form::dataChanged);
    connect(ui->lineEdit_race, &QLineEdit::textChanged, this, &Form::dataChanged);
    connect(ui->lineEdit_Size, &QLineEdit::textChanged, this, &Form::dataChanged);
    connect(ui->spinBox_health, QOverload<int>::of(&QSpinBox::valueChanged), this, &Form::dataChanged);
    connect(ui->spinBox_mastership_bonus, QOverload<int>::of(&QSpinBox::valueChanged), this, &Form::dataChanged);
    connect(ui->lineEdit_worldview, &QLineEdit::textChanged, this, &Form::dataChanged);
    connect(ui->lineEdit_class, &QLineEdit::textChanged, this, &Form::dataChanged);
    connect(ui->comboBox_The_main_characteristic, &QComboBox::currentTextChanged, this, &Form::dataChanged);
}

void Form::mousePressEvent(QMouseEvent *event)
{
    // Проверяем, был ли клик по label_icon
    if (ui->label_icon->rect().contains(event->pos())) {
        onImageClicked();  // Если да, то вызываем функцию для выбора изображения
    }
    QWidget::mousePressEvent(event);  // Важно вызвать базовый класс для нормальной работы других событий
}

void Form::onImageClicked()
{
    // Открываем диалоговое окно для выбора изображения
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выберите изображение"), "", tr("Images (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);  // Загружаем изображение
        if (!pixmap.isNull()) {
            ui->label_icon->setPixmap(pixmap.scaled(ui->label_icon->size(), Qt::KeepAspectRatio));  // Устанавливаем изображение в QLabel
            imagePath = fileName;  // Сохраняем путь к изображению
        }
    }
}

void Form::saveToJsonFile(const QString &filePath)
{
    QJsonObject jsonObject;

    // Заполняем JSON объект данными из виджетов
    jsonObject["name"] = ui->lineEdit_name->text();
    jsonObject["race"] = ui->lineEdit_race->text();
    jsonObject["size"] = ui->lineEdit_Size->text();
    jsonObject["health"] = ui->spinBox_health->value();
    jsonObject["mastership_bonus"] = ui->spinBox_mastership_bonus->value();
    jsonObject["worldview"] = ui->lineEdit_worldview->text();
    jsonObject["class"] = ui->lineEdit_class->text();
    jsonObject["main_characteristic"] = ui->comboBox_The_main_characteristic->currentText();

    // Сохраняем путь к изображению, если оно было выбрано
    if (!imagePath.isEmpty()) {
        jsonObject["imagePath"] = imagePath;
    }

    QJsonDocument jsonDoc(jsonObject);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл для записи:" << filePath;
        return;
    }

    file.write(jsonDoc.toJson());
    file.close();
}

void Form::loadFromJsonFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл:" << filePath;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qWarning() << "Файл не содержит валидный JSON объект:" << filePath;
        return;
    }

    QJsonObject jsonObject = doc.object();

    // Заполняем виджеты данными
    ui->lineEdit_name->setText(jsonObject["name"].toString());
    ui->lineEdit_race->setText(jsonObject["race"].toString());
    ui->lineEdit_Size->setText(jsonObject["size"].toString());
    ui->spinBox_health->setValue(jsonObject["health"].toInt());
    ui->spinBox_mastership_bonus->setValue(jsonObject["mastership_bonus"].toInt());
    ui->lineEdit_worldview->setText(jsonObject["worldview"].toString());
    ui->lineEdit_class->setText(jsonObject["class"].toString());
    ui->comboBox_The_main_characteristic->setCurrentText(jsonObject["main_characteristic"].toString());

    // Загружаем изображение, если путь существует в JSON
    QString imagePath = jsonObject["imagePath"].toString();
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            ui->label_icon->setPixmap(pixmap.scaled(ui->label_icon->size(), Qt::KeepAspectRatio));  // Устанавливаем изображение в QLabel
        }
    }
}
