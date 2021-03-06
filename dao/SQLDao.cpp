//
// Created by septemberhx on 5/17/18.
//

#include <QList>
#include <QDebug>
#include <QSqlQuery>
#include <QException>
#include <QSqlError>
#include <iostream>
#include "SQLDao.h"
#include "../core/SqlErrorException.h"
#include "../config/TodoConfig.h"


QList<todo::ItemDetailDao> todo::SQLDao::selectItemDetailByDate(const QDate &targetDate) {
    QList<ItemDetailDao> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType,"
                  " createdTime, lastUpdatedTime"
                  " FROM todo_items"
                  " WHERE targetDate = :targetDate");
    query.bindValue(":targetDate", targetDate);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemDetailDao detail;
            detail.setId(query.value("id").toString());
            detail.setTitle(query.value("title").toString());
            detail.setPriority(query.value("priority").toInt());
            detail.setDescription(query.value("description").toString());
            detail.setFromTime(query.value("startTime").toTime());
            detail.setToTime(query.value("endTime").toTime());
            detail.setDone(query.value("done").toBool());
            detail.setTargetDate(query.value("targetDate").toDate());
            detail.setMode(todo::ItemMode(query.value("type").toInt()));
            detail.setCreatedTime(query.value("createdTime").toDateTime());
            detail.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            // todo: parentID, freq, freqType in SELECT
            resultList.append(detail);
        }
    }

    return resultList;
}

void todo::SQLDao::updateItemDetailByID(const QString &itemID, const todo::ItemDetailDao &itemDetailDao) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE todo_items"
                  " SET title=:title, priority=:priority, description=:description, parentID=:parentID,"
                  "     startTime=:startTime, endTime=:endTime, done=:done, targetDate=:targetDate,"
                  "     `type`=:type, freq=:freq, freqType=:freqType, createdTime=:createdTime, lastUpdatedTime=:lastUpdatedTime"
                  " WHERE id=:id;");
    query.bindValue(":id", itemDetailDao.getId());
    query.bindValue(":title", itemDetailDao.getTitle());
    query.bindValue(":priority", itemDetailDao.getPriority());
    query.bindValue(":description", itemDetailDao.getDescription());
    query.bindValue(":parentID", QVariant::Int);
    query.bindValue(":startTime", itemDetailDao.getFromTime());
    query.bindValue(":endTime", itemDetailDao.getToTime());
    query.bindValue(":done", itemDetailDao.isDone());
    query.bindValue(":targetDate", itemDetailDao.getTargetDate());
    query.bindValue(":type", itemDetailDao.getMode());
    query.bindValue(":freq", QVariant::Int);
    query.bindValue(":freqType", QVariant::Int);
    query.bindValue(":createdTime", itemDetailDao.getCreatedTime());
    query.bindValue(":lastUpdatedTime", itemDetailDao.getLastUpdatedTime());
    // todo: parentID, freq, freqType in UPDATE

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemDetailByID(const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM todo_items"
                  " WHERE id=:id;");
    query.bindValue(":id", itemID);

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemDetailByIDs(const QList<QString> &itemIDs) {
    if (itemIDs.isEmpty()) return;

    QString whereClauseStr = this->generateWhereClauseStrValues("id", "=", false, itemIDs);
    QString queryStr("DELETE FROM todo_items WHERE ");
    queryStr += whereClauseStr;
    QSqlQuery query(this->db);
    if (!query.exec(queryStr)) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::insertItemDetail(const todo::ItemDetailDao &itemDetailDao) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO todo_items"
                  " (id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType, createdTime, lastUpdatedTime)"
                  " VALUES(:id, :title, :priority, :description, :parentID, :startTime, :endTime, :done, :targetDate, :type, :freq, :freqType, :createdTime, :lastUpdatedTime);");
    query.bindValue(":id", itemDetailDao.getId());
    query.bindValue(":title", itemDetailDao.getTitle());
    query.bindValue(":priority", itemDetailDao.getPriority());
    query.bindValue(":description", itemDetailDao.getDescription());
    query.bindValue(":parentID", QVariant::Int);
    query.bindValue(":startTime", itemDetailDao.getFromTime());
    query.bindValue(":endTime", itemDetailDao.getToTime());
    query.bindValue(":done", itemDetailDao.isDone());
    query.bindValue(":targetDate", itemDetailDao.getTargetDate());
    query.bindValue(":type", itemDetailDao.getMode());
    query.bindValue(":freq", QVariant::Int);
    query.bindValue(":freqType", QVariant::Int);
    query.bindValue(":createdTime", itemDetailDao.getCreatedTime());
    query.bindValue(":lastUpdatedTime", itemDetailDao.getLastUpdatedTime());
    // todo: parentID, freq, freqType in INSERT
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

todo::SQLDao::~SQLDao() {
    this->db.close();
}

