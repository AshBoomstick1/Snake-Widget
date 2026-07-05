#include "snake_widget.h"
#include <QtWidgets>
#include <QRegion>

int main(int argc, char *argv[])
{
    srand(time(0));

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Snake-Widget_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    auto w = new Snake_widget();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->resize(500, 500);
    w->show();
    w->start_game();


    return QApplication::exec();
}