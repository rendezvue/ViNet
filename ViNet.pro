#-------------------------------------------------
#
# Project created by QtCreator 2019-03-24T22:56:55
#
#-------------------------------------------------

QT       += core gui
#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViNet
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./src ./module/EnsembleAPI ./module/EnsembleAPI/src ./module/PacketEthComm ./module/PacketEthComm/Client ./module/EnsembleCommon ./module/EnsembleCommand ./module/ImgEncDec ./module/pugixml/src
LIBS += -lboost_system -lboost_thread -lboost_filesystem -lboost_regex

CONFIG += link_pkgconfig
PKGCONFIG += opencv4

RESOURCES += \
    resource.qrc

HEADERS += \
    module/EnsembleAPI/src/Interface.h \
    module/EnsembleAPI/src/InterfaceControl.h \
    module/EnsembleAPI/EnsembleAPI.h \
    module/EnsembleCommand/EnsembleCommand.h \
    module/EnsembleCommon/EnsembleCommon.h \
    module/ImgEncDec/ImgDec.h \
    module/ImgEncDec/ImgEnc.h \
    module/pugixml/src/pugiconfig.hpp \
    module/pugixml/src/pugixml.hpp \
    src/cdialogconnect.h \
    src/cdialognewproject.h \
    src/cjobtree.h \
    src/cmat2qimage.h \
    src/csettoolinfo.h \
    src/csetuserregion.h \
    src/cthreaddraw.h \
    src/ctoollist.h \
    src/dialogchangename.h \
    src/dialogselecttooloption.h \
    src/dialogsetbase.h \
    src/dialogsetcalibration.h \
    src/dialogsettoolangle.h \
    src/dialogsettoolangleinspect.h \
    src/dialogsettoolcircle.h \
    src/dialogsettoolcolor.h \
    src/dialogsettoolcrack.h \
    src/dialogsettooldiameterinspect.h \
    src/dialogsettooldistance.h \
    src/dialogsettooldistanceinspect.h \
    src/dialogsettoolinspectdistance.h \
    src/dialogsettoolline.h \
    src/dialogsettoolobject.h \
    src/formcandidatetool.h \
    src/formdeviceinfo.h \
    src/formjobbase.h \
    src/formjobtool.h \
    src/formproject.h \
    src/formtoollist.h \
    src/formtooloption.h \
    src/mainwindow.h \
    src/dialogsetcustomfeatureoption.h \
    src/csearchtreeitem.h \
    src/dialogresultinfo.h \
    src/canimatedlabel.h \
    src/dialogbasecameraconfig.h \
    src/cgetimagethread.h \
    src/dialogsettooloffsetdistance.h \
    src/dialogsetcode.h \
    src/dialogsetdetectplane.h \
    module/PacketEthComm/ErrorType.h \
    module/PacketEthComm/RendezvueCheckData.h \
    module/PacketEthComm/EthernetGetInfo.h \
    module/PacketEthComm/Client/EthernetClient.h \
    module/PacketEthComm/Client/EthernetClientControlData.h \
    src/censemble.h

SOURCES += \
    module/EnsembleAPI/src/Interface.cpp \
    module/EnsembleAPI/src/InterfaceControl.cpp \
    module/EnsembleAPI/EnsembleAPI.cpp \
    module/ImgEncDec/ImgDec.cpp \
    module/ImgEncDec/ImgEnc.cpp \
    module/pugixml/src/pugixml.cpp \
    src/cdialogconnect.cpp \
    src/cdialognewproject.cpp \
    src/cjobtree.cpp \
    src/cmat2qimage.cpp \
    src/csettoolinfo.cpp \
    src/csetuserregion.cpp \
    src/cthreaddraw.cpp \
    src/ctoollist.cpp \
    src/dialogchangename.cpp \
    src/dialogselecttooloption.cpp \
    src/dialogsetbase.cpp \
    src/dialogsetcalibration.cpp \
    src/dialogsettoolangle.cpp \
    src/dialogsettoolangleinspect.cpp \
    src/dialogsettoolcircle.cpp \
    src/dialogsettoolcolor.cpp \
    src/dialogsettoolcrack.cpp \
    src/dialogsettooldiameterinspect.cpp \
    src/dialogsettooldistance.cpp \
    src/dialogsettooldistanceinspect.cpp \
    src/dialogsettoolinspectdistance.cpp \
    src/dialogsettoolline.cpp \
    src/dialogsettoolobject.cpp \
    src/formcandidatetool.cpp \
    src/formdeviceinfo.cpp \
    src/formjobbase.cpp \
    src/formjobtool.cpp \
    src/formproject.cpp \
    src/formtoollist.cpp \
    src/formtooloption.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/dialogsetcustomfeatureoption.cpp \
    src/csearchtreeitem.cpp \
    src/dialogresultinfo.cpp \
    src/canimatedlabel.cpp \
    src/dialogbasecameraconfig.cpp \
    src/cgetimagethread.cpp \
    src/dialogsettooloffsetdistance.cpp \
    src/dialogsetcode.cpp \
    src/dialogsetdetectplane.cpp \
    module/PacketEthComm/EthernetGetInfo.cpp \
    module/PacketEthComm/RendezvueCheckData.cpp \
    module/PacketEthComm/Client/EthernetClient.cpp \
    module/PacketEthComm/Client/EthernetClientControlData.cpp \
    src/censemble.cpp

FORMS += \
    ui/cdialogconnect.ui \
    ui/cdialognewproject.ui \
    ui/dialogchangename.ui \
    ui/dialogselecttooloption.ui \
    ui/dialogsetbase.ui \
    ui/dialogsetcalibration.ui \
    ui/dialogsettoolangle.ui \
    ui/dialogsettoolangleinspect.ui \
    ui/dialogsettoolcircle.ui \
    ui/dialogsettoolcolor.ui \
    ui/dialogsettoolcrack.ui \
    ui/dialogsettooldiameterinspect.ui \
    ui/dialogsettooldistance.ui \
    ui/dialogsettooldistanceinspect.ui \
    ui/dialogsettoolinspectdistance.ui \
    ui/dialogsettoolline.ui \
    ui/dialogsettoolobject.ui \
    ui/formcandidatetool.ui \
    ui/formdeviceinfo.ui \
    ui/formjobbase.ui \
    ui/formjobtool.ui \
    ui/formproject.ui \
    ui/formtoollist.ui \
    ui/formtooloption.ui \
    ui/mainwindow.ui \
    ui/dialogsetcustomfeatureoption.ui \
    ui/dialogresultinfo.ui \
    ui/dialogbasecameraconfig.ui \
    ui/dialogsettooloffsetdistance.ui \
    ui/dialogsetcode.ui \
    ui/dialogsetdetectplane.ui