void todo::SQLDao::init() {
    qDebug() << "Init SQLDao";
    if (this->sqlType == 0) {
        this->db = QSqlDatabase::addDatabase("QMYSQL");
        this->db.setUserName("root");
        this->db.setPassword("");
        this->db.setDatabaseName("todo_manager");
    } else if (this->sqlType == 1) {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
        this->db.setUserName("root");
        this->db.setPassword("");
        this->db.setDatabaseName(TodoConfig::getInstance()->getSqLiteConfig().getDbPath());
    }

    bool openResult = this->db.open();

    if (!openResult) {
        qDebug() << "Open database failed";
        throw SqlErrorException(this->db.lastError().text().toStdString().c_str());
    } else {
        this->createTables();
    }

    for (auto name : this->db.tables()) {
        qDebug() << name;
    }
}

todo::SQLDao::SQLDao()
    : sqlType(0) {

}

void todo::SQLDao::createTables() {
    QStringList sqlScripts;
    sqlScripts.append("CREATE TABLE if not exists tags ("
                      "     name VARCHAR(255) NOT NULL,"
                      "     description text,"
                      "     color VARCHAR(7),"
                      "     PRIMARY KEY (name)"
                      ");");
    sqlScripts.append("CREATE TABLE if not exists todo_items ("
                      "     id VARCHAR(255) NOT NULL,"
                      "     title VARCHAR(255) NOT NULL,"
                      "     priority INTEGER NOT NULL,"
                      "     description LONGTEXT,"
                      "     parentID VARCHAR(50),"
                      "     startTime DATETIME,"
                      "     endTime DATETIME,"
                      "     done BOOLEAN NOT NULL,"
                      "     targetDate DATETIME,"
                      "     type INTEGER NOT NULL,"
                      "     freq INTEGER,"
                      "     freqType INTEGER,"
                      "     createdTime DATETIME,"
                      "     lastUpdatedTime DATETIME,"
                      "     PRIMARY KEY (id)"
                      ");");
    sqlScripts.append("CREATE TABLE if not exists item_tags ("
                      "     itemID VARCHAR(255),"
                      "     tagID VARCHAR(255),"
                      "     orderNum INTEGER NOT NULL,"
                      "     foreign key(itemID) references todo_items(id),"
                      "     foreign key(tagID) references tags(name)"
                      ");");
    sqlScripts.append("CREATE TABLE if not exists item_groups ("
                      "     id VARCHAR(255) NOT NULL,"
                      "     title VARCHAR(255) NOT NULL,"
                      "     description LONGTEXT,"
                      "     fromDate DATETIME,"
                      "     toDate DATETIME,"
                      "     type INTEGER NOT NULL,"
                      "     milestone BOOLEAN NOT NULL,"
                      "     createdTime DATETIME,"
                      "     lastUpdatedTime DATETIME,"
                      "     color VARCHAR(7),"
                      "     PRIMARY KEY (id)"
                      ");");
    sqlScripts.append("CREATE TABLE if not exists item_group_relations("
                      "     rootGroupID VARCHAR(255) NOT NULL,"
                      "     directGroupID VARCHAR(255) NOT NULL,"
                      "     itemID VARCHAR(255) NOT NULL"
                      ");");
    sqlScripts.append("CREATE TABLE IF NOT EXISTS sticky_note("
                      "     id VARCHAR(255) NOT NULL,"
                      "     name VARCHAR(255) NOT NULL,"
                      "     shown BOOLEAN NOT NULL,"
                      "     x INTEGER NOT NULL,"
                      "     y INTEGER NOT NULL,"
                      "     type INTEGER NOT NULL,"
                      "     fontColor VARCHAR(7),"
                      "     backgroundColor VARCHAR(7),"
                      "     createdTime DATETIME,"
                      "     lastUpdatedTime DATETIME,"
                      "     PRIMARY KEY (id)"
                      ");");
    sqlScripts.append("CREATE TABLE IF NOT EXISTS todo_item_times("
                      "     itemID VARCHAR(255) NOT NULL,"
                      "     startTime DATETIME NOT NULL,"
                      "     endTime DATETIME NOT NULL"
                      ");");

    QSqlQuery query(this->db);
    for (auto &sqlScript : sqlScripts) {
        if (!query.exec(sqlScript)) {
            qDebug() << QString("Failed to execute sql %1").arg(sqlScript);
            throw SqlErrorException(query.lastError().text().toStdString().c_str());
        }
    }
}

void todo::SQLDao::updateDoneByID(const QString &itemID, bool flag) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE todo_items"
                  " SET done=:done"
                  " WHERE id=:id;");
    query.bindValue(":id", itemID);
    query.bindValue(":done", flag);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

