#include "line_entity.h"
#include "qmath.h"
#include "qpen.h"
#include "qbrush.h"
#include "qgraphicsview.h"
#include "qgraphicsitem.h"


QString LineEntity::kX0 = "X0";
QString LineEntity::kY0 = "Y0";
QString LineEntity::kX1 = "X1";
QString LineEntity::kY1 = "Y1";
QString LineEntity::kX2 = "X2";
QString LineEntity::kY2 = "Y2";
QString LineEntity::kX3 = "X3";
QString LineEntity::kY3 = "Y3";
QString LineEntity::kWidth = "Width";
QString LineEntity::kHeight = "Height";
QString LineEntity::kTheta = "Theta";

int LineEntity::m_counter = 0;

static void CalcLineABC(const QLineF& line, double& a, double& b, double& c)
{
    a = 1.0;
    b = 0.0;
    c = -line.p1().x();

    if (qAbs(line.p1().x() - line.p2().x()) >= 0.00001) {
        a = (line.p1().y() - line.p2().y()) / (line.p1().x() - line.p2().x());
        c = line.p1().y() - a * line.p1().x();
        b = -1.0;
    }
}

LineEntity::LineEntity(double r0, double c0, double r1, double c1)
{
    m_type = Type::kLineEntity;
    m_name = tr("LIN") + QString::number(m_counter++);
    m_tool_name = "FindLineTooL";

    m_line = new QGraphicsLineItem(r0, c0, r1, c1);
    QPen pen(m_properties[Entity::kItemPenColor].value<QColor>(), 1.0);
    pen.setCosmetic(true);
    m_line->setPen(pen);
    m_line->setFlags(QGraphicsItem::ItemIsSelectable);

    m_shape.insert(kX0, 0.0);
    m_shape.insert(kY0, 0.0);
    m_shape.insert(kX1, 0.0);
    m_shape.insert(kY1, 0.0);
    m_shape.insert(kX2, 0.0);
    m_shape.insert(kY2, 0.0);
    m_shape.insert(kX3, 0.0);
    m_shape.insert(kY3, 0.0);
    m_shape.insert(kTheta, m_properties[Entity::kRegionWidth].toDouble());
    m_shape.insert(kHeight, m_line->line().length());

    m_region = new QGraphicsPolygonItem;
    QPen region_pen(Get(Entity::kRegionPenColor).value<QColor>(), 1.0);
    region_pen.setCosmetic(true);
    m_region->setPen(region_pen);
    m_region->setBrush(QBrush(Get(Entity::kRegionBrushColor).value<QColor>()));
    m_region->setOpacity(Get(Entity::kRegionOpacity).toDouble());

    UpdateRegionItem();
}

LineEntity::~LineEntity()
{
}

bool LineEntity::Set(const QString& key, const QVariant& value)
{
    bool result = Entity::Set(key, value);
    if (result) {
        if (key == Entity::kRegionWidth || key == Entity::kRegionLengthRatio) {
            UpdateRegionItem();
        }
    }
    return result;
}

QGraphicsItem* LineEntity::GraphicsItem()
{
    return m_line;
}

QGraphicsItem* LineEntity::RegionGraphicsItem()
{
    return m_region;
}

void LineEntity::UpdateRegionItem()
{
    double a ,b, c;
    QLineF line = m_line->line();
    CalcLineABC(line, a, b, c);
    double m = qSqrt(a * a + b * b);
    if (m < 0.00001) {
        return ;
    }

    double ratio = Get(kRegionLengthRatio).toDouble();
    SetShape(kHeight, line.length() * ratio);

    double region_width = Get(kRegionWidth).toDouble();
    SetShape(kWidth, Get(kRegionWidth));

    double dx = a / m * region_width / 2.0;
    double dy = b / m * region_width / 2.0;

    QPointF center = line.center();
    QPointF p1 = center + ratio * (line.p1() - center);
    QPointF p2 = center + ratio * (line.p2() - center);

    SetShape(kX0, p1.x() + dx);
    SetShape(kY0, p1.y() + dy);
    SetShape(kX1, p1.x() - dx);
    SetShape(kY1, p1.y() - dy);
    SetShape(kX2, p2.x() - dx);
    SetShape(kY2, p2.y() - dy);
    SetShape(kX3, p2.x() + dx);
    SetShape(kY3, p2.y() + dy);

    double x0 = GetShape(kX0).toDouble();
    double y0 = GetShape(kY0).toDouble();
    double x1 = GetShape(kX1).toDouble();
    double y1 = GetShape(kY1).toDouble();

    if (qAbs(y1 - y0) < 0.0001) {
        SetShape(kTheta, 0.0);
    } else {
        double k = (y0 <= y1) ? (y0 - y1) /(x1 - x0) : (y1 - y0) /(x0 - x1);
        SetShape(kTheta, qAtan(k));
    }

    QVector<QPointF> pts{
        { GetShape(kX0).toDouble(), GetShape(kY0).toDouble() },
        { GetShape(kX1).toDouble(), GetShape(kY1).toDouble() },
        { GetShape(kX2).toDouble(), GetShape(kY2).toDouble() },
        { GetShape(kX3).toDouble(), GetShape(kY3).toDouble() }
    };

    m_region->setPolygon(QPolygonF(pts));
}