#include "circle_entity.h"

#include "qmath.h"

#include "qpen.h"
#include "qbrush.h"

#include "qgraphicsitem.h"
#include "qgraphicsview.h"


QString CircleEntity::kCX = "CX";
QString CircleEntity::kCY = "CY";
QString CircleEntity::kInnerRadius = "InnerRadius";
QString CircleEntity::kOuterRadius = "OuterRadius";

int CircleEntity::m_counter = 0;

CircleEntity::CircleEntity(double cx, double cy, double radius) {
    m_type = Type::kCircleEntity;
    m_name = tr("CIR") + QString::number(m_counter++);
    m_tool_name = "FindCircleTool";

    m_circle = new QGraphicsEllipseItem(cx - radius, cy - radius, 2 * radius, 2 * radius);
    QPen pen(m_properties[Entity::kItemPenColor].value<QColor>(), 1.0);
    pen.setCosmetic(true);
    m_circle->setPen(pen);
    m_circle->setFlags(QGraphicsItem::ItemIsSelectable);

    m_shape[kCX] = 0.0;
    m_shape[kCY] = 0.0;
    m_shape[kInnerRadius] = 0.0;
    m_shape[kOuterRadius] = 0.0;

    m_region = new QGraphicsPathItem;
    QPen region_pen(Get(Entity::kRegionPenColor).value<QColor>(), 1.0);
    region_pen.setCosmetic(true);
    m_region->setPen(region_pen);
    m_region->setBrush(QBrush(Get(Entity::kRegionBrushColor).value<QColor>()));
    m_region->setOpacity(Get(Entity::kRegionOpacity).toDouble());

    UpdateRegionItem();
}

CircleEntity::~CircleEntity() {
}


QGraphicsItem* CircleEntity::GraphicsItem() {
    return m_circle;
}

QGraphicsItem* CircleEntity::RegionGraphicsItem() {
    return m_region;
}


bool CircleEntity::Set(const QString& key, const QVariant& value) {
    bool result = Entity::Set(key, value);
    if (result && key == Entity::kRegionWidth) {
        UpdateRegionItem();
    }

    return result;
}


void CircleEntity::UpdateRegionItem() {
    QRectF rect = m_circle->rect();
    QPointF center = rect.center();
    double radius = rect.width() / 2.0;
    double region_width = Get(kRegionWidth).toDouble();
    double inner = radius - region_width / 2.0;
    double outer = radius + region_width / 2.0;

    SetShape(kCX, center.x());
    SetShape(kCY, center.y());
    SetShape(kInnerRadius, inner);
    SetShape(kOuterRadius, outer);


    QPainterPath path;
    path.addEllipse(center, outer, outer);
    path.addEllipse(center, inner, inner);
    m_region->setPath(path);
}


