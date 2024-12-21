#include "resizehandle.h"
#include <QBrush>

ResizeHandle::ResizeHandle(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setRect(-5, -5, 10, 10);  // Размер ручки
    setBrush(Qt::blue);       // Цвет ручки
}

void ResizeHandle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    dragStartPos = event->pos();
    QGraphicsRectItem::mousePressEvent(event);
}

void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF offset = event->pos() - dragStartPos;
        setPos(pos() + offset);  // Перемещаем ручку

        // Изменяем размеры родительского элемента, если он является QGraphicsRectItem
        if (auto parent = dynamic_cast<QGraphicsRectItem *>(parentItem())) {
            QRectF newRect = parent->rect();
            newRect.setWidth(newRect.width() + offset.x());
            newRect.setHeight(newRect.height() + offset.y());
            parent->setRect(newRect);
        }
        dragStartPos = event->pos();
    }
    QGraphicsRectItem::mouseMoveEvent(event);
}
