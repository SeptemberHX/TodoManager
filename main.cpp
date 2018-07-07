#include <QApplication>
#include "mainwindow.h"
#include "widgets/todolistwidget.h"
#include "widgets/tagwidget.h"
#include "core/ItemDetail.h"
#include "widgets/itemaddtagwidget.h"
#include "widgets/taglabelwidget.h"
#include "widgets/FetchConfigFilePathWidget.h"
#include "utils/styleutils.h"
#include "widgets/itemlistitemwidget.h"
#include "utils/itemdetailutils.h"
#include "utils/NofityUtils.h"
#include "dao/DaoFactory.h"
#include "widgets/CalendarMode/CalendarCellWidget.h"
#include <QLabel>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StyleUtils::setStyle(":/styles/defaultStyle.qss");
    qRegisterMetaTypeStreamOperators<todo::ItemDetail>();
    qRegisterMetaTypeStreamOperators<todo::ItemTag>();

//    MainWindow w;
//    w.show();

    CalendarCellWidget cellWidget;
    cellWidget.setDate(QDate::currentDate());
    cellWidget.show();

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
