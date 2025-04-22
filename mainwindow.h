#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <list>

#include "authordialog.h"
#include "dbfile.h"
#include "vrheadset.h"
#include "vrheadsettablemodel.h"

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

private:
  std::list<VRHeadset> headsets;
  DbFile db;
  AuthorDialog *authorDialog;
  VRHeadsetTableModel *headsetTableModel;

  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
