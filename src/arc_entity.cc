#include "arc_entity.h"

#include "qmath.h"

#include "qpen.h"
#include "qbrush.h"

#include "qgraphicsview.h"
#include "qgraphicsitem.h"


static QPainterPath CreateArcPath(double cx, double cy, double radius, double start, double end);

QString ArcEntity::kCX = "CX";
QString ArcEntity::kCY = "CY";
QString ArcEntity::kStartAngle = "StartAngle";
QString ArcEntity::kEndAngle = "EndAngle";
QString ArcEntity::kInnerRadius = "InnerRadius";
QString ArcEntity::kOuterRadius = "OuterRadius";
QString ArcEntity::kIXS = "IXS";
QString ArcEntity::kIYS = "IYS";
QString ArcEntity::kIXE = "IXE";
QString ArcEntity::kIYE = "IYE";
QString ArcEntity::kOXS = "OXS";
QString ArcEntity::kOYS = "OYS";
QString ArcEntity::kOXE = "OXE";
QString ArcEntity::kOYE = "OYE";
QString ArcEntity::kPointOrder = "PointOrder";

int ArcEntity::m_counter = 0;

ArcEntity::ArcEntity(double cx, double cy, double radius, double start, double end)
    : m_center(cx, cy)
    , m_radius(radius)
    , m_start(start)
    , m_end(end) {
    m_type = Type::kArcEntity;
    m_name = tr("ARC") + QString::number(m_counter++);
    m_tool_name = "FindArcTool";

    m_arc = new QGraphicsPathItem(CreateArcPath(cx, cy, radius, start, end));
    QPen pen(m_properties[Entity::kItemPenColor].value<QColor>());
    pen.setCosmetic(true);
    m_arc->setPen(pen);
    m_arc->setFlags(QGraphicsItem::ItemIsSelectable);

    m_shape.insert(kCX, 0.0);
    m_shape.insert(kCY, 0.0);
    m_shape.insert(kStartAngle, 0.0);
    m_shape.insert(kEndAngle, 0.0);
    m_shape.insert(kInnerRadius, 0.0);
    m_shape.insert(kOuterRadius, 0.0);
    m_shape.insert(kIXS, 0.0);
    m_shape.insert(kIYS, 0.0);
    m_shape.insert(kIXE, 0.0);
    m_shape.insert(kIYE, 0.0);
    m_shape.insert(kOXS, 0.0);
    m_shape.insert(kOYS, 0.0);
    m_shape.insert(kOXE, 0.0);
    m_shape.insert(kOYE, 0.0);
    m_shape.insert(kPointOrder, 0.0);

    m_region = new QGraphicsPathItem;
    QPen region_pen(Get(Entity::kRegionPenColor).value<QColor>(), 1.0);
    region_pen.setCosmetic(true);
    m_region->setPen(region_pen);
    m_region->setBrush(QBrush(Get(Entity::kRegionBrushColor).value<QColor>()));
    m_region->setOpacity(Get(Entity::kRegionOpacity).toDouble());
    UpdateRegionItem();
}

ArcEntity::~ArcEntity() {
}

QGraphicsItem* ArcEntity::GraphicsItem() {
    return m_arc;
}

QGraphicsItem* ArcEntity::RegionGraphicsItem() {
    return m_region;
}

bool ArcEntity::Set(const QString& key, const QVariant& value) {
    bool result = Entity::Set(key, value);
    if (result) {
        if (key == kRegionWidth || key == kRegionLengthRatio) {
            UpdateRegionItem();
        }
    }
    return result;
}

double SweepAngle(double start, double end) {
    return end > start ? end - start : 360 - start + end;
}

double NormalizeAngle(double a) {
    while (a > 360) {
        a -= 360;
    }

    while (a < 0) {
        a += 360;
    }

    return a;
}

void ShrinkAngleRange(double& start, double& end, double ratio) {
    double sweep = SweepAngle(start, end);
    double delta = (1 - ratio) * sweep / 2.0;
    start += delta;
    end -= delta;

    start = NormalizeAngle(start);
    end = NormalizeAngle(end);
}

void ArcEntity::UpdateRegionItem() {
    double region_width = Get(kRegionWidth).toDouble();
    double ratio = Get(kRegionLengthRatio).toDouble();
    double radius = m_radius;
    double start = m_start;
    double end = m_end;

    ShrinkAngleRange(start, end, Get(kRegionLengthRatio).toDouble());

    double inner = radius - region_width / 2.0;
    double outer = radius + region_width / 2.0;

    double ixs = qCos(qDegreesToRadians(start)) * inner + m_center.x();
    double iys = -qSin(qDegreesToRadians(start)) * inner + m_center.y();
    double ixe = qCos(qDegreesToRadians(end)) * inner + m_center.x();
    double iye = -qSin(qDegreesToRadians(end)) * inner + m_center.y();

    double oxs = qCos(qDegreesToRadians(start)) * outer + m_center.x();
    double oys = -qSin(qDegreesToRadians(start)) * outer + m_center.y();
    double oxe = qCos(qDegreesToRadians(end)) * outer + m_center.x();
    double oye = -qSin(qDegreesToRadians(end)) * outer + m_center.y();

    QPainterPath path;
    path.moveTo(ixs, iys);
    path.arcTo(m_center.x() - inner, m_center.y() - inner, 2 * inner, 2 * inner, start, SweepAngle(start, end));
    path.lineTo(oxe, oye);
    path.arcTo(m_center.x() - outer, m_center.y() - outer, 2 * outer, 2 * outer, end, -SweepAngle(start, end));
    path.lineTo(ixs, iys);
    path.closeSubpath();
    m_region->setPath(path);


    SetShape(kCX, m_center.x());
    SetShape(kCY, m_center.y());
    SetShape(kInnerRadius, inner);
    SetShape(kOuterRadius, outer);
    SetShape(kStartAngle, start);
#if ACCEPT_VCA_ARC_ERROR
    SetShape(kEndAngle, end - start);
#else
    SetShape(kEndAngle, end);
#endif
    SetShape(kIXS, ixs);
    SetShape(kIYS, iys);
    SetShape(kIXE, ixe);
    SetShape(kIYE, iye);
    SetShape(kOXS, oxs);
    SetShape(kOYS, oys);
    SetShape(kOXE, oxe);
    SetShape(kOYE, oye);
}

QPainterPath CreateArcPath(double cx, double cy, double radius, double start, double end) {
    QPainterPath path;
    double sx = qCos(qDegreesToRadians(start)) * radius + cx;
    double sy = -qSin(qDegreesToRadians(start)) * radius + cy;

    path.moveTo(sx, sy);
    path.arcTo(cx - radius, cy - radius, 2 * radius, 2 * radius, start, SweepAngle(start, end));

    return path;
}

