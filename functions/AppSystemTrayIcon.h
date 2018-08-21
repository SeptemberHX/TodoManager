//
// Created by septemberhx on 8/21/18.
//

#ifndef TODOMANAGER_APPSYSTEMTRAYICON_H
#define TODOMANAGER_APPSYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QString>
#include <QObject>

class AppSystemTrayIcon : public QObject {
    Q_OBJECT
public:
    static AppSystemTrayIcon *getInstance();
    void init(QWidget *parent = nullptr);
    ~AppSystemTrayIcon();
    void hide();
    void showMessage(const QString &titleStr, const QString &bodyStr);

signals:
    void activated();
    void quitClicked();

private:
    static AppSystemTrayIcon *instancePtr;

    AppSystemTrayIcon() = default;
    QSystemTrayIcon *trayIcon;

private slots:
    void quit_clicked();
};


#endif //TODOMANAGER_APPSYSTEMTRAYICON_H
