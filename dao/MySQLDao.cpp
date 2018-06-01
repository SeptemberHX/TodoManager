//
// Created by septemberhx on 5/17/18.
//

#include <QList>
#include <QDebug>
#include <QSqlQuery>
#include <QException>
#include <iostream>
#include "MySQLDao.h"
#include "../core/SqlErrorException.h"


QList<todo::ItemDetail> todo::MySQLDao::selectItemDetailByDate(const QDate &targetDate) {
    QList<ItemDetail> resultList;
    QSqlQuery query(this->db);
    query.prepare("SELECT id, title, priority, description, parentID, startTime, endTime, done, targetDate, `type`, freq, freqType,"
                  " createdTime, lastUpdatedTime"
                  " FROM todo_manager.todo_items"
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

void todo::MySQLDao::updateItemDetailByID(const QString &itemID, const todo::ItemDetail &itemDetail) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE todo_manager.todo_items"
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

void todo::MySQLDao::deleteItemDetailByID(const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM todo_manager.todo_items"
                  " WHERE id=:id;");
    query.bindValue(":id", itemID);

    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::MySQLDao::insertItemDetail(const todo::ItemDetail &itemDetail) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO todo_manager.todo_items"
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

todo::MySQLDao::~MySQLDao() {

}

void todo::MySQLDao::init() {
    qDebug() << "Init MySQLDao";
    this->db = QSqlDatabase::addDatabase("QMYSQL");
    this->db.setUserName("root");
    this->db.setPassword("");
    this->db.setDatabaseName("todo_manager");
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

todo::MySQLDao::MySQLDao() {

}

void todo::MySQLDao::createTables() {
    QStringList sqlScripts;
    sqlScripts.append("CREATE TABLE if not exists todo_manager.tags ("
                      "     name VARCHAR(255) NOT NULL,"
                      "     description text,"
                      "     color VARCHAR(7),"
                      "     PRIMARY KEY (name)"
                      ") DEFAULT CHARSET utf8;");
    sqlScripts.append("CREATE TABLE if not exists todo_manager.todo_items ("
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
                      ") DEFAULT CHARSET utf8;");
    sqlScripts.append("CREATE TABLE if not exists todo_manager.item_tags ("
                      "     itemID VARCHAR(255),"
                      "     tagID VARCHAR(255),"
                      "     foreign key(itemID) references todo_items(id),"
                      "     foreign key(tagID) references tags(name)"
                      ") DEFAULT CHARSET utf8;");

    QSqlQuery query(this->db);
    for (auto &sqlScript : sqlScripts) {
        if (!query.exec(sqlScript)) {
            qDebug() << QString("Failed to execute sql %1").arg(sqlScript);
            throw SqlErrorException();
        }
    }
}

void todo::MySQLDao::updateDoneByID(const QString &itemID, bool flag) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE todo_manager.todo_items"
                  " SET done=:done"
                  " WHERE id=:id;");
    query.bindValue(":id", itemID);
    query.bindValue(":done", flag);
    if (!query.exec()) {
        throw SqlErrorException();
    }
}

QList<todo::ItemTag> todo::MySQLDao::selectItemTagById(const QString &tagId) {
    QList<todo::ItemTag> resultLists;
    QSqlQuery query(this->db);
    query.prepare("SELECT name, description, color"
                  " FROM todo_manager.tags;"
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

void todo::MySQLDao::updateItemTagById(const QString &tagId, const ItemTag &itemTag) {
    QSqlQuery query(this->db);
    query.prepare("UPDATE todo_manager.tags"
                  " SET description=:description, color=:color"
                  " WHERE name=:name;");
    query.bindValue(":description", QVariant::String);
    query.bindValue(":color", itemTag.getColor().name());
    query.bindValue(":name", itemTag.getName());

    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::MySQLDao::deleteItemTagById(const QString &tagId) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM todo_manager.tags"
                  " WHERE name=:name;");
    query.bindValue(":name", tagId);

    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::MySQLDao::insertItemTag(const todo::ItemTag &tag) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO todo_manager.tags"
                  " (name, description, color)"
                  " VALUES(:name, :description, :color);");
    query.bindValue(":name", tag.getName());
    query.bindValue(":description", QVariant::String);
    query.bindValue(":color", tag.getColor().name());

    if (!query.exec()) {
        throw SqlErrorException();
    }
}

QList<todo::ItemTag> todo::MySQLDao::selectAllItemTag() {
    QList<todo::ItemTag> resultLists;
    QSqlQuery query(this->db);
    query.prepare("SELECT name, description, color"
                  " FROM todo_manager.tags;");
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

QList<todo::ItemDetailAndTag> todo::MySQLDao::selectItemAndTagMatchByItemID(const QString &itemID) {
    QList<ItemDetailAndTag> results;
    QSqlQuery query(this->db);
    query.prepare("SELECT itemID, tagID"
                  " FROM todo_manager.item_tags"
                  " WHERE itemID = :itemID;");
    query.bindValue(":itemID", itemID);
    if (!query.exec()) {
        throw SqlErrorException();
    } else {
        while (query.next()) {
            results.append(ItemDetailAndTag(query.value("itemID").toString(), query.value("tagID").toString()));
        }
    }

    return results;
}

QList<todo::ItemDetailAndTag> todo::MySQLDao::selectItemAndTagMatchByItemIDs(const QList<QString> &itemIDs) {
    QList<ItemDetailAndTag> results;
    if (itemIDs.isEmpty()) {
        return results;
    }

    QSqlQuery query(this->db);

//  Can't work !!! ?
//    query.prepare("SELECT itemID, tagID"
//                  " FROM todo_manager.item_tags"
//                  " WHERE itemID = ?;");
//    QVariantList ids;
//    for (auto const &itemID : itemIDs) {
//        ids.append(itemID);
//    }
//    query.addBindValue(ids);
    QString sqlStr("SELECT itemID, tagID"
                   " FROM todo_manager.item_tags"
                   " WHERE itemID IN (%1);");
    QStringList ids;
    for (auto const &itemID : itemIDs) {
        ids.append(QString("\"%1\"").arg(itemID));
    }

    if (!query.exec(sqlStr.arg(ids.join(",")))) {
        throw SqlErrorException();
    } else {
        while (query.next()) {
            results.append(ItemDetailAndTag(query.value("itemID").toString(), query.value("tagID").toString()));
        }
    }

    return results;
}

QList<todo::ItemDetailAndTag> todo::MySQLDao::selectItemAndTagMatchByTagID(const QString &tagID) {
    QList<ItemDetailAndTag> results;
    QSqlQuery query(this->db);
    query.prepare("SELECT itemID, tagID"
                  " FROM todo_manager.item_tags"
                  " WHERE tagID = :tagID;");
    query.bindValue(":tagID", tagID);
    if (!query.exec()) {
        throw SqlErrorException();
    } else {
        while (query.next()) {
            results.append(ItemDetailAndTag(query.value("itemID").toString(), query.value("tagID").toString()));
        }
    }

    return results;
}

QList<todo::ItemDetailAndTag> todo::MySQLDao::selectItemAndTagMatchByTagIDs(const QList<QString> &tagIDs) {
    QList<ItemDetailAndTag> results;
    QSqlQuery query(this->db);

    QStringList tagIDList;
    for (auto const &tagID : tagIDs) {
        tagIDList.append(QString("\"%1\"").arg(tagID));
    }
    QString tagIDListStr = tagIDList.join(",");

    query.prepare("SELECT itemID, tagID"
                  " FROM todo_manager.item_tags"
                  " WHERE tagID IN (:tagIDs);");
    query.bindValue(":tagIDs", tagIDListStr);
    if (!query.exec()) {
        throw SqlErrorException();
    } else {
        while (query.next()) {
            results.append(ItemDetailAndTag(query.value("itemID").toString(), query.value("tagID").toString()));
        }
    }

    return results;
}

void todo::MySQLDao::insertItemAndTagMatch(const todo::ItemDetailAndTag &newMatch) {
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO todo_manager.item_tags(itemID, tagID)"
                  " VALUES(:itemID, :tagID);");
    query.bindValue(":itemID", newMatch.getItemID());
    query.bindValue(":tagID", newMatch.getTagID());
    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::MySQLDao::deleteItemAndTagMatch(const todo::ItemDetailAndTag &match) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM todo_manager.item_tags"
                  " WHERE itemID=:itemID AND tagID=:tagID;");
    query.bindValue(":itemID", match.getItemID());
    query.bindValue(":tagID", match.getTagID());
    if (!query.exec()) {
        throw SqlErrorException();
    }
}

void todo::MySQLDao::deleteItemAndTagMatchByItemID(const QString &itemID) {
    QSqlQuery query(this->db);
    query.prepare("DELETE FROM todo_manager.item_tags"
                  " WHERE itemID=:itemID;");
    query.bindValue(":itemID", itemID);
    if (!query.exec()) {
        throw SqlErrorException();
    }
}
