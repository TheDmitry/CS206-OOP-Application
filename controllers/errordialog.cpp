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
  call("It looks like error happened while parsing file!\n" + std::string(error.what()));
}

void ErrorDialog::callWithDbError(DbError const &error) {
  call("It looks like error happened while trying to open file!\n" + std::string(error.what()));
}

void ErrorDialog::call(std::string const &message) {
  ui->label->setText(QString::fromStdString(message));
  show();
}
