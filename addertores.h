#ifndef ADDERTORES_H
#define ADDERTORES_H

#include <QObject>
#include <QtXml>
#include <QFile>
#include <assert.h>

class AdderToRes : public QObject
{
    Q_OBJECT
public:
    explicit AdderToRes(const QString &srcfile, const QString &filePartXML, QObject *parent = nullptr);
    bool process();
signals:

public slots:

private:
    bool findSectionNode(QDomNode &node, const QString& value);
    bool addPartXmlToConfiguration(QDomNode &element);
    void error();
    bool saveToFile(const QDomDocument& doc);
    QString src;
    QString partXml;
    QDomElement elementXml;//То, что будем впихивать в каждый диалог
    QDomNode section;
};

#endif // ADDERTORES_H
