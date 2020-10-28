#ifndef ENTITY_H_
#define ENTITY_H_

#include "qobject.h"
#include "qstring.h"
#include "qmap.h"
#include "qrect.h"
#include "qvariant.h"

class QGraphicsItem;

class Entity : public QObject
{
    Q_OBJECT
public:
    static QString kLayerName;
    static QString kItemPenColor;
    static QString kRegionPenColor;
    static QString kRegionBrushColor;
    static QString kRegionOpacity;
    static QString kRegionWidth;

    static QString kRegionLengthRatio;

    enum class Type {
        kLineEntity,
        kCircleEntity,
        kArcEntity
    };

    Entity();
    virtual ~Entity() {}
    Type type() const {return m_type;}
    QString name() const {return m_name;}
    QString tool_name() const {return m_tool_name;}
    
    static void ResetCounters();

    bool visible() const {return m_visible;}
    void set_visible(bool visible);
    
    // for item
    virtual QGraphicsItem* GraphicsItem() = 0;
    // for region
    virtual QGraphicsItem* RegionGraphicsItem() = 0;
    virtual bool Set(const QString&, const QVariant&);
    virtual bool SetShape(const QString&, const QVariant&);
    virtual bool SetParameter(const QString&, const QVariant&);

    virtual QVariant Get(const QString&);
    virtual QVariant GetShape(const QString&);
    virtual QVariant GetParameter(const QString&);

protected:
    virtual void UpdateRegionItem() = 0;

    QMap<QString, QVariant> m_shape;
    QMap<QString, QVariant> m_parameters;
    QMap<QString, QVariant> m_properties;

    Type m_type;
    QString m_name;
    bool m_visible = true;
    QString m_tool_name;
};
#endif // !ENTITY_H_
