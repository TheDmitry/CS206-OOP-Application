#include "authordialog.h"
#include "ui_authordialog.h"

AuthorDialog::AuthorDialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::AuthorDialog)
{
  ui->setupUi(this);
}

AuthorDialog::~AuthorDialog()
{
  delete ui;
}

void AuthorDialog::changeEvent(QEvent *e) {
  if (e->type() == QEvent::LanguageChange)
    ui->retranslateUi(this);

  QWidget::changeEvent(e);
}
