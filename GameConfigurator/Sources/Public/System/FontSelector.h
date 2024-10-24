#pragma once

#include <QDir>
#include <QFile>
#include <QFontDatabase>
#include <QObject>

class CBFontSelector : public QObject {
    Q_OBJECT

public:
    // === C++
    explicit CBFontSelector(QObject *parent = nullptr)
        : QObject(parent) {}
    static bool CopyFontToLocalDir(const QString &fontName, const QString &destinationDir);
    static QFileInfo GetFontPath(const QString &fontName);

    // ==== QML
    Q_INVOKABLE QStringList getSystemFonts();
};
