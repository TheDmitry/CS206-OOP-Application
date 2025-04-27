#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>
#include "external/dberror.h"
#include "external/parseerror.h"

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog {
  Q_OBJECT

public:
  explicit ErrorDialog(QWidget *parent = nullptr);
  ~ErrorDialog();

  void callWithDbError(DbError const &error);
  void callWithParseError(ParseError const &error);

private:
  void call(std::string const &message);
  Ui::ErrorDialog *ui;
};

#endif // ERRORDIALOG_H
