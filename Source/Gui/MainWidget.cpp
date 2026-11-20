#include "MainWidget.h"

#include "../Data/Data.h"
#include "../Data/Mesh.h"

//------------------------------------------------------------------------------
gui::MainWidget::MainWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
//------------------------------------------------------------------------------
{
    setupUi(this);

    fileDialog = new QFileDialog();
    fileDialog->hide();

    connect(this->loadButton, SIGNAL(clicked()), fileDialog, SLOT(show()));
    connect(fileDialog, SIGNAL(accepted()), this, SLOT(accepted()));
}

//------------------------------------------------------------------------------
gui::MainWidget::~MainWidget()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
void gui::MainWidget::accepted()
//------------------------------------------------------------------------------
{
    dm::Data::instance()->mesh()->load(fileDialog->selectedFiles().first());

    glWidget->updateObject();
    glWidget->update();
}
