#include <QApplication>
#include "mainwindow.h"
#include "utils/styleutils.h"
#include <QDebug>
#include "widgets/MyTextEdit.h"
#include "widgets/StickyNote/StickyNoteWidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StyleUtils::setStyle(":/styles/defaultStyle.qss");
    qRegisterMetaTypeStreamOperators<todo::ItemDetail>();
    qRegisterMetaTypeStreamOperators<todo::ItemTag>();
    qRegisterMetaTypeStreamOperators<todo::ItemAndGroupWrapper>();
    qSetMessagePattern("%{file}:%{line}, %{function}: %{message}");

    MainWindow w;
    w.show();

    StickyNoteWidget snw;
    snw.show();

    return a.exec();
}
