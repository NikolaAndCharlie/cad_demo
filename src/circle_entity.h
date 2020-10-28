#ifndef CIRCLE_ENTITY_H_
#define CIRCLE_ENTITY_H_




#include "entity.h"

class QGraphicsEllipseItem;
class QGraphicsPathItem;

class CircleEntity : public Entity {
    Q_OBJECT
public:
    static QString kCX;
    static QString kCY;
    static QString kInnerRadius;
    static QString kOuterRadius;

    CircleEntity(double cx, double cy, double radius);
    ~CircleEntity();

    static void ResetCounter() { m_counter = 0; }

    QGraphicsItem* GraphicsItem() override;
    QGraphicsItem* RegionGraphicsItem() override;


protected:
    bool Set(const QString& key, const QVariant& value) override;

private:
    void UpdateRegionItem() override;

    QGraphicsEllipseItem* m_circle;
    QGraphicsPathItem* m_region;

    static int m_counter;
};

#endif // !CIRCLE_ENTITY_H_