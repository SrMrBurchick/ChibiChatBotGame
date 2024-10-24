#include "System/FontSelector.h"
#include "System/Logger.h"

#include <QFontDatabase>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

QStringList CBFontSelector::getSystemFonts() {
    return QFontDatabase::families();
}

QFileInfo GetFontPathInternal(const QString &fontName, const QDir Dir)
{
    QStringList FontExtensions = {"*.ttf", "*.otf", "*.ttc", "*.pfa", "*.pfb"};
    if (Dir.exists()) {
        for (const QFileInfo& FileInfo : Dir.entryInfoList(FontExtensions, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks)) {
            int FontID = QFontDatabase::addApplicationFont(FileInfo.absoluteFilePath());

            if (FontID != -1) {
                QStringList Families = QFontDatabase::applicationFontFamilies(FontID);
                if (Families.contains(fontName, Qt::CaseSensitive)) {
                    return FileInfo;
                }
            }
        }

        for (const QFileInfo& FileInfo : Dir.entryInfoList(FontExtensions, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks)) {
            QFileInfo Result = GetFontPathInternal(fontName, FileInfo.absoluteFilePath());
            if (Result.exists()) {
                return Result;
            }
        }

    }

    return QFileInfo();
}

QFileInfo CBFontSelector::GetFontPath(const QString &fontName)
{
    for (const QString& FontDir : QStandardPaths::standardLocations(QStandardPaths::FontsLocation)) {
        QFileInfo Result = GetFontPathInternal(fontName, FontDir);
        if (Result.exists()) {
            return Result;
        }
    }
    return QFileInfo();
}


bool CBFontSelector::CopyFontToLocalDir(const QString &fontName, const QString &destinationDir) {
    QString FontPath = GetFontPath(fontName).absoluteFilePath();

    QDir dir(destinationDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (FontPath.isEmpty()) {
        LOG_WARNING("Failed to locate font %s", fontName.toStdString().c_str());
        return false;
    }

    QFile fontFile(FontPath);
    QString destinationPath = dir.filePath(QFileInfo(fontFile).fileName());

    QFile TargetPath(destinationPath);
    if (TargetPath.exists()) {
        return true;
    }

    return fontFile.copy(destinationPath);
}
