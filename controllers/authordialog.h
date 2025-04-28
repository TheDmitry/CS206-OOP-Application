#ifndef AUTHORDIALOG_H
#define AUTHORDIALOG_H

#include <QDialog>

namespace Ui {
class AuthorDialog;
}

class AuthorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AuthorDialog(QWidget *parent = nullptr);
  ~AuthorDialog();

private:
  Ui::AuthorDialog *ui;

  void changeEvent(QEvent *e) override;
};

#endif // AUTHORDIALOG_H
