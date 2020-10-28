#ifndef ARC_ENTITY_H_
#define ARC_ENTITY_H_


#include "entity.h"

class QGraphicsPathItem;

class ArcEntity : public Entity {
    Q_OBJECT
public:
    static QString kCX;
    static QString kCY;
    static QString kStartAngle;
    static QString kEndAngle;
    static QString kInnerRadius;
    static QString kOuterRadius;
    static QString kIXS;
    static QString kIYS;
    static QString kIXE;
    static QString kIYE;
    static QString kOXS;
    static QString kOYS;
    static QString kOXE;
    static QString kOYE;
    static QString kPointOrder;


    ArcEntity(double cx, double cy, double radius, double start, double end);
    ~ArcEntity();

    static void ResetCounter() { m_counter = 0; }

    QGraphicsItem* GraphicsItem() override;
    QGraphicsItem* RegionGraphicsItem() override;

protected:
    bool Set(const QString& key, const QVariant& value) override;

private:
    void UpdateRegionItem() override;

    QGraphicsPathItem* m_arc;
    QGraphicsPathItem* m_region;

    QPointF m_center;
    double m_radius;
    double m_start;
    double m_end;

    static int m_counter;
};
#endif // !ARC_ENTITY_H_
