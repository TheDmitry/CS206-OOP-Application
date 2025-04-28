QT += core gui widgets linguist

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
CONFIG += lrelease
CONFIG += embed_translations

FORM_DIR = views
DIALOG_DIR = $$FORM_DIR/dialogs

DIALOGS += \
$$DIALOG_DIR/authordialog.ui \
$$DIALOG_DIR/errordialog.ui

PROVIDER_DIR = providers
ITEM_DIR = items
MODEL_DIR = models
CONTROLLER_DIR = controllers
DELEGATE_DIR = delegates
EXTERNAL_DIR = external


# Providers (Adapters)
PROVIDER_SOURCES = \
$$PROVIDER_DIR/vrheadsetprovider.cpp # VRHeadsetProvider

PROVIDER_HEADERS = \
$$PROVIDER_DIR/abstractprovider.h \ # AbstractProvider
$$PROVIDER_DIR/vrheadsetprovider.h # VRHeadsetProvider


# Items
ITEM_SOURCES += \
$$ITEM_DIR/vrheadset.cpp # VRHeadset

ITEM_HEADERS += \
$$ITEM_DIR/abstractitem.h \ # AbstractItem
$$ITEM_DIR/vrheadset.h # VRHeadset


# Models
MODEL_SOURCES += \
$$MODEL_DIR/customtablemodel.cpp \ # CustomTableModel
$$MODEL_DIR/customsortfilterproxymodel.cpp \ # CustomSortFilterProxyModel

MODEL_HEADERS += \
$$MODEL_DIR/customtablemodel.h \ # CustomTableModel
$$MODEL_DIR/customsortfilterproxymodel.h \ # CustomSortFilterProxyModel



# Controllers
CONTROLLER_SOURCES += \
$$CONTROLLER_DIR/authordialog.cpp \ # AuthorDialog
$$CONTROLLER_DIR/errordialog.cpp \ # ErrorDialog
$$CONTROLLER_DIR/mainwindow.cpp \ # MainWindow
$$CONTROLLER_DIR/tableview.cpp \ # TableView
$$CONTROLLER_DIR/workspace.cpp \ # Workspace

CONTROLLER_HEADERS += \
$$CONTROLLER_DIR/authordialog.h \ # AuthorDialog
$$CONTROLLER_DIR/errordialog.h \ # ErrorDialog
$$CONTROLLER_DIR/mainwindow.h \ # MainWindow
$$CONTROLLER_DIR/tableview.h \ # TableView
$$CONTROLLER_DIR/workspace.h # Workspace


# Delegates
DELEGATE_SOURCES = \
$$DELEGATE_DIR/highlightdelegate.cpp \ # HighlightColumnDelegate

DELEGATE_HEADERS = \
$$DELEGATE_DIR/highlightdelegate.h \ # HighlightColumnDelegate


# Externals (Might be some utility, throwables, special types, external libs)
EXTERNAL_SOURCES += \
$$EXTERNAL_DIR/vector3.cpp \ # Vector3
$$EXTERNAL_DIR/dbfile.cpp \ # DbFile -> parser
$$EXTERNAL_DIR/parseerror.cpp \ # ParseError -> throwable
$$EXTERNAL_DIR/dberror.cpp \ # DbError -> throwable

EXTERNAL_HEADERS += \
$$EXTERNAL_DIR/vector3.h \ # Vector3
$$EXTERNAL_DIR/dbfile.h \ # DbFile
$$EXTERNAL_DIR/parseerror.h \ # ParseError -> throwable
$$EXTERNAL_DIR/dberror.h \ # DbError -> throwable
$$EXTERNAL_DIR/undoer.h \ # Undoer

SOURCES += \
main.cpp \
$$ITEM_SOURCES \
$$MODEL_SOURCES \
$$CONTROLLER_SOURCES \
$$DELEGATE_SOURCES \
$$EXTERNAL_SOURCES \
$$PROVIDER_SOURCES \

HEADERS += \
$$ITEM_HEADERS \
$$MODEL_HEADERS \
$$CONTROLLER_HEADERS \
$$DELEGATE_HEADERS \
$$EXTERNAL_HEADERS \
$$PROVIDER_HEADERS \

FORMS += \
$$DIALOGS \
$$FORM_DIR/mainwindow.ui \
  views/tableview.ui \
  views/workspace.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS += translations/project_ru_RU.ts
TRANSLATIONS += translations/project_en_US.ts

DISTFILES +=
