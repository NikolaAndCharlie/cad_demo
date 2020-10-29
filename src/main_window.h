#ifndef MAINWIDGET_H_
#define MAINWIDGET_H_

#include "entity_view.h"
#include "dxf_importer.h"
#include "qwidget.h"
#include <memory>

class QLineEdit;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget();
    ~MainWidget();
private slots:
    void LoadDxf();
    void ItemSelectionChanged();

private:
    void CreateUi();

    void SetLayers(const QStringList& layers);
    void SetItems(const QList<std::shared_ptr<Entity>>& items);

    std::shared_ptr<Entity> m_current_item;
    EntityView* m_view;

    QLineEdit* m_dxf_file_path_line_eidt;
    QList<QGraphicsItem*> m_selected_items;
};

#endif // !MAINWIDGET_H_
