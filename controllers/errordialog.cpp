#include "controllers/errordialog.h"
#include "external/dberror.h"
#include "external/parseerror.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::ErrorDialog) {
  ui->setupUi(this);
  ui->label->setText("");
}

ErrorDialog::~ErrorDialog() {
  delete ui;
}

void ErrorDialog::callWithParseError(ParseError const &error) {
  call(tr("It looks like error happened while parsing file!").toStdString() + "\n"
       + std::string(error.what()));
}

void ErrorDialog::callWithDbError(DbError const &error) {
  call(tr("It looks like error happened while trying to open file!").toStdString() + "\n"
       + std::string(error.what()));
}

void ErrorDialog::call(std::string const &message) {
  ui->label->setText(QString::fromStdString(message));
  show();
}

void ErrorDialog::changeEvent(QEvent *e) {
  if (e->type() == QEvent::LanguageChange)
    ui->retranslateUi(this);

  QWidget::changeEvent(e);
}
