#include "entity_view.h"
#include "qevent.h"
#include "qtimeline.h"
#include "qmenu.h"

EntityView::EntityView(QGraphicsScene* scene, QWidget* parent)
          : QGraphicsView(scene, parent)
{
    m_scheduled_scalings = 0;
    setMouseTracking(true);
}

void EntityView::wheelEvent(QWheelEvent* event)
{
    int degrees = event->delta() / 8;
    int steps = degrees / 15;
    m_scheduled_scalings += steps;

    if (m_scheduled_scalings * steps < 0) {
        m_scheduled_scalings = steps;
    }

    QTimeLine* anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(ScalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(AnimFinished()));
    anim->start();
}

void EntityView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        if (event->modifiers() & Qt::ControlModifier) {
            fitInView(sceneRect(), Qt::KeepAspectRatio);
        }
        event->accept();
    }
    else {
        QGraphicsView::mousePressEvent(event);
    }
}

void EntityView::ScalingTime(qreal x) {
    qreal factor = 1.0 + qreal(m_scheduled_scalings) / 300.0;
    scale(factor, factor);
}
void EntityView::AnimFinished() {
    if (m_scheduled_scalings > 0) {
        m_scheduled_scalings--;
    }
    else {
        m_scheduled_scalings++;
    }
    sender()->~QObject();
}

