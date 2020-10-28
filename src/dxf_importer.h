#ifndef DXFIMPORT_H_
#define DXFIMPORT_H_

#include "qobject.h"
#include "qstringlist.h"
#include "qpoint.h"
#include <memory>
#include "3rdparty\dxflib\dl_creationadapter.h"

class Entity;

class DxfImport : public DL_CreationAdapter
{
public:
    DxfImport();
    ~DxfImport();

    bool Read(const QString& file_name);
    const QList<std::shared_ptr<Entity>>& items() const {return m_items;}
    const QStringList& layers() const {return m_layers;}

private:
    void addLayer(const DL_LayerData& data) override;
    void addLine(const DL_LineData& data) override;
    void addArc(const DL_ArcData& data) override;
    void addCircle(const DL_CircleData& data) override;

    void addBlock(const DL_BlockData&) override;
    void endBlock() override {m_in_model_space = true;}

    QStringList m_layers;
    QList<std::shared_ptr<Entity>> m_items;
    double m_scale;
    double m_offset;
    QPointF m_offset;
    bool m_in_model_space = true;
};

#endif // !DXFIMPORT_H_