QList<todo::ItemTag> todo::SQLDao::selectItemTagById(const QString &tagId) {
    QList<todo::ItemTag> resultLists;
    QSqlQuery query(this->db);
    query.prepare("SELECT name, description, color"
                  " FROM tags"
                  " WHERE name = :tagId");
    query.bindValue(":tagId", tagId);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemTag itemTag(query.value("name").toString());
            itemTag.setColor(query.value("color").toString());
            resultLists.append(itemTag);
        }
    }

    return resultLists;
}

void todo::SQLDao::updateItemTagById(const QString &tagId, const ItemTag &itemTag) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE tags"
                  " SET description=:description, color=:color"
                  " WHERE name=:name;");
    query.bindValue(":description", QVariant::String);
    query.bindValue(":color", itemTag.getColor().name());
    query.bindValue(":name", itemTag.getName());

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemTagById(const QString &tagId) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM tags"
                  " WHERE name=:name;");
    query.bindValue(":name", tagId);

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::insertItemTag(const todo::ItemTag &tag) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO tags"
                  " (name, description, color)"
                  " VALUES(:name, :description, :color);");
    query.bindValue(":name", tag.getName());
    query.bindValue(":description", QVariant::String);
    query.bindValue(":color", tag.getColor().name());

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

QList<todo::ItemTag> todo::SQLDao::selectAllItemTag() {
    QList<todo::ItemTag> resultLists;
    QSqlQuery query(this->db);
    query.prepare("SELECT name, description, color"
                  " FROM tags;");
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemTag itemTag(query.value("name").toString());
            itemTag.setColor(query.value("color").toString());
            resultLists.append(itemTag);
        }
    }

    return resultLists;
}

QList<todo::ItemDetailAndTag> todo::SQLDao::selectItemAndTagMatchByItemID(const QString &itemID) {
    QList<ItemDetailAndTag> results;
    QSqlQuery query(this->db);
    query.prepare("SELECT itemID, tagID, orderNum"
                  " FROM item_tags"
                  " WHERE itemID = :itemID;");
    query.bindValue(":itemID", itemID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            results.append(ItemDetailAndTag(query.value("itemID").toString(),
                                            query.value("tagID").toString(),
                                            query.value("orderNum").toInt()
            ));
        }
    }

    return results;
}

QList<todo::ItemDetailAndTag> todo::SQLDao::selectItemAndTagMatchByItemIDs(const QList<QString> &itemIDs) {
    QList<ItemDetailAndTag> results;
    if (itemIDs.isEmpty()) {
        return results;
    }

    QSqlQuery query(this->db);

//  Can't work !!! ?
//    query.prepare("SELECT itemID, tagID"
//                  " FROM item_tags"
//                  " WHERE itemID = ?;");
//    QVariantList ids;
//    for (auto const &itemID : itemIDs) {
//        ids.append(itemID);
//    }
//    query.addBindValue(ids);
    QString sqlStr("SELECT itemID, tagID, orderNum"
                   " FROM item_tags"
                   " WHERE itemID IN (%1);");
    QStringList ids;
    for (auto const &itemID : itemIDs) {
        ids.append(QString("\"%1\"").arg(itemID));
    }

    if (!query.exec(sqlStr.arg(ids.join(",")))) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            results.append(ItemDetailAndTag(query.value("itemID").toString(),
                                            query.value("tagID").toString(),
                                            query.value("orderNum").toInt()
            ));
        }
    }

    return results;
}

QList<todo::ItemDetailAndTag> todo::SQLDao::selectItemAndTagMatchByTagID(const QString &tagID) {
    QList<ItemDetailAndTag> results;
    QSqlQuery query(this->db);
    query.prepare("SELECT itemID, tagID, orderNum"
                  " FROM item_tags"
                  " WHERE tagID = :tagID;");
    query.bindValue(":tagID", tagID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            results.append(ItemDetailAndTag(query.value("itemID").toString(),
                                            query.value("tagID").toString(),
                                            query.value("orderNum").toInt()
            ));
        }
    }

    return results;
}

QList<todo::ItemDetailAndTag> todo::SQLDao::selectItemAndTagMatchByTagIDs(const QList<QString> &tagIDs) {
    QList<ItemDetailAndTag> results;
    QSqlQuery query(this->db);

    QStringList tagIDList;
    for (auto const &tagID : tagIDs) {
        tagIDList.append(QString("\"%1\"").arg(tagID));
    }
    QString tagIDListStr = tagIDList.join(",");

    query.prepare("SELECT itemID, tagID, orderNum"
                  " FROM item_tags"
                  " WHERE tagID IN (:tagIDs);");
    query.bindValue(":tagIDs", tagIDListStr);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            results.append(ItemDetailAndTag(query.value("itemID").toString(),
                                            query.value("tagID").toString(),
                                            query.value("orderNum").toInt()
            ));
        }
    }

    return results;
}

