#include "MainWidget.h" // Header

// Qt

// Project
#include "../Data/Data.h"
#include "../Data/Mesh.h"

//------------------------------------------------------------------------------
gui::MainWidget::MainWidget(QWidget *parent /* = 0*/,
                            Qt::WindowFlags f /* = 0*/)
    : QWidget(parent, f)
//------------------------------------------------------------------------------
{
    setupUi(this);

    mFileDialog = new QFileDialog();
    mFileDialog->hide();

    connect(this->mLoadButton, SIGNAL(clicked()), mFileDialog, SLOT(show()));
    connect(mFileDialog, SIGNAL(accepted()), this, SLOT(accepted()));
}

//------------------------------------------------------------------------------
gui::MainWidget::~MainWidget()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
void gui::MainWidget::accepted()
//------------------------------------------------------------------------------
{
    dm::Data::instance()->mesh()->load(mFileDialog->selectedFiles().first());

    mGlWidget->updateObject();
    mGlWidget->update();
    // mGlWidget->updateGL();
}
