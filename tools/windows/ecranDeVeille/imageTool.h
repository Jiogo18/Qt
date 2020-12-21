#ifndef IMAGETOOL_H
#define IMAGETOOL_H

#include <QImage>
#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>

namespace imageTool {
QImage tookScreenshot();
double difference(const QImage &img1, const QImage &img2);
bool nullImage(const QImage &img);
};

#endif // IMAGETOOL_H
