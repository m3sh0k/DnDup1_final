#include "mapviewwidget.h" // Подключаем заголовочный файл, который определяет класс mapviewwidget
#include "ui_mapviewwidget.h" // Подключаем файл, который содержит автоматически сгенерированный код для интерфейса

// Конструктор класса mapviewwidget
mapviewwidget::mapviewwidget(QWidget *parent)
    : QWidget(parent), // Инициализируем базовый класс QWidget с передачей родительского виджета
    ui(new Ui::mapviewwidget), // Создаем объект интерфейса
    scene(new QGraphicsScene(this)), // Создаем сцену для отображения графики
    currentScale(1.0) // Устанавливаем начальный масштаб (100%)
{
    ui->setupUi(this); // Инициализируем элементы интерфейса

    setupGraphicsView(); // Настраиваем виджет для отображения графики
    ui->graphicsView->viewport()->installEventFilter(this); // Устанавливаем фильтр событий для viewport графического вида
}

// Деструктор класса mapviewwidget
mapviewwidget::~mapviewwidget()
{
    delete ui; // Освобождаем память, занятую объектом интерфейса
}

// Метод для настройки QGraphicsView
void mapviewwidget::setupGraphicsView()
{
    ui->graphicsView->setScene(scene); // Устанавливаем сцену для отображения в графическом виде
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем горизонтальную полосу прокрутки
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем вертикальную полосу прокрутки
    ui->graphicsView->setRenderHint(QPainter::Antialiasing); // Включаем сглаживание для отрисовки
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag); // Отключаем режим перетаскивания
}

// Метод для загрузки карты
void mapviewwidget::loadMap(const QString &mapPath)
{
    QPixmap pixmap(mapPath); // Загружаем изображение карты из указанного пути
    if (pixmap.isNull()) { // Проверяем, удалось ли загрузить изображение
        qWarning() << "Failed to load map:" << mapPath; // Выводим предупреждение в случае ошибки
        return; // Завершаем выполнение метода
    }

    scene->clear(); // Очищаем текущую сцену
    scene->addPixmap(pixmap); // Добавляем загруженное изображение на сцену
    ui->graphicsView->setScene(scene); // Назначаем сцену графическому виду
}

/*
// Метод для обработки фильтра событий
bool mapviewwidget::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->graphicsView->viewport()) { // Проверяем, что событие относится к viewport
        switch (event->type()) { // Определяем тип события
        case QEvent::MouseButtonPress: // Обработка нажатия мыши
        case QEvent::MouseButtonRelease: // Обработка отпускания мыши
            handleDrag(static_cast<QMouseEvent *>(event)); // Передаем событие в обработчик перетаскивания
            break;
        case QEvent::Wheel: // Обработка прокрутки колеса мыши
            handleZoom(static_cast<QWheelEvent *>(event)); // Передаем событие в обработчик масштабирования
            return true; // Блокируем стандартное поведение прокрутки колеса
        default:
            break; // Игнорируем другие типы событий
        }
    }
    return QWidget::eventFilter(object, event); // Вызываем стандартный обработчик событий
}
*/

// Метод для обработки событий перетаскивания
void mapviewwidget::handleDrag(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) { // Проверяем, была ли нажата левая кнопка мыши
        ui->graphicsView->setDragMode(
            event->type() == QEvent::MouseButtonPress
                ? QGraphicsView::ScrollHandDrag // Включаем режим перетаскивания при нажатии кнопки
                : QGraphicsView::NoDrag); // Выключаем режим перетаскивания при отпускании кнопки
    }
}

// Метод для обработки событий масштабирования
void mapviewwidget::handleZoom(QWheelEvent *event)
{
    const double scaleFactor = 1.15; // Коэффициент масштабирования
    if (event->angleDelta().y() > 0) { // Если колесо прокручено вперед (увеличение)
        ui->graphicsView->scale(scaleFactor, scaleFactor); // Увеличиваем масштаб
    } else { // Если колесо прокручено назад (уменьшение)
        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Уменьшаем масштаб
    }
}

// Метод для создания контекстного меню при нажатии правой кнопкой мыши
void mapviewwidget::createContextMenu(QPoint position)
{
    QMenu contextMenu; // Создаем объект контекстного меню

    // Создаем действия (кнопки) для контекстного меню
    QAction *zoomInAction = new QAction("Zoom In", this); // Действие для увеличения масштаба
    QAction *zoomOutAction = new QAction("Zoom Out", this); // Действие для уменьшения масштаба
    QAction *resetViewAction = new QAction("Reset View", this); // Действие для сброса вида

    // Добавляем действия в контекстное меню
    contextMenu.addAction(zoomInAction); // Добавляем действие для увеличения масштаба
    contextMenu.addAction(zoomOutAction); // Добавляем действие для уменьшения масштаба
    contextMenu.addAction(resetViewAction); // Добавляем действие для сброса вида

    // Подключаем действия к соответствующим методам
    connect(zoomInAction, &QAction::triggered, this, [this]() {
        ui->graphicsView->scale(1.15, 1.15); // Увеличиваем масштаб
    });

    connect(zoomOutAction, &QAction::triggered, this, [this]() {
        ui->graphicsView->scale(1.0 / 1.15, 1.0 / 1.15); // Уменьшаем масштаб
    });

    connect(resetViewAction, &QAction::triggered, this, [this]() {
        ui->graphicsView->resetTransform(); // Сбрасываем трансформации вида
    });

    // Показываем контекстное меню в позиции щелчка правой кнопки мыши
    contextMenu.exec(ui->graphicsView->mapToGlobal(position));
}

// Дополняем метод eventFilter для обработки щелчка правой кнопкой мыши
bool mapviewwidget::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->graphicsView->viewport()) { // Проверяем, что событие относится к viewport
        switch (event->type()) { // Определяем тип события
        case QEvent::MouseButtonPress: { // Обработка нажатия мыши
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // Приводим событие к типу QMouseEvent
            if (mouseEvent->button() == Qt::RightButton) { // Проверяем, что нажата правая кнопка мыши
                createContextMenu(mouseEvent->pos()); // Вызываем метод для отображения контекстного меню // Вызываем обработчик для нажатия правой кнопки мыши
                return true; // Блокируем дальнейшую обработку события
            }
            handleDrag(mouseEvent); // Передаем событие в обработчик перетаскивания для других кнопок
            break;
        }
        case QEvent::MouseButtonRelease: { // Обработка отпускания мыши
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // Приводим событие к типу QMouseEvent
            handleDrag(mouseEvent); // Передаем событие в обработчик перетаскивания
            break;
        }
        case QEvent::Wheel: // Обработка прокрутки колеса мыши
            handleZoom(static_cast<QWheelEvent *>(event)); // Передаем событие в обработчик масштабирования
            return true; // Блокируем стандартное поведение прокрутки колеса
        default:
            break; // Игнорируем другие типы событий
        }
    }
    return QWidget::eventFilter(object, event); // Вызываем стандартный обработчик событий
}
