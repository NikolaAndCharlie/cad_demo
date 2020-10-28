#include "entity.h"
#include "qcolor.h"
#include "qgraphicsitem.h"
#include "QtXml\qxml.h"
#include "line_entity.h"
#include "arc_entity.h"
#include "circle_entity.h"

QString Entity::kLayerName = "LayerName";
QString Entity::kItemPenColor = "ItemPenColor";
QString Entity::kRegionPenColor = "RegionPenColor";
QString Entity::kRegionBrushColor = "RegionBrushColor";
QString Entity::kRegionOpacity = "RegionOpacity";
QString Entity::kRegionWidth = "RegionWidth";
QString Entity::kRegionLengthRatio = "RegionLengthRatio";


Entity::Entity()
{
    m_type = Type::kLineEntity;
    m_properties.insert(kLayerName, QString());
    m_properties.insert(kItemPenColor, QVariant(QColor(Qt::white)));
    m_properties.insert(kRegionPenColor, QVariant(QColor(Qt::green)));
    m_properties.insert(kRegionBrushColor, QVariant(QColor(0, 100, 0)));
    m_properties.insert(kRegionWidth, 20);
    m_properties.insert(kRegionOpacity, 0.4);
    m_properties.insert(kRegionLengthRatio, 0.8);
}


bool Entity::Set(const QString& key, const QVariant& value)
{
    if (m_properties.contains(key) && m_properties[key].type() == value.type()) {
        if (m_properties[key] != value) {
            m_properties[key] = value;
        }
        return true;
    }
    return false;
}

bool Entity::SetShape(const QString& key, const QVariant& value)
{
    if (m_shape.contains(key) && m_shape[key].type() == value.type()) {
        if (m_shape[key] != value) {
            m_shape[key] = value;
        }
        return true;
    }

    return false;
}

bool Entity::SetParameter(const QString& key, const QVariant& value)
{
    if (m_parameters.contains(key) && m_parameters[key].type() == value.type()) {
        if (m_parameters[key] != value) {
            m_parameters[key] = value;
        }
        return true;
    }
    return false;
}

QVariant Entity::Get(const QString& key)
{
    if (m_properties.contains(key)) {
        return m_properties[key];
    }
    return QVariant();
}

QVariant Entity::GetShape(const QString& key)
{
    if (m_shape.contains(key)) {
        return m_shape[key];
    }
    return QVariant();
}

QVariant Entity::GetParameter(const QString& key)
{
    if(m_parameters.contains(key)) {
        return m_parameters[key];
    }

    return QVariant();
}

void Entity::ResetCounters()
{
    LineEntity::ResetCounter();
    CircleEntity::ResetCounter();
    ArcEntity::ResetCounter();
}

void Entity::set_visible(bool visible)
{
    if (visible != m_visible) {
        m_visible = visible;
        GraphicsItem()->setVisible(m_visible);
        RegionGraphicsItem()->setVisible(m_visible);
    }
}