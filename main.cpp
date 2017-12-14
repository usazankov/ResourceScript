#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTextStream>
#include "addertores.h"

static QTextStream cout(stdout);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
#ifndef QT_NO_TEXTCODEC
    cout.setCodec("csIBM866");
#endif
    QCoreApplication::setApplicationName("ResourceScript");
    QCoreApplication::setApplicationVersion("0.1");
    QString descr("ResourceScript помогает работать с файлом ресурсов UNIPOS");
    QCommandLineParser parser;
    parser.setApplicationDescription(descr);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file unipos.xml"));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination file."));
    QCommandLineOption commandOption(QStringList() << "c" << "command", "command",
                                     "command");
    parser.addOption(commandOption);
    parser.process(app);
    const QStringList positionalArguments = parser.positionalArguments();

    if (positionalArguments.isEmpty())
    {
        cout << parser.applicationDescription();
        return 0;
    }

    if(positionalArguments.count() > 3)
    {
        cout << QString("Слишком много аргументов\n");
        cout << parser.applicationDescription();
        return 0;
    }
    else if(positionalArguments.count() <= 2 )
    {
        cout << QString("Слишком мало аргументов\n");
        cout << parser.applicationDescription();
        return 0;
    }
    if(positionalArguments.count() == 3)
    {
        cout << positionalArguments.at(0) << QString('\n');
        cout << positionalArguments.at(1) << QString('\n');
        cout << positionalArguments.at(2) << QString('\n');
        AdderToRes adder(positionalArguments.at(0), positionalArguments.at(1), positionalArguments.at(2));
        adder.process();
    }
    return 1;
}
