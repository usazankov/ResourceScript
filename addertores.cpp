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
    QDomDocument domDoc_part;
    QFile file(partXml);
    if(file.open(QIODevice::ReadOnly))
    {
        if(domDoc_part.setContent(&file))
        {
            elementXml = domDoc_part.documentElement();
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
               qDebug() << section.toElement().attribute("Name");
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
                            //domElement.appendChild(elementXml.cloneNode());
                            qDebug() << domElement.attribute("Name");
                            QDomElement conf = domElement;
                            if(addPartXmlToConfiguration(conf))
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
                    saveToFile(domDoc);
                    return 1;//saveToFile(domDoc);
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

bool AdderToRes::findSectionNode(QDomNode& node ,const QString& value)
{
    if(node.isElement())
    {
        QDomElement element = node.toElement();
        if(element.tagName() == "Section" && element.attribute("Name") == value)
        {
            section = node;
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

bool AdderToRes::addPartXmlToConfiguration(QDomNode &el)
{
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
            el.appendChild(elementXml.cloneNode());
        }else
        {
            return false;
        }
        dialog = dialog.nextSibling();
    }
    return true;
}

void AdderToRes::error()
{
    cout << "Ошибка! Структура xml файла не подходит под программу!";
    exit(-1);
}

bool AdderToRes::saveToFile(const QDomDocument &doc)
{
    QFile outFile( "simple-modified.xml" );
    if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        qDebug( "Failed to open file for writing." );
        return 0;
    }
    QTextStream stream( &outFile );
    stream.setCodec("UTF-8");
    stream.setGenerateByteOrderMark(true);
    stream << doc.toString();
    outFile.close();
}
