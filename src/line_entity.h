#ifndef LINE_ENTITY_H_
#define LINE_ENTITY_H_

#include "entity.h"

class QGraphicsLineItem;
class QGraphicsPolygonItem;

class LineEntity : public Entity
{
    Q_OBJECT
public:
    static QString kX0;
    static QString kY0;
    static QString kX1;
    static QString kY1;
    static QString kX2;
    static QString kY2;
    static QString kX3;
    static QString kY3;
    static QString kWidth;
    static QString kHeight;
    static QString kTheta;

    LineEntity(double ro, double co, double r1, double c1);
    ~LineEntity();

    static void ResetCounter() {m_counter = 0;}

    QGraphicsItem* GraphicsItem() override;
    QGraphicsItem* RegionGraphicsItem() override;

protected:
    bool Set(const QString& , const QVariant&) override;
private:
    void UpdateRegionItem() override;

    QGraphicsLineItem* m_line;
    QGraphicsPolygonItem* m_region;

    static int m_counter;
};


#endif // !LINE_ENTITY_H_
