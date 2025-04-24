QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

FORM_DIR = views

DIALOGS += \
$$FORM_DIR/dialogs/authordialog.ui

PROVIDER_DIR = providers
ITEM_DIR = items
MODEL_DIR = models
CONTROLLER_DIR = controllers
EXTERNAL_DIR = external

PROVIDER_SOURCES = \
$$PROVIDER_DIR/vrheadsetprovider.cpp

PROVIDER_HEADERS = \
$$PROVIDER_DIR/abstractprovider.h \
$$PROVIDER_DIR/vrheadsetprovider.h

ITEM_SOURCES += \
$$ITEM_DIR/vrheadset.cpp

ITEM_HEADERS += \
$$ITEM_DIR/abstractitem.h \
$$ITEM_DIR/vrheadset.h

MODEL_SOURCES += \
$$MODEL_DIR/vrheadsettablemodel.cpp

MODEL_HEADERS += \
$$MODEL_DIR/vrheadsettablemodel.h

CONTROLLER_SOURCES += \
$$CONTROLLER_DIR/authordialog.cpp \
$$CONTROLLER_DIR/mainwindow.cpp

CONTROLLER_HEADERS += \
$$CONTROLLER_DIR/authordialog.h \
$$CONTROLLER_DIR/mainwindow.h

EXTERNAL_SOURCES += \
$$EXTERNAL_DIR/vector3.cpp \
$$EXTERNAL_DIR/dbfile.cpp

EXTERNAL_HEADERS += \
$$EXTERNAL_DIR/vector3.h \
$$EXTERNAL_DIR/dbfile.h


SOURCES += \
main.cpp \
$$ITEM_SOURCES \
$$MODEL_SOURCES \
$$CONTROLLER_SOURCES \
$$EXTERNAL_SOURCES \
$$PROVIDER_SOURCES

HEADERS += \
$$ITEM_HEADERS \
$$MODEL_HEADERS \
$$CONTROLLER_HEADERS \
$$EXTERNAL_HEADERS \
$$PROVIDER_HEADERS

FORMS += \
$$DIALOGS \
$$FORM_DIR/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS +=

DISTFILES +=
