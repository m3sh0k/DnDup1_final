#ifndef MAPVIEWWIDGET_H
#define MAPVIEWWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QMenu>
#include <QAction>

namespace Ui {
class mapviewwidget;
}

class mapviewwidget : public QWidget
{
    Q_OBJECT

public:
    explicit mapviewwidget(QWidget *parent = nullptr);
    ~mapviewwidget();

    // Загрузка карты из файла
    void loadMap(const QString &filePath);
    void createContextMenu(QPoint position);
    //bool eventFilter(QObject *object, QEvent *event);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    Ui::mapviewwidget *ui;

    QGraphicsScene *scene; // Сцена для отображения
    double currentScale;   // Текущий масштаб

    // Вспомогательные методы
    void setupGraphicsView();            // Настройка GraphicsView
    void handleDrag(QMouseEvent *event); // Перетаскивание карты
    void handleZoom(QWheelEvent *event); // Масштабирование карты
};

#endif // MAPVIEWWIDGET_H
