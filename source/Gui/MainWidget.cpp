#include "MainWidget.h" // Header

// Qt

// Project
#include "../Data/Data.h"
#include "../Data/Mesh.h"

//------------------------------------------------------------------------------
gui::MainWidget::MainWidget(QWidget * parent/* = 0*/, Qt::WindowFlags f/* = 0*/) 
	: QWidget(parent,f)
//------------------------------------------------------------------------------
{
	setupUi(this);

	m_fileDialog = new QFileDialog();
	m_fileDialog->hide();

    connect(this->m_loadButton,SIGNAL(clicked()),m_fileDialog,SLOT(show()));
	connect(m_fileDialog,SIGNAL(accepted()),this,SLOT(accepted()));


}

//------------------------------------------------------------------------------
gui::MainWidget::~MainWidget()
//------------------------------------------------------------------------------
{


}

//------------------------------------------------------------------------------
void gui::MainWidget::accepted()
//------------------------------------------------------------------------------
{
	dm::Data::instance()->mesh()->load(m_fileDialog->selectedFiles().first());

	m_glWidget->updateObject();
	m_glWidget->updateGL();
}
