#include "main_window.h"
#include "dxf_importer.h"
#include "entity.h"
#include "QGraphicsScene"
#include "qlayout.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "qpushbutton.h"
#include "qlineedit.h"
#include "qdir.h"
#include "qapplication.h"
#include "qfileinfo.h"
#include "qfiledialog.h"
#include "qgraphicsitem.h"


static void SetGraphicsItemPen(QGraphicsItem* item, const QColor& color, int width)
{
    QPen pen;
    switch (item->type()) {
    case QGraphicsLineItem::Type:
    {
        QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item);
        pen = line->pen();
        pen.setColor(color);
        pen.setWidth(width);
        line->setPen(pen);
    }
    break;
    case QGraphicsEllipseItem::Type:
    case QGraphicsPathItem::Type:
    {
        QAbstractGraphicsShapeItem* shape = dynamic_cast<QAbstractGraphicsShapeItem*>(item);
        pen = shape->pen();
        pen.setColor(color);
        pen.setWidth(width);
        shape->setPen(pen);
    }
    break;
    default:
        break;
    }
}

MainWidget::MainWidget()
{
    CreateUi();
}

MainWidget::~MainWidget()
{
}

void MainWidget::CreateUi()
{
    m_view = new EntityView(new QGraphicsScene);
    m_view->setBackgroundBrush(QBrush(qRgb(33, 40, 48)));
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    connect(m_view->scene(), &QGraphicsScene::selectionChanged, this, &MainWidget::ItemSelectionChanged);
    
    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addWidget(m_view, 9);

    m_dxf_file_path_line_eidt = new QLineEdit();
    QPushButton* load_button = new QPushButton("...");
    connect(load_button, &QPushButton::clicked, [&]
    {
        QString file_name = QFileDialog::getOpenFileName(this, tr("dxf file"), QString(), "dxf (*.dxf)");
        QFileInfo file(file_name);
        if (file.exists()) {
            m_dxf_file_path_line_eidt->setText(file_name);
        }
    });
    QPushButton* set_button = new QPushButton("show");
    connect(set_button, SIGNAL(clicked()), this, SLOT(LoadDxf()));
    QHBoxLayout* line_button_layout = new QHBoxLayout();
    line_button_layout->addWidget(m_dxf_file_path_line_eidt, 6);
    line_button_layout->addWidget(load_button, 2);
    line_button_layout->addWidget(set_button, 2);

    main_layout->addLayout(line_button_layout, 1);
    this->setLayout(main_layout);
}

void MainWidget::LoadDxf()
{
    QString dxf_file = m_dxf_file_path_line_eidt->text();
    if (dxf_file.isEmpty()) {
        return;
    }

    DxfImport box_reader(1);
    if (!box_reader.Read(dxf_file)) {
        return ;
    }
    QRectF box;
    for (int i = 0; i < box_reader.items().size(); ++i) {
        auto item = box_reader.items()[i];
        box = box.united(item->GraphicsItem()->boundingRect());
    }

    QPointF offset = QPointF(10, 10) - box.topLeft();
    DxfImport reader(1);
    if (!reader.Read(dxf_file)) {
        return;
    }

    SetItems(reader.items());
    SetLayers(reader.layers());
}

void MainWidget::SetItems(const QList<std::shared_ptr<Entity>>& items)
{
    m_selected_items.clear();
    m_view->scene()->clear();
    
    for (auto item : items) {
        item->Set(Entity::kRegionWidth, 5);
        item->Set(Entity::kRegionLengthRatio, 0.8);
        m_view->scene()->addItem(item->GraphicsItem());
      //  m_view->scene()->addItem(item->RegionGraphicsItem());
    }

    m_view->scene()->setSceneRect(m_view->scene()->itemsBoundingRect().marginsAdded(QMarginsF(20, 20, 20, 20)));
    m_view->fitInView(m_view->sceneRect(), Qt::KeepAspectRatio);

}

void MainWidget::SetLayers(const QStringList& layers)
{

}

void MainWidget::ItemSelectionChanged()
{
    for (auto item : m_selected_items) {
        SetGraphicsItemPen(item, Qt::white, 1);
    }

    m_selected_items = m_view->scene()->selectedItems();

    for (auto item : m_selected_items) {
        SetGraphicsItemPen(item, Qt::red, 2);
    }

}
