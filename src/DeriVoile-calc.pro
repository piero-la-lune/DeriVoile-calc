QT += widgets webkitwidgets network webkit

SOURCES += main.cpp FenPrincipale.cpp MAJ.cpp Classement.cpp Step1.cpp Step2.cpp Step3.cpp Step4.cpp FenPropos.cpp FenAide.cpp \
    FenRatings.cpp

HEADERS += FenPrincipale.h FenPropos.h FenAide.h \
    FenRatings.h

FORMS += FenPrincipale.ui FenPropos.ui FenAide.ui \
    FenRatings.ui

RESOURCES += resources.qrc

TRANSLATIONS += translations/EN.ts
