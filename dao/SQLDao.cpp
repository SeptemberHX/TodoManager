//
// Created by septemberhx on 5/17/18.
//

#include <QList>
#include <QDebug>
#include <QSqlQuery>
#include <QException>
#include <iostream>
#include "SQLDao.h"
#include "../core/SqlErrorException.h"
#include "../config/TodoConfig.h"


QList<todo::ItemDetail> todo::SQLDao::selectItemDetailByDate(const QDate &targetDate) {
    QList<ItemDetail> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType,"
                  " createdTime, lastUpdatedTime"
                  " FROM todo_items"
                  " WHERE targetDate = :targetDate");
    query.bindValue(":targetDate", targetDate);
    if (!query.exec()) {
        throw SqlErrorException();
    } else {
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
            resultList.append(detail);
        }
    }

    return resultList;
}

void todo::SQLDao::updateItemDetailByID(const QString &itemID, const todo::ItemDetail &itemDetail) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE todo_items"
                  " SET title=:title, priority=:priority, description=:description, parentID=:parentID,"
                  "     startTime=:startTime, endTime=:endTime, done=:done, targetDate=:targetDate,"
                  "     `type`=:type, freq=:freq, freqType=:freqType, createdTime=:createdTime, lastUpdatedTime=:lastUpdatedTime"
                  " WHERE id=:id;");
    query.bindValue(":id", itemDetail.getId());
    query.bindValue(":title", itemDetail.getTitle());
    query.bindValue(":priority", itemDetail.getPriority());
    query.bindValue(":description", itemDetail.getDescription());
    query.bindValue(":parentID", QVariant::Int);
    query.bindValue(":startTime", itemDetail.getFromTime());
    query.bindValue(":endTime", itemDetail.getToTime());
    query.bindValue(":done", itemDetail.isDone());
    query.bindValue(":targetDate", itemDetail.getTargetDate());
    query.bindValue(":type", itemDetail.getMode());
    query.bindValue(":freq", QVariant::Int);
    query.bindValue(":freqType", QVariant::Int);
    query.bindValue(":createdTime", itemDetail.getCreatedTime());
    query.bindValue(":lastUpdatedTime", itemDetail.getLastUpdatedTime());
    // todo: parentID, freq, freqType in UPDATE

    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::SQLDao::deleteItemDetailByID(const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM todo_items"
                  " WHERE id=:id;");
    query.bindValue(":id", itemID);

    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::SQLDao::insertItemDetail(const todo::ItemDetail &itemDetail) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO todo_items"
                  " (id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType, createdTime, lastUpdatedTime)"
                  " VALUES(:id, :title, :priority, :description, :parentID, :startTime, :endTime, :done, :targetDate, :type, :freq, :freqType, :createdTime, :lastUpdatedTime);");
    query.bindValue(":id", itemDetail.getId());
    query.bindValue(":title", itemDetail.getTitle());
    query.bindValue(":priority", itemDetail.getPriority());
    query.bindValue(":description", itemDetail.getDescription());
    query.bindValue(":parentID", QVariant::Int);
    query.bindValue(":startTime", itemDetail.getFromTime());
    query.bindValue(":endTime", itemDetail.getToTime());
    query.bindValue(":done", itemDetail.isDone());
    query.bindValue(":targetDate", itemDetail.getTargetDate());
    query.bindValue(":type", itemDetail.getMode());
    query.bindValue(":freq", QVariant::Int);
    query.bindValue(":freqType", QVariant::Int);
    query.bindValue(":createdTime", itemDetail.getCreatedTime());
    query.bindValue(":lastUpdatedTime", itemDetail.getLastUpdatedTime());
    // todo: parentID, freq, freqType in INSERT
    if (!query.exec()) {
        throw SqlErrorException();
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
        throw SqlErrorException();
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
                      "     id VARCHAR(50) NOT NULL,"
                      "     title VARCHAR(255) NOT NULL,"
                      "     priority INTEGER NOT NULL,"
                      "     description text,"
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

    QSqlQuery query(this->db);
    for (auto &sqlScript : sqlScripts) {
        if (!query.exec(sqlScript)) {
            qDebug() << QString("Failed to execute sql %1").arg(sqlScript);
            throw SqlErrorException();
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
        throw SqlErrorException();
    }
}

QList<todo::ItemTag> todo::SQLDao::selectItemTagById(const QString &tagId) {
    QList<todo::ItemTag> resultLists;
    QSqlQuery query(this->db);
    query.prepare("SELECT name, description, color"
                  " FROM tags;"
                  " WHERE name = :tagId");
    query.bindValue(":tagId", tagId);
    if (!query.exec()) {
        throw SqlErrorException();
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
        throw SqlErrorException();
    }
}

void todo::SQLDao::deleteItemTagById(const QString &tagId) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM tags"
                  " WHERE name=:name;");
    query.bindValue(":name", tagId);

    if (!query.exec()) {
        throw SqlErrorException();
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
        throw SqlErrorException();
    }
}

QList<todo::ItemTag> todo::SQLDao::selectAllItemTag() {
    QList<todo::ItemTag> resultLists;
    QSqlQuery query(this->db);
    query.prepare("SELECT name, description, color"
                  " FROM tags;");
    if (!query.exec()) {
        throw SqlErrorException();
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
        throw SqlErrorException();
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
        throw SqlErrorException();
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
        throw SqlErrorException();
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
        throw SqlErrorException();
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
    query.prepare("INSERT INTO item_tags(itemID, tagID)"
                  " VALUES(:itemID, :tagID);");
    query.bindValue(":itemID", newMatch.getItemID());
    query.bindValue(":tagID", newMatch.getTagID());
    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::SQLDao::deleteItemAndTagMatch(const todo::ItemDetailAndTag &match) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_tags"
                  " WHERE itemID=:itemID AND tagID=:tagID;");
    query.bindValue(":itemID", match.getItemID());
    query.bindValue(":tagID", match.getTagID());
    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::SQLDao::deleteItemAndTagMatchByItemID(const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM item_tags"
                  " WHERE itemID=:itemID;");
    query.bindValue(":itemID", itemID);
    if (!query.exec()) {
        throw SqlErrorException();
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
        throw SqlErrorException();
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
            throw SqlErrorException();
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

QList<todo::ItemDetail> todo::SQLDao::selectItemDetailByDate(const QDate &fromDate, const QDate &toDate) {
    QList<ItemDetail> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType,"
                  " createdTime, lastUpdatedTime"
                  " FROM todo_items"
                  " WHERE targetDate >= :fromDate AND targetDate <= :toDate;");
    query.bindValue(":fromDate", fromDate);
    query.bindValue(":toDate", toDate);
    if (!query.exec()) {
        throw SqlErrorException();
    } else {
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
            resultList.append(detail);
        }
    }

    return resultList;
}
