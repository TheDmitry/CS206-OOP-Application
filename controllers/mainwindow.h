#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>

#include "controllers/authordialog.h"
#include "models/vrheadsettablemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:

  void on_actionProgramAuthor_triggered();

  void on_actionFileOpen_triggered();

  void on_actionFileWrite_triggered();

  void on_actionFileUpdate_triggered();

  void on_actionFileClose_triggered();

private:
  AuthorDialog *authorDialog;
  VRHeadsetTableModel *headsetTableModel;

  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
