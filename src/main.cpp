#include <QApplication>

#include "glwidget.h"

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setVersion(3,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    GLWidget glWidget;
    qreal pixelRatio = glWidget.devicePixelRatio();
    glWidget.resize(1000/pixelRatio,562/pixelRatio);
    glWidget.show();

    return a.exec();
}
