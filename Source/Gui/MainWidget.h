#pragma once

#include <QFileDialog>
#include <QWidget>

#include "ui_MainWidget.h"

//------------------------------------------------------------------------------
namespace gui
//------------------------------------------------------------------------------
{

//! The widget
class MainWidget : public QWidget, Ui_MainWidget {
    Q_OBJECT

  public:
    MainWidget(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

    virtual ~MainWidget();

  public slots:
    void accepted();

  private:
    QFileDialog *fileDialog;
};

//------------------------------------------------------------------------------
}; // namespace gui
//------------------------------------------------------------------------------
