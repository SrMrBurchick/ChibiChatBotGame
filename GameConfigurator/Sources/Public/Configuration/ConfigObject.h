#include <QJsonDocument>
#include <QMultiMap>
#include <QString>

struct SpriteSize {
    int Height;
    int Width;
};

struct TableSize {
    int Columns;
    int Rows;
};

struct AnimationSpriteInfo {
    uint SpriteId;
    int Column;
    int Row;
};

struct SystemConfig {
    QString ImagePath;
    QString ChatBotWebSockURL;
    int ChatBotWebSockPort;
};

class ConfigObject {
public:
    void ParseJsonDocument(const QJsonDocument& JsonDocument);
    void SaveConfigToFile(const QString& ConfigFile);

protected:
    QString SpriteSheetPath;
    SpriteSize SpriteSettings;
    TableSize TableSettings;
    SystemConfig SystemSettings;

    // List of the animations
    QMultiMap<QString, AnimationSpriteInfo> AnimationsMap;
};
