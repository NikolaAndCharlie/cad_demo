#include "dxf_importer.h"
#include "3rdparty/dxflib/dl_dxf.h"
#include <iostream>
#include "line_entity.h"
#include "arc_entity.h"
#include "circle_entity.h"


DxfImport::DxfImport(double scale, const QPointF& offset)
         : m_scale(scale)
         , m_offset(offset)
{

}

DxfImport::~DxfImport()
{

}

bool DxfImport::Read(const QString& file_name)
{
    m_layers.clear();
    m_items.clear();

    DL_Dxf dxf_lib;
    Entity::ResetCounters();
    bool success = false;
    success = dxf_lib.in((const char*)file_name.toUtf8(), this);
    return success;
}

void DxfImport::addLayer(const DL_LayerData& data)
{
    m_layers.push_back(data.name.c_str());
}

void DxfImport::addLine(const DL_LineData& data)
{
    if (!m_in_model_space) {
        return;
    }

    double y1 = data.y1 * m_scale;
    double x1 = data.x1 * m_scale;
    double x2 = data.x2 * m_scale;
    double y2 = data.y2 * m_scale;

    y1 = -y1;
    y2 = -y2;

    x1 += m_offset.x();
    y1 += m_offset.y();
    x2 += m_offset.x();
    y2 += m_offset.y();
    std::shared_ptr<Entity> item = std::make_shared<LineEntity>(y1, x1, y2, x2);
    item->Set(Entity::kLayerName, getAttributes().getLayer().c_str());
    m_items.push_back(item);
}

void DxfImport::addCircle(const DL_CircleData& data) {
    if (!m_in_model_space) {
        return;
    }

    double cx = data.cx * m_scale;
    double cy = data.cy * m_scale;
    double radius = data.radius * m_scale;

    cy = -cy;

    cx += m_offset.x();
    cy += m_offset.y();

    std::shared_ptr<Entity> item = std::make_shared<CircleEntity>(cx, cy, radius);
    item->Set(Entity::kLayerName, getAttributes().getLayer().c_str());
    m_items.push_back(item);
}

void DxfImport::addBlock(const DL_BlockData& block) {
    if (block.name == "*Model_Space") {
        m_in_model_space = true;
    }
    else {
        m_in_model_space = false;
    }
}

void DxfImport::addArc(const DL_ArcData& data) {
    if (!m_in_model_space) {
        return;
    }

    double cx = data.cx * m_scale;
    double cy = data.cy * m_scale;
    double radius = data.radius * m_scale;
    double start = data.angle1;
    double end = data.angle2;

    cy = -cy;

    cx += m_offset.x();
    cy += m_offset.y();

    std::shared_ptr<Entity> item = std::make_shared<ArcEntity>(cx, cy, radius, start, end);
    item->Set(Entity::kLayerName, getAttributes().getLayer().c_str());
    m_items.push_back(item);
}