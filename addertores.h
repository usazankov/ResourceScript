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
    explicit AdderToRes(const QString &srcfile, const QString &filePartXML, const QString &destfile, QObject *parent = nullptr);
    bool process();
signals:

public slots:

private:
    bool findSectionNode(const QDomNode& node, const QString& value);
    bool addPartXmlToConfiguration(const QDomElement &element);
    void error();
    QString src;
    QString dest;
    QString partXml;
    QDomElement elementXml;//То, что будем впихивать в каждый диалог
    QDomElement section;
};

#endif // ADDERTORES_H
