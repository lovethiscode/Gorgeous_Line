#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QFontDatabase>
#include "clipboard.h"
#include "clipboard_image_provider.h"
#include "filetype_image_provider.h"
#include "utils.h"
#include "model/sessionmodel.h"
#include "model/eventmodel.h"
#include "globalconfig.h"
#include "lineengine.h"
#include "httpuploadmanager.h"
#include "model/contactmodel.h"
#include "model/groupinvitedmodel.h"
#include "model/groupmodel.h"
#include "util/fileutil.h"
#include "enum_define.h"

#include <QUrl>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication ::setOrganizationName("NXChat");
    QApplication ::setApplicationName("NXChat");
    QApplication ::setApplicationDisplayName("NXChat");
    QApplication ::setApplicationVersion("1.0.0");



    QApplication  app(argc, argv);

    // Force the default universal QML style, notably prevents
    // KDE from hijacking base controls and messing up everything
    QQuickStyle::setStyle("Fusion");
    QQuickStyle::setFallbackStyle("Default");

    // Register default theme fonts. Take the files  from the
    // Qt resource system if possible (resources stored in the app executable),
    QList<QString> fontFamilies;
    fontFamilies << "roboto" << "hack";

    QList<QString> fontVariants;
    fontVariants << "regular" << "italic" << "bold" << "bold-italic";

    foreach (QString family, fontFamilies) {
        foreach (QString var, fontVariants) {
            QFontDatabase::addApplicationFont(
                "qrc:/resource/fonts/" + family + "/" + var + ".ttf"
            );
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/gui/application.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    // Register our custom non-visual QObject singletons,
    // that will be importable anywhere in QML. Example:
    //     import Clipboard 0.1
    //     ...
    //     Component.onCompleted: print(Clipboard.text)
    qmlRegisterSingletonType<Clipboard>(
        "Clipboard", 0, 1, "Clipboard",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(scriptEngine)

            Clipboard *clipboard = new Clipboard();

            // Register out custom image providers.
            // QML will be able to request an image from them by setting an
            // `Image`'s `source` to `image://<providerId>/<id>`
            engine->addImageProvider("clipboard", new ClipboardImageProvider(clipboard));
            engine->addImageProvider("icon", new FileTypeImageProvider);

            return clipboard;
        }
    );

    qmlRegisterSingletonType<Utils>(
        "CppUtils", 0, 1, "CppUtils",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return new Utils();
        }
    );

    qmlRegisterSingletonType<Utils>(
        "FileUtil", 0, 1, "FileUtil",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return new FileUtil();
        }
    );

    qmlRegisterType<SessionModel>("SessionModel", 0, 1, "SessionModel");   
    qmlRegisterType<EnumDefine>("EnumDefine", 0, 1, "EnumDefine");
    qmlRegisterType<ContactModel>("ContactModel", 0, 1, "ContactModel");
    qmlRegisterType<GroupModel>("GroupModel", 0, 1, "GroupModel");
    qmlRegisterType<GroupInvitedModel>("GroupInvitedModel", 0, 1, "GroupInvitedModel");


    qmlRegisterSingletonType<GlobalConfig>(
        "GlobalConfig", 0, 1, "GlobalConfig",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return &GlobalConfig::Instance();
        }
    );

    qmlRegisterSingletonType<GlobalConfig>(
        "HttpUploadManager", 0, 1, "HttpUploadManager",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return HttpUploadManager::Instance();
        }
    );

    qmlRegisterType<EventModel>("EventModel", 0, 1, "EventModel");

    qmlRegisterSingletonType<LineEngine>(
        "LineEngine", 0, 1, "LineEngine",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return LineEngine::Instance();
        }
    );


    engine.load(url);

    return app.exec();
}