void todo::SQLDao::insertItemAndTagMatch(const todo::ItemDetailAndTag &newMatch) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO item_tags(itemID, tagID, orderNum)"
                  " VALUES(:itemID, :tagID, :orderNum);");
    query.bindValue(":itemID", newMatch.getItemID());
    query.bindValue(":tagID", newMatch.getTagID());
    query.bindValue(":orderNum", newMatch.getOrder());
    if (!query.exec()) {
        qDebug() << " ====== " << query.lastError().text().toStdString().c_str();
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemAndTagMatch(const todo::ItemDetailAndTag &match) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_tags"
                  " WHERE itemID=:itemID AND tagID=:tagID;");
    query.bindValue(":itemID", match.getItemID());
    query.bindValue(":tagID", match.getTagID());
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemAndTagMatchByItemID(const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_tags"
                  " WHERE itemID=:itemID;");
    query.bindValue(":itemID", itemID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemAndTagMatchByItemIDs(const QList<QString> &itemIDList) {
    if (itemIDList.isEmpty()) return;

    QString whereClauseStr = this->generateWhereClauseStrValues("itemID", "=", false, itemIDList);
    QString queryStr("DELETE FROM item_tags WHERE ");
    queryStr += whereClauseStr;
    QSqlQuery query(this->db);
    if (!query.exec(queryStr)) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemAndTagMatchByTagID(const QString &tagID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_tags"
                  " WHERE tagID=:tagID;");
    query.bindValue(":tagID", tagID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

todo::SQLDao::SQLDao(int type)
    : SQLDao() {
    if (type >= 0 and type <= 1) {
        this->sqlType = type;
    } else {
        this->sqlType = 1;
    }
}

QList<todo::ItemDetail> todo::SQLDao::selectNextNotifiedItemDetail() {
    QList<ItemDetail> result;
    QSqlQuery query(this->db);
    query.prepare("SELECT startTime, targetDate"
                  " FROM todo_items"
                  " WHERE targetDate >= :targetDate AND startTime >= :startTime"
                  " ORDER BY targetDate, startTime"
                  " LIMIT 1;");
    query.bindValue(":targetDate", QDate::currentDate());
    query.bindValue(":startTime", QTime::currentTime());
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }

    if (query.next()) {
        QTime startTime = query.value("startTime").toTime();
        QDate targetDate = query.value("targetDate").toDate();
        query.clear();
        query.prepare("SELECT id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType,"
                      " createdTime, lastUpdatedTime"
                      " FROM todo_items"
                      " WHERE startTime = :startTime AND targetDate = :targetDate;");
        query.bindValue(":startTime", startTime);
        query.bindValue(":targetDate", targetDate);
        if (!query.exec()) {
            throw SqlErrorException(query.lastError().text().toStdString().c_str());
        }

        while (query.next()) {
            ItemDetail detail;
            detail.setId(query.value("id").toString());
            detail.setTitle(query.value("title").toString());
            detail.setPriority(query.value("priority").toInt());
            detail.setDescription(query.value("description").toString());
            detail.setFromTime(query.value("startTime").toTime());
            detail.setToTime(query.value("endTime").toTime());
            detail.setDone(query.value("done").toBool());
            detail.setTargetDate(query.value("targetDate").toDate());
            detail.setMode(todo::ItemMode(query.value("type").toInt()));
            detail.setCreatedTime(query.value("createdTime").toDateTime());
            detail.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            // todo: parentID, freq, freqType in SELECT
            result.append(detail);
        }
    }

    return result;
}

QList<todo::ItemDetailDao> todo::SQLDao::selectItemDetailByDate(const QDate &fromDate, const QDate &toDate) {
    QList<ItemDetailDao> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType,"
                  " createdTime, lastUpdatedTime"
                  " FROM todo_items"
                  " WHERE targetDate >= :fromDate AND targetDate <= :toDate;");
    query.bindValue(":fromDate", fromDate);
    query.bindValue(":toDate", toDate);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemDetailDao detail;
            detail.setId(query.value("id").toString());
            detail.setTitle(query.value("title").toString());
            detail.setPriority(query.value("priority").toInt());
            detail.setDescription(query.value("description").toString());
            detail.setFromTime(query.value("startTime").toTime());
            detail.setToTime(query.value("endTime").toTime());
            detail.setDone(query.value("done").toBool());
            detail.setTargetDate(query.value("targetDate").toDate());
            detail.setMode(todo::ItemMode(query.value("type").toInt()));
            detail.setCreatedTime(query.value("createdTime").toDateTime());
            detail.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            // todo: parentID, freq, freqType in SELECT
            resultList.append(detail);
        }
    }

    return resultList;
}

