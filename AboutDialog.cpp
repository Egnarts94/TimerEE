#include "AboutDialog.h"
#include "ui_AboutDialog.h"

CAboutDialog::CAboutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CAboutDialog)
{
  ui->setupUi(this);
}

CAboutDialog::~CAboutDialog()
{
  delete ui;
}
