#include <QApplication>
#include "mainwindow.h"
#include "utils/styleutils.h"
#include <QDebug>


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

//    GroupDetailWidget gdw;
//    gdw.show();

//    CalendarTimeLineWidget timeLineWidget;
//    todo::DataCenter dataCenter;
//    timeLineWidget.setItemDetailList(dataCenter.selectItemDetailByDate(QDate(2018, 6, 13)));
//    timeLineWidget.show();

//    CalendarModeWidget cellWidget;
//    cellWidget.show();

//    TodoListWidget tlw;
//    tlw.show();

//    TagWidget tw;
//    tw.addTag("HomeWork");
//    tw.addTag("Python");
//    // tw.setReadOnly(true);
//    tw.show();

//    ItemAddTagWidget iatw;
//    iatw.show();

//    TagLabelWidget tlw;
//    tlw.setText("Test");
//    tlw.show();

//    ItemListItemWidget iliw;
//    iliw.setTagColor(QColor(255, 0, 0, 127));
//    iliw.show();
//
//    iliw.loadItemDetail(ItemDetailUtils::createOneItemDetail());

//    auto daoFactory = todo::DaoFactory::getInstance();
////    daoFactory->getSQLDao()->insertItemDetail(ItemDetailUtils::createOneItemDetail());
////    daoFactory->getSQLDao()->deleteItemDetailByID("todo_1526782083274");
//    todo::ItemTag tag("test");
//    tag.setColor("#202387");
//    daoFactory->getSQLDao()->insertItemTag(tag);

//    FetchConfigFilePathWidget fcfpd;
//    fcfpd.show();

    return a.exec();
}
