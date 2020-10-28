#ifndef ENTITY_VIEW_H_
#define ENTITY_VIEW_H_


#include "qgraphicsview.h"
#include "qpoint.h"

class EntityView : public QGraphicsView
{
    Q_OBJECT
public:
    EntityView(QGraphicsScene* scene, QWidget* parent = nullptr);
    ~EntityView();
protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private slots:
    void ScalingTime(qreal x);
    void AnimFinished();

private:
    int m_scheduled_scalings;
};


#endif // !ENTITY_VIEW_H_