QList<todo::ItemDetailDao> todo::SQLDao::selectItemDetailByIDs(const QList<QString> &itemIDs) {
    QList<ItemDetailDao> resultList;
    QSqlQuery query(this->db);
    QStringList itemIDList;
    for (auto const &itemID : itemIDs) {
        itemIDList.append(QString(" id = \"%1\"").arg(itemID));
    }
    QString itemIDListStr = itemIDList.join(" or ");
    if (itemIDListStr.isEmpty()) {
        return resultList;
    }

    QString queryStr("SELECT id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType,"
                  " createdTime, lastUpdatedTime"
                  " FROM todo_items"
                  " WHERE ");
    queryStr += itemIDListStr;
    if (!query.exec(queryStr)) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemDetailDao detail;
            detail.setId(query.value("id").toString());
            detail.setTitle(query.value("title").toString());
            detail.setPriority(query.value("priority").toInt());
            detail.setDescription(query.value("description").toString());
            detail.setFromTime(query.value("startTime").toTime());
            detail.setToTime(query.value("endTime").toTime());
            detail.setDone(query.value("done").toBool());
            detail.setTargetDate(query.value("targetDate").toDate());
            detail.setMode(todo::ItemMode(query.value("type").toInt()));
            detail.setCreatedTime(query.value("createdTime").toDateTime());
            detail.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            // todo: parentID, freq, freqType in SELECT
            resultList.append(detail);
        }
    }

    return resultList;
}

QList<todo::ItemGroupDao> todo::SQLDao::selectItemGroupByID(const QString &groupID) {
    QList<ItemGroupDao> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT id, title, description, fromDate, toDate, `type`, milestone, createdTime, lastUpdatedTime, color"
                  " FROM item_groups"
                  " WHERE id = :id");
    query.bindValue(":id", groupID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            todo::ItemGroupDao itemGroup;
            itemGroup.setId(query.value("id").toString());
            itemGroup.setTitle(query.value("title").toString());
            itemGroup.setDescription(query.value("description").toString());
            itemGroup.setFromDate(query.value("fromDate").toDate());
            itemGroup.setToDate(query.value("toDate").toDate());
            itemGroup.setType(todo::ItemGroupType(query.value("type").toInt()));
            itemGroup.setMileStone(query.value("milestone").toBool());
            itemGroup.setCreatedTime(query.value("createdTime").toDateTime());
            itemGroup.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            itemGroup.setColor(query.value("color").toString());
            resultList.append(itemGroup);
        }
    }
    return resultList;
}

QList<todo::ItemGroupDao> todo::SQLDao::selectItemGroupByType(const todo::ItemGroupType &type) {
    QList<ItemGroupDao> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT id, title, description, fromDate, toDate, `type`, milestone, createdTime, lastUpdatedTime, color"
                  " FROM item_groups"
                  " WHERE `type` = :type");
    query.bindValue(":type", type);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            todo::ItemGroupDao itemGroup;
            itemGroup.setId(query.value("id").toString());
            itemGroup.setTitle(query.value("title").toString());
            itemGroup.setDescription(query.value("description").toString());
            itemGroup.setFromDate(query.value("fromDate").toDate());
            itemGroup.setToDate(query.value("toDate").toDate());
            itemGroup.setType(todo::ItemGroupType(query.value("type").toInt()));
            itemGroup.setMileStone(query.value("milestone").toBool());
            itemGroup.setCreatedTime(query.value("createdTime").toDateTime());
            itemGroup.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            itemGroup.setColor(query.value("color").toString());
            resultList.append(itemGroup);
        }
    }
    return resultList;
}

