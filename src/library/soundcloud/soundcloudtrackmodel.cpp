#include "library/soundcloud/soundcloudtrackmodel.h"

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

#include "util/logger.h"

namespace {
const mixxx::Logger kLogger("SoundCloudTrackModel");
const QString kApiBase = QStringLiteral("https://api.soundcloud.com");
}

SoundCloudTrackModel::SoundCloudTrackModel(QObject* parent)
        : QAbstractTableModel(parent), TrackModel(QSqlDatabase(), "SoundCloud") {
}

int SoundCloudTrackModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_tracks.size();
}

int SoundCloudTrackModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 3; // Artist, Title, ID
}

QVariant SoundCloudTrackModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    const SoundCloudTrack& track = m_tracks.at(index.row());
    switch (index.column()) {
    case 0:
        return track.artist;
    case 1:
        return track.title;
    case 2:
        return track.id;
    default:
        return QVariant();
    }
}

QVariant SoundCloudTrackModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }
    switch (section) {
    case 0:
        return tr("Artist");
    case 1:
        return tr("Title");
    case 2:
        return tr("ID");
    default:
        return QVariant();
    }
}

void SoundCloudTrackModel::fetch(const QString& query) {
    m_tracks.clear();

    QUrl url(kApiBase + "/tracks");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("client_id", qgetenv("SOUNDCLOUD_CLIENT_ID"));
    urlQuery.addQueryItem("q", query);
    urlQuery.addQueryItem("limit", "50");
    url.setQuery(urlQuery);

    QNetworkReply* reply = m_network.get(QNetworkRequest(url));
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        kLogger.warning() << "SoundCloud request failed" << reply->errorString();
        reply->deleteLater();
        return;
    }

    const QByteArray data = reply->readAll();
    reply->deleteLater();

    const auto json = QJsonDocument::fromJson(data);
    if (!json.isArray()) {
        return;
    }

    const QJsonArray arr = json.array();
    m_tracks.reserve(arr.size());
    for (const QJsonValue& value : arr) {
        if (!value.isObject()) {
            continue;
        }
        QJsonObject obj = value.toObject();
        SoundCloudTrack track;
        track.id = QString::number(obj.value("id").toInt());
        track.title = obj.value("title").toString();
        track.artist = obj["user"].toObject().value("username").toString();
        track.streamUrl = obj.value("stream_url").toString();
        m_tracks.append(track);
    }
}

void SoundCloudTrackModel::search(const QString& searchText, const QString& extraFilter) {
    Q_UNUSED(extraFilter);
    m_currentSearch = searchText;
    fetch(searchText);
    beginResetModel();
    endResetModel();
}

QString SoundCloudTrackModel::modelKey(bool noSearch) const {
    if (noSearch) {
        return QStringLiteral("soundcloud:");
    }
    return QStringLiteral("soundcloud:") + m_currentSearch;
}

QUrl SoundCloudTrackModel::getTrackUrl(const QModelIndex& index) const {
    if (!index.isValid() || index.row() >= m_tracks.size()) {
        return QUrl();
    }
    return QUrl(m_tracks.at(index.row()).streamUrl);
}

void SoundCloudTrackModel::replyFinished(QNetworkReply* reply) {
    Q_UNUSED(reply);
}
