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

INCLUDEPATH += /usr/local/include/opencv4/opencv /usr/local/include/opencv4 ./module/EnsembleAPI ./module/EnsembleAPI/src ./module/PacketEthComm ./module/EnsembleCommon ./module/EnsembleCommand ./module/ImgEncDec ./module/pugixml/src
LIBS += -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lopencv_gapi -lopencv_stitching -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_line_descriptor -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_videostab -lopencv_video -lopencv_viz -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core



FORMS += \
        mainwindow.ui \
    cdialogconnect.ui \
    formtoollist.ui \
    formjobbase.ui \
    formdeviceinfo.ui \
    formcandidatetool.ui \
    formjobtool.ui \
    dialogsetbase.ui \
    dialogchangename.ui \
    dialogsettoolobject.ui \
    dialogsettoolcrack.ui \
    dialogsettoolline.ui \
    dialogsettoolcircle.ui \
    dialogsettooldistance.ui \
    dialogsettoolinspectdistance.ui \
    cdialognewproject.ui \
    dialogselecttooloption.ui \
    formtooloption.ui \
    formproject.ui \
    dialogsettoolangle.ui \
    dialogsettoolcolor.ui \
    dialogsettooldistanceinspect.ui \
    dialogsettoolangleinspect.ui \
    dialogsettooldiameterinspect.ui \
    dialogsetcalibration.ui

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
    module/PacketEthComm/ErrorType.h \
    module/PacketEthComm/EthernetClient.h \
    module/PacketEthComm/RendezvueCheckData.h \
    module/pugixml/src/pugiconfig.hpp \
    module/pugixml/src/pugixml.hpp \
    cdialogconnect.h \
    cdialognewproject.h \
    cjobtree.h \
    cmat2qimage.h \
    csettoolinfo.h \
    csetuserregion.h \
    cthreaddraw.h \
    ctoollist.h \
    dialogchangename.h \
    dialogselecttooloption.h \
    dialogsetbase.h \
    dialogsetcalibration.h \
    dialogsettoolangle.h \
    dialogsettoolangleinspect.h \
    dialogsettoolcircle.h \
    dialogsettoolcolor.h \
    dialogsettoolcrack.h \
    dialogsettooldiameterinspect.h \
    dialogsettooldistance.h \
    dialogsettooldistanceinspect.h \
    dialogsettoolinspectdistance.h \
    dialogsettoolline.h \
    dialogsettoolobject.h \
    formcandidatetool.h \
    formdeviceinfo.h \
    formjobbase.h \
    formjobtool.h \
    formproject.h \
    formtoollist.h \
    formtooloption.h \
    mainwindow.h

SOURCES += \
    module/EnsembleAPI/src/Interface.cpp \
    module/EnsembleAPI/src/InterfaceControl.cpp \
    module/EnsembleAPI/EnsembleAPI.cpp \
    module/ImgEncDec/ImgDec.cpp \
    module/ImgEncDec/ImgEnc.cpp \
    module/PacketEthComm/EthernetClient.cpp \
    module/PacketEthComm/RendezvueCheckData.cpp \
    module/pugixml/src/pugixml.cpp \
    cdialogconnect.cpp \
    cdialognewproject.cpp \
    cjobtree.cpp \
    cmat2qimage.cpp \
    csettoolinfo.cpp \
    csetuserregion.cpp \
    cthreaddraw.cpp \
    ctoollist.cpp \
    dialogchangename.cpp \
    dialogselecttooloption.cpp \
    dialogsetbase.cpp \
    dialogsetcalibration.cpp \
    dialogsettoolangle.cpp \
    dialogsettoolangleinspect.cpp \
    dialogsettoolcircle.cpp \
    dialogsettoolcolor.cpp \
    dialogsettoolcrack.cpp \
    dialogsettooldiameterinspect.cpp \
    dialogsettooldistance.cpp \
    dialogsettooldistanceinspect.cpp \
    dialogsettoolinspectdistance.cpp \
    dialogsettoolline.cpp \
    dialogsettoolobject.cpp \
    formcandidatetool.cpp \
    formdeviceinfo.cpp \
    formjobbase.cpp \
    formjobtool.cpp \
    formproject.cpp \
    formtoollist.cpp \
    formtooloption.cpp \
    main.cpp \
    mainwindow.cpp


