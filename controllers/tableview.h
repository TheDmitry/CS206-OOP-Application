#pragma once

#include <QAction>
#include <QMenu>
#include <QShortcut>
#include <QTableView>
#include <QWidget>

#include "models/customsortfilterproxymodel.h"
#include "models/customtablemodel.h"

#include "delegates/highlightdelegate.h"

namespace Ui {
class TableView;
}

/*!
 * \brief TableView class is used as controoler for TableView UI Component
 */
class TableView : public QWidget {
  Q_OBJECT

public:
  /*!
   * \brief TalbeView constructor
   */
  explicit TableView(QWidget *parent = nullptr);

  /*!
   * \brief TableView destructor
   */
  ~TableView();

  /*!
   * \brief Returns QTableView which is part of this component
   */
  QTableView *getTable();

  /*!
   * \brief Returns CustomTableModel which is model for QTableView used in this
   * component
   */
  CustomTableModel *getTableModel();

signals:
  /*!
   * \brief Called whenever drop from Drag&Drop functionality happened
   *
   * WARNING: success is not used
   */
  void dropHappened(bool success);

private slots:
  /*!
   * \brief TableView buttons, "Clear"
   *
   * Clears current table
   */
  void on_pushButton_clear_clicked();

  /*!
   * \brief TableView buttons, "New"
   *
   * Creates new line
   */
  void on_pushButton_new_clicked();

  /*!
   * \brief TableView buttons, "Remove"
   *
   * Removes current selected line
   */
  void on_pushButton_remove_clicked();

  /*!
   * \brief TableView buttons, "Undo"
   *
   * Undo latest change, can be used up to UNDO_STEPS limitation
   */
  void on_pushButton_undo_clicked();

  /*!
   * \brief TableView searchline
   *
   * Called whenever text in search line has been changed
   */
  void on_searchLine_textChanged(const QString &arg1);

  /*!
   * \brief TableView buttons, "Graph"
   *
   * Is used for SimpChart creation. From this function user can access
   * SimpChart component
   */
  void on_pushButton_graph_clicked();

private:
  /*!
   * \brief CustomTableModel for QTableView
   */
  CustomTableModel *model;

  /*!
   * \brief CustomSortFilterProxyModel for CustomTableModel
   */
  CustomSortFilterProxyModel *proxyModel;

  /*!
   * \brief Hotkeys storage
   */
  std::map<std::string, QShortcut *> shortcut;

  /*!
   * \brief Delegate for selected field highlighting
   */
  HighlightColumnDelegate *highlightDelegate;

  Ui::TableView *ui;

  /*!
   * \brief Hotkeys initialization
   */
  void initShortcuts();

  /*!
   * \brief Signals initialization
   */
  void connectSignals();

  /*!
   * \brief Here used solely for ui retranslation
   */
  void changeEvent(QEvent *e) override;
};
