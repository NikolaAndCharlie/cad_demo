#include "main_window.h"
#include "QGraphicsScene"
#include "qlayout.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "qlineedit.h"
MainWidget::MainWidget()
{
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
    main_layout->addWidget(m_view);

    m_dxf_file_path_line_eidt = new QLineEdit();
}

