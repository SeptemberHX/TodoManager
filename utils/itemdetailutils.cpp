#include "itemdetailutils.h"
#include "ItemTagUtils.h"
#include <algorithm>
#include <random>
#include <chrono>

ItemDetailUtils::ItemDetailUtils()
{

}

todo::ItemDetail ItemDetailUtils::createOneItemDetail() {
    QString titleStr;
    todo::ItemDetail id(titleStr);
    id.setTitle(QString("This is test NO ") + id.getId() + QString(". Test for everything !!!!"));
    id.setMode(todo::ItemMode::SCHEDULE);
    id.setDescription("Test. \n A beautiful test !!! \n Hello World ! \n iiiiiiiiiiiiiiiiiiiiiiiiii jjjjjjjj kkkkkkkkkkk mmmmmmmmmmm iii");
    id.setTargetDate(QDate(2018, 4, 27));
    id.setFromTime(QTime(12, 20));
    id.setToTime(QTime(13, 50));
    id.setPriority(2);
    id.setDone(true);

    QStringList tagNameList;
    tagNameList << "test";
    tagNameList << "International";
    tagNameList << "Company";
    tagNameList << "Try your best";
    tagNameList << "Hello World!";
    tagNameList << "Beautiful";
    tagNameList << "Computer";
    tagNameList << "Nintendo Switch";

    std::shuffle(tagNameList.begin(), tagNameList.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    long tagCount = random() % tagNameList.size();
    for (int i = 0; i < tagCount; ++i) {
        id.addTag(todo::ItemTagUtils::convertStr2Tag(tagNameList[i]));
    }

    id.setCreatedTime(QDateTime::currentDateTime());
    id.setLastUpdatedTime(QDateTime::currentDateTime());

    return id;
}
