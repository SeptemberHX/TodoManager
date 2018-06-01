#include <QApplication>
#include "mainwindow.h"
#include "widgets/todolistwidget.h"
#include "widgets/tagwidget.h"
#include "core/ItemDetail.h"
#include "widgets/itemaddtagwidget.h"
#include "widgets/taglabelwidget.h"
#include "utils/styleutils.h"
#include "widgets/itemlistitemwidget.h"
#include "utils/itemdetailutils.h"
#include "dao/DaoFactory.h"
#include <QLabel>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StyleUtils::setStyle(":/styles/defaultStyle.qss");
    qRegisterMetaTypeStreamOperators<todo::ItemDetail>();

    MainWindow w;
    w.show();

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

    return a.exec();
}
