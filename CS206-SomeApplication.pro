QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

FORMS_DIRECTORY += views
DIALOGS_DIRECTORY += $$FORMS_DIRECTORY/dialogs

SOURCES += \
  authordialog.cpp \
main.cpp \
mainwindow.cpp \
vector3.cpp \
vrheadset.cpp \
dbfile.cpp \
  vrheadsettablemodel.cpp


HEADERS += \
  authordialog.h \
mainwindow.h \
vector3.h \
vrheadset.h \
dbfile.h \
  vrheadsettablemodel.h

FORMS += \
  $$DIALOGS_DIRECTORY/authordialog.ui \
  $$FORMS_DIRECTORY/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS +=

DISTFILES +=
