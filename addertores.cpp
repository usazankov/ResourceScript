#include "addertores.h"

static QTextStream cout(stdout);
namespace
{
    //Конфигурации, для которых будем добавлять диалоги
    const QStringList list = {"240x320_VerixV_IG",
                              "320x240_Pax_IG",
                              "320x240_VerixV_IG",
                              "240x320_Pax_IG",
                              "800x480_Pax_IG",
                              "320x240_Pax_IG_D210E"};
}
AdderToRes::AdderToRes(const QString &srcfile,const QString &filePartXML, const QString &destfile, QObject *parent) :
    QObject(parent), src(srcfile), partXml(filePartXML), dest(destfile)
{
#ifndef QT_NO_TEXTCODEC
    cout.setCodec("csIBM866");
#endif
}

bool AdderToRes::process()
{
    //Сначала парсим то, что хотим вставить
    QDomDocument domDoc;
    QFile file(partXml);
    if(file.open(QIODevice::ReadOnly))
    {
        if(domDoc.setContent(&file))
        {
            elementXml = domDoc.documentElement();
            qDebug() << elementXml.nodeName();
            qDebug() << elementXml.attribute("xsi:type");
        }
    }
    file.close();
    //Теперь заходим в Unipos.xml
    file.setFileName(src);
    if(file.open(QIODevice::ReadOnly))
    {
        if(domDoc.setContent(&file))
        {
            QDomElement domElement = domDoc.documentElement();

            //Находим Section Dialogs
            if(findSectionNode(domElement, "Dialogs"))
            {
               qDebug() << section.attribute("Name");
               //Section Dialog нашли? Ищем Тэг Configurations
               //Первый элемент Handlers
               domElement = section.firstChildElement();
               qDebug() << domElement.nodeName();
               //Второй Configurations
               domElement = domElement.nextSiblingElement();
               if(domElement.nodeName() == "Configurations")
               {
                    qDebug() << domElement.nodeName();
                    domElement = domElement.firstChildElement();
                    qDebug() << "Configurations to add:\n";
                    while(!domElement.isNull())
                    {
                        //qDebug() << domElement.attribute("Name");
                        if(list.contains(domElement.attribute("Name")))
                        {
                            qDebug() << domElement.attribute("Name");
                            if(addPartXmlToConfiguration(domElement))
                            {
                                qDebug() << "SUCCESS\n";
                            }
                            else
                            {
                                qDebug() << "ERROR\n";
                            }
                        }
                        domElement = domElement.nextSiblingElement();
                    }
                    return 1;
               }
               else
               {
                   assert(0);
                   error();
               }
            }
            else
            {
                assert(0);
                error();
            }
        }
        file.close();
    }
    else
    {
        qDebug() << file.errorString();
    }
}

bool AdderToRes::findSectionNode(const QDomNode& node ,const QString& value)
{
    if(node.isElement())
    {
        QDomElement element = node.toElement();
        if(element.tagName() == "Section" && element.attribute("Name") == value)
        {
            section = element;
            return true;
        }
    }
    QDomNode siblingNode = node.nextSiblingElement();
    while(!siblingNode.isNull()){
        bool res = findSectionNode( siblingNode, value);
        if(res)
            return true;
        siblingNode = siblingNode.nextSiblingElement();
    }

    QDomNode childNode = node.firstChild();
    if(!childNode.isNull()){
        bool res = findSectionNode( childNode, value);
        if(res)
            return true;
    }
    return false;
}

bool AdderToRes::addPartXmlToConfiguration(const QDomElement &element)
{
    QDomNode el = element;
    QDomNode dialog;
    if(el.nodeName() != "Configuration")
    {
        return false;
    }
    //Спускаемся до Languages
    el = el.firstChild();
    //Затем до Language
    el = el.firstChild();
    //Дальше до Messages
    el = el.firstChild();
    //И вот дошли до первого диалога
    el = el.firstChild();
    dialog = el;
    while(!dialog.isNull())
    {
        qDebug() << dialog.toElement().attribute("ID");
        el = dialog.firstChild();
        //Находим тэг DialogControls
        while(!el.isNull() && el.nodeName() != "DialogControls")
        {
            el = el.nextSiblingElement();
        }
        if(el.nodeName() == "DialogControls")
        {
            qDebug() << "OK";
            el = el.appendChild(elementXml);
        }
        dialog = dialog.nextSibling();
    }
    return false;
}

void AdderToRes::error()
{
    cout << "Ошибка! Структура xml файла не подходит под программу!";
    exit(-1);
}