void todo::SQLDao::updateItemGroupByID(const QString &groupID, const ItemGroupDao &itemGroupDao) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE item_groups"
                  " SET title=:title, description=:description, fromDate=:fromDate, toDate=:toDate,"
                  "     `type`=:type, milestone=:milestone,"
                  "     createdTime=:createdTime, lastUpdatedTime=:lastUpdatedTime, color=:color"
                  " WHERE id=:id");
    query.bindValue(":id", groupID);
    query.bindValue(":title", itemGroupDao.getTitle());
    query.bindValue(":description", itemGroupDao.getDescription());
    query.bindValue(":fromDate", itemGroupDao.getFromDate());
    query.bindValue(":toDate", itemGroupDao.getToDate());
    query.bindValue(":type", itemGroupDao.getType());
    query.bindValue(":milestone", itemGroupDao.isMileStone());
    query.bindValue(":createdTime", itemGroupDao.getCreatedTime());
    query.bindValue(":lastUpdatedTime", itemGroupDao.getLastUpdatedTime());
    query.bindValue(":color", itemGroupDao.getColor().name());

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemGroupByID(const QString &groupID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_groups"
                  " WHERE id=:id");
    query.bindValue(":id", groupID);

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemGroupByIDs(const QList<QString> &groupIDList) {
    QSqlQuery query(this->db);
    QStringList itemIDList;
    for (auto const &groupId : groupIDList) {
        itemIDList.append(QString(" id = \"%1\"").arg(groupId));
    }
    QString itemIDListStr = itemIDList.join(" or ");
    if (itemIDListStr.isEmpty()) {
        return;
    }

    QString queryStr("DELETE FROM item_groups WHERE ");
    queryStr += itemIDListStr;

    if (!query.exec(queryStr)) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::insertItemGroup(const ItemGroupDao &itemGroupDao) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO item_groups"
                  " (id, title, description, fromDate, toDate, `type`, milestone, createdTime, lastUpdatedTime, color)"
                  " VALUES(:id, :title, :description, :fromDate, :toDate, :type, :milestone,"
                  "        :createTime, :lastUpdatedTime, :color)");
    query.bindValue(":id", itemGroupDao.getId());
    query.bindValue(":title", itemGroupDao.getTitle());
    query.bindValue(":description", itemGroupDao.getDescription());
    query.bindValue(":fromDate", itemGroupDao.getFromDate());
    query.bindValue(":toDate", itemGroupDao.getToDate());
    query.bindValue(":type", itemGroupDao.getType());
    query.bindValue(":milestone", itemGroupDao.isMileStone());
    query.bindValue(":createdTime", itemGroupDao.getCreatedTime());
    query.bindValue(":lastUpdatedTime", itemGroupDao.getLastUpdatedTime());
    query.bindValue(":color", itemGroupDao.getColor().name());

    if (!query.exec()) {
        qDebug() << "insertItemGroup: exception happens";
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

QList<todo::ItemGroupDao> todo::SQLDao::selectItemGroupByIDs(const QList<QString> &groupIDs) {
    QList<ItemGroupDao> resultList;
    QSqlQuery query(this->db);
    QStringList itemIDList;
    for (auto const &groupId : groupIDs) {
        itemIDList.append(QString(" id = \"%1\"").arg(groupId));
    }
    QString itemIDListStr = itemIDList.join(" or ");
    if (itemIDListStr.isEmpty()) {
        return resultList;
    }

    QString queryStr("SELECT id, title, description, fromDate, toDate, `type`, milestone, createdTime, lastUpdatedTime, color"
              " FROM item_groups"
              " WHERE ");
    queryStr += itemIDListStr;
    if (!query.exec(queryStr)) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            todo::ItemGroupDao itemGroup;
            itemGroup.setId(query.value("id").toString());
            itemGroup.setTitle(query.value("title").toString());
            itemGroup.setDescription(query.value("description").toString());
            itemGroup.setFromDate(query.value("fromDate").toDate());
            itemGroup.setToDate(query.value("toDate").toDate());
            itemGroup.setType(todo::ItemGroupType(query.value("type").toInt()));
            itemGroup.setMileStone(query.value("milestone").toBool());
            itemGroup.setCreatedTime(query.value("createdTime").toDateTime());
            itemGroup.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            itemGroup.setColor(query.value("color").toString());
            resultList.append(itemGroup);
        }
    }
    return resultList;
}

QList<todo::ItemGroupRelation> todo::SQLDao::selectItemGroupRelationByRootID(const QString &rootID) {
    QList<ItemGroupRelation> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT rootGroupID, directGroupID, itemID"
                  " FROM item_group_relations"
                  " WHERE rootGroupID = :rootGroupID");
    query.bindValue(":rootGroupID", rootID);

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemGroupRelation relation;
            relation.setRootGroupID(query.value("rootGroupID").toString());
            relation.setDirectGroupID(query.value("directGroupID").toString());
            relation.setItemID(query.value("itemID").toString());
            resultList.append(relation);
        }
    }

    return resultList;
}

void
todo::SQLDao::deleteItemGroupRelationByDirectParentIDAndItemID(const QString &directParentID, const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_group_relations"
                  " WHERE directGroupID = :directGroupID AND itemID = :itemID");
    query.bindValue(":directGroupID", directParentID);
    query.bindValue(":itemID", itemID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemGroupRelationByRootID(const QString &rootID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_group_relations"
                  " WHERE rootGroupID = :rootGroupID");
    query.bindValue(":rootGroupID", rootID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::insertItemGroupRelation(const todo::ItemGroupRelation &relation) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO item_group_relations"
                  " (rootGroupID, directGroupID, itemID)"
                  " VALUES(:rootGroupID, :directGroupID, :itemID)");
    query.bindValue(":rootGroupID", relation.getRootGroupID());
    query.bindValue(":directGroupID", relation.getDirectGroupID());
    query.bindValue(":itemID", relation.getItemID());

    if (!query.exec()) {
        qDebug() << "insertItemGroupRelation: exception happens";
        qDebug() << relation.getRootGroupID() << relation.getDirectGroupID() << relation.getItemID();
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

QList<todo::ItemGroupRelation> todo::SQLDao::selectItemGroupRelationByParentID(const QString &parentID) {
    QList<ItemGroupRelation> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT rootGroupID, directGroupID, itemID"
                  " FROM item_group_relations"
                  " WHERE directGroupID = :directGroupID");
    query.bindValue(":directGroupID", parentID);

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemGroupRelation relation;
            relation.setRootGroupID(query.value("rootGroupID").toString());
            relation.setDirectGroupID(query.value("directGroupID").toString());
            relation.setItemID(query.value("itemID").toString());
            resultList.append(relation);
        }
    }

    return resultList;
}

QList<todo::ItemGroupRelation> todo::SQLDao::selectItemGroupRelationByItemID(const QString &itemID) {
    QList<ItemGroupRelation> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT rootGroupID, directGroupID, itemID"
                  " FROM item_group_relations"
                  " WHERE itemID = :itemID");
    query.bindValue(":itemID", itemID);

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemGroupRelation relation;
            relation.setRootGroupID(query.value("rootGroupID").toString());
            relation.setDirectGroupID(query.value("directGroupID").toString());
            relation.setItemID(query.value("itemID").toString());
            resultList.append(relation);
        }
    }

    return resultList;
}

void todo::SQLDao::deleteItemGroupRelationByItemID(const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_group_relations"
                  " WHERE itemID = :itemID");
    query.bindValue(":itemID", itemID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemGroupRelationByItemIDs(const QList<QString> &itemIDList) {
    if (itemIDList.isEmpty()) return;

    QString whereClauseStr = this->generateWhereClauseStrValues("itemID", "=", false, itemIDList);
    QString queryStr("DELETE FROM item_group_relations WHERE ");
    queryStr += whereClauseStr;

    QSqlQuery query(this->db);
    if (!query.exec(queryStr)) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

QList<todo::ItemGroupRelation> todo::SQLDao::selectAllItemGroupRelation() {
    QList<ItemGroupRelation> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT rootGroupID, directGroupID, itemID"
                  " FROM item_group_relations");

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemGroupRelation relation;
            relation.setRootGroupID(query.value("rootGroupID").toString());
            relation.setDirectGroupID(query.value("directGroupID").toString());
            relation.setItemID(query.value("itemID").toString());
            resultList.append(relation);
        }
    }

    return resultList;
}

QString todo::SQLDao::generateWhereClauseStrValues(const QString &fieldName, const QString &operation, bool isAnd,
                                          const QList<QString> &valueList) {
    QStringList itemIDList;
    for (auto const &valueStr : valueList) {
        itemIDList.append(QString(" %1 %2 \"%3\"").arg(fieldName).arg(operation).arg(valueStr));
    }

    if (isAnd) {
        return itemIDList.join(" AND ");
    } else {
        return itemIDList.join(" or ");
    }
}

void todo::SQLDao::startTransaction() {
    this->db.transaction();
}

void todo::SQLDao::endTransaction(bool isSuccessful) {
    if (isSuccessful) {
        this->db.commit();
    } else {
        this->db.rollback();
    }
}

QList<todo::StickyNoteDao> todo::SQLDao::selectStickyNoteById(const QString &id) {
    QList<StickyNoteDao> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT"
                  " id, name, shown, x, y, `type`, fontColor, backgroundColor, createdTime, lastUpdatedTime"
                  " FROM sticky_note"
                  " WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            StickyNoteDao dao;
            dao.setId(query.value("id").toString());
            dao.setName(query.value("name").toString());
            dao.setShown(query.value("shown").toBool());
            dao.setPos(QPoint(query.value("x").toInt(), query.value("y").toInt()));
            dao.setType(StickyNoteType(query.value("type").toInt()));
            dao.setFontColor(query.value("fontColor").toString());
            dao.setBackgroundColor(query.value("backgroundColor").toString());
            dao.setCreatedTime(query.value("createdTime").toDateTime());
            dao.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            resultList.append(dao);
        }
    }

    return resultList;
}

void todo::SQLDao::insertStickyNote(const todo::StickyNoteDao &stickyNoteDao) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO sticky_note"
                  " (id, name, shown, x, y, `type`, fontColor, backgroundColor, createdTime, lastUpdatedTime)"
                  " VALUES(:id, :name, :shown, :x, :y, :type, :fontColor, :bgColor, :createdTime, :lastUpdatedTime);");
    query.bindValue(":id", stickyNoteDao.getId());
    query.bindValue(":name", stickyNoteDao.getName());
    query.bindValue(":shown", stickyNoteDao.isShown());
    query.bindValue(":x", stickyNoteDao.getPos().x());
    query.bindValue(":y", stickyNoteDao.getPos().y());
    query.bindValue(":type", stickyNoteDao.getType().toTypeNum());
    query.bindValue(":fontColor", stickyNoteDao.getFontColor().name());
    query.bindValue(":bgColor", stickyNoteDao.getBackgroundColor().name());
    query.bindValue(":createdTime", stickyNoteDao.getCreatedTime());
    query.bindValue(":lastUpdatedTime", stickyNoteDao.getLastUpdatedTime());
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::updateStickyNoteById(const QString &id, const todo::StickyNoteDao &stickyNoteDao) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE sticky_note"
                  " SET name=:name, shown=:shown, x=:x, y=:y, `type`=:type, fontColor=:fontColor,"
                  "     backgroundColor=:bgColor, createdTime=:createdTime, lastUpdatedTime=:lastUpdatedTime"
                  " WHERE id=:id;");
    query.bindValue(":id", id);
    query.bindValue(":name", stickyNoteDao.getName());
    query.bindValue(":shown", stickyNoteDao.isShown());
    query.bindValue(":x", stickyNoteDao.getPos().x());
    query.bindValue(":y", stickyNoteDao.getPos().y());
    query.bindValue(":type", stickyNoteDao.getType().toTypeNum());
    query.bindValue(":fontColor", stickyNoteDao.getFontColor().name());
    query.bindValue(":bgColor", stickyNoteDao.getBackgroundColor().name());
    query.bindValue(":createdTime", stickyNoteDao.getCreatedTime());
    query.bindValue(":lastUpdatedTime", stickyNoteDao.getLastUpdatedTime());
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteStickyNoteById(const QString &id) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM sticky_note"
                  " WHERE id=:id;");
    query.bindValue(":id", id);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

QList<todo::StickyNoteDao> todo::SQLDao::selectAllStickyNote() {
    QList<StickyNoteDao> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT"
                  " id, name, shown, x, y, `type`, fontColor, backgroundColor, createdTime, lastUpdatedTime"
                  " FROM sticky_note");
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            StickyNoteDao dao;
            dao.setId(query.value("id").toString());
            dao.setName(query.value("name").toString());
            dao.setShown(query.value("shown").toBool());
            dao.setPos(QPoint(query.value("x").toInt(), query.value("y").toInt()));
            dao.setType(StickyNoteType(query.value("type").toInt()));
            dao.setFontColor(query.value("fontColor").toString());
            dao.setBackgroundColor(query.value("backgroundColor").toString());
            dao.setCreatedTime(query.value("createdTime").toDateTime());
            dao.setLastUpdatedTime(query.value("lastUpdatedTime").toDateTime());
            resultList.append(dao);
        }
    }

    return resultList;}

void todo::SQLDao::updateStickyNotePositionById(const QString &id, int x, int y) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE sticky_note"
                  " SET x=:x, y=:y"
                  " WHERE id=:id;");
    query.bindValue(":id", id);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

QList<todo::ItemDetailTimeDao> todo::SQLDao::selectItemDetailTimeByItemID(const QString &itemID) {
    QList<ItemDetailTimeDao> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT itemID, startTime, endTime"
                  " FROM todo_item_times"
                  " WHERE itemID = :itemID");
    query.bindValue(":itemID", itemID);
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemDetailTimeDao dao;
            dao.setItemID(query.value("itemID").toString());
            dao.setStartTime(query.value("startTime").toDateTime());
            dao.setEndTime(query.value("endTime").toDateTime());
            resultList.append(dao);
        }
    }

    return resultList;
}

void todo::SQLDao::insertItemDetailTime(const todo::ItemDetailTimeDao &dao) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO todo_item_times"
                  " (itemID, startTime, endTime)"
                  " VALUES(:itemID, :startTime, :endTime);");
    query.bindValue(":itemID", dao.getItemID());
    query.bindValue(":startTime", dao.getStartTime());
    query.bindValue(":endTime", dao.getEndTime());

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemDetailTimeByItemID(const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM todo_item_times"
                  " WHERE itemID = :itemID;");
    query.bindValue(":itemID", itemID);

    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

void todo::SQLDao::deleteItemDetailTimeByItemIDs(const QList<QString> &itemIDs) {
    if (itemIDs.empty()) return;

    QString queryStr("DELETE FROM todo_item_times WHERE ");
    QString whereClause = this->generateWhereClauseStrValues("itemID", "=", false, itemIDs);
    queryStr += whereClause;

    QSqlQuery query(this->db);
    if (!query.exec(queryStr)) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    }
}

QList<todo::ItemDetailTimeDao> todo::SQLDao::selectItemDetailTimeByTargetDate(const QDate &targetDate) {
    QList<ItemDetailTimeDao> resultList;
    QDateTime targetDateTime(targetDate);
    QSqlQuery query(this->db);
    query.prepare("SELECT itemID, startTime, endTime"
                  " FROM todo_item_times"
                  " WHERE startTime >= :fromTime and startTime < :toTime"
                  "    or endTime >= :fromTime and endTime < :toTime");
    query.bindValue(":fromTime", targetDateTime);
    query.bindValue(":toTime", targetDateTime.addDays(1));
    if (!query.exec()) {
        throw SqlErrorException(query.lastError().text().toStdString().c_str());
    } else {
        while (query.next()) {
            ItemDetailTimeDao dao;
            dao.setItemID(query.value("itemID").toString());
            dao.setStartTime(query.value("startTime").toDateTime());
            dao.setEndTime(query.value("endTime").toDateTime());
            resultList.append(dao);
        }
    }

    return resultList;
}
