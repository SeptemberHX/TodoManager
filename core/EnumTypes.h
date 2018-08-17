//
// Created by septemberhx on 8/17/18.
//

#ifndef TODOMANAGER_ENUMTYPES_H
#define TODOMANAGER_ENUMTYPES_H

#include <QString>
#include <QMap>


namespace todo {

template <typename T> class map_init_helper {
public:
    explicit map_init_helper(T& d) : data(d) { }
    map_init_helper& operator() (typename T::key_type const &key, typename T::mapped_type const &value) {
        data[key] = value;
        return *this;
    }
private:
    T& data;
};

template <typename T> map_init_helper<T> map_init(T& item) {
    return map_init_helper<T>(item);
}

class StickyNoteType {
public:
    const static QString PROJECT;
    const static QString DAILY;
    const static QString TAG;
    const static QString CUSTOM;

    StickyNoteType(const QString &str);
    StickyNoteType(int typeNum);
    StickyNoteType();

    bool operator==(const StickyNoteType &other) const;
    bool operator==(const QString &str) const;
    int toTypeNum() const;
    QString toString() const;

private:
    static QMap<int, QString> type2StrMap;
    static QMap<int, QString> generateType2StrMap();

    int fromStr2TypeNum(const QString &str) const;
    int typeNum;
};


// todo: add a base type class which provides prefix & typeStrList
//       then every class inherited from it can be registered by CommonType
//       and we can use template function to get prefix.
class CommonType {
public:
    CommonType();
    void registerType(const QString &prefix, const QStringList &typeStrList);
private:
    static int currTypeNum;
    static QMap<int, QString> type2StrMap;
};

}


#endif //TODOMANAGER_ENUMTYPES_H
