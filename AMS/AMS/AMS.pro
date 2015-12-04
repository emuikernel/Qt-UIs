#-------------------------------------------------
#
# Project created by QtCreator 2013-12-30T11:05:41
#
#-------------------------------------------------

QT       += core gui sql network axcontainer multimedia printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AMS
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp \
    frmhost.cpp \
    frmdata.cpp \
    frmlogin.cpp \
    frmlogout.cpp \    
    frmdefence.cpp \    
    frmconfig.cpp \
    frmselect.cpp \
    frmpwd.cpp \
    frmpopup.cpp \
    api/myapp.cpp \
    api/myapi.cpp \
    api/mysqliteapi.cpp \
    frmhostinfo.cpp \
    usercontrol/myswitchbutton.cpp \
    usercontrol/mydefencebutton.cpp

HEADERS  += frmmain.h \    
    frmhost.h \
    frmdata.h \
    frmlogin.h \
    frmlogout.h \    
    frmdefence.h \    
    frmconfig.h \
    frmselect.h \
    frmpwd.h \
    frmpopup.h \
    frmhostinfo.h \
    api/myhelper.h \
    api/myapp.h \
    api/myapi.h \
    api/myexcelapi.h \
    api/myprintapi.h \
    api/mysqliteapi.h \
    usercontrol/myswitchbutton.h \
    usercontrol/mydefencebutton.h

FORMS    += frmmain.ui \
    frmhost.ui \
    frmdata.ui \
    frmlogin.ui \
    frmlogout.ui \
    frmdefence.ui \
    frmconfig.ui \
    frmselect.ui \
    frmpwd.ui \
    frmpopup.ui \
    frmhostinfo.ui

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin

win32:RC_FILE=main.rc

RESOURCES += \
    main.qrc
