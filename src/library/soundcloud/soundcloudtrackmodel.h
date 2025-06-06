#ifndef MIXXX_SOUNDCLOUDTRACKMODEL_H
#define MIXXX_SOUNDCLOUDTRACKMODEL_H

#include <QAbstractTableModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>

#include "library/trackmodel.h"

struct SoundCloudTrack {
    QString id;
    QString title;
    QString artist;
    QString streamUrl;
};

class SoundCloudTrackModel : public QAbstractTableModel, public TrackModel {
    Q_OBJECT
  public:
    explicit SoundCloudTrackModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    void search(const QString& searchText, const QString& extraFilter = QString()) override;
    const QString currentSearch() const override { return m_currentSearch; }

    TrackPointer getTrack(const QModelIndex& index) const override { Q_UNUSED(index); return TrackPointer(); }
    TrackPointer getTrackByRef(const TrackRef& trackRef) const override { Q_UNUSED(trackRef); return TrackPointer(); }
    QUrl getTrackUrl(const QModelIndex& index) const override;
    QString getTrackLocation(const QModelIndex& index) const override { Q_UNUSED(index); return QString(); }
    TrackId getTrackId(const QModelIndex& index) const override { Q_UNUSED(index); return TrackId(index.row()); }
    CoverInfo getCoverInfo(const QModelIndex& index) const override { Q_UNUSED(index); return CoverInfo(); }
    const QVector<int> getTrackRows(TrackId trackId) const override { Q_UNUSED(trackId); return {}; }
    bool isColumnInternal(int column) override { Q_UNUSED(column); return false; }
    bool isColumnHiddenByDefault(int column) override { Q_UNUSED(column); return false; }
    QString modelKey(bool noSearch) const override;
    SortColumnId sortColumnIdFromColumnIndex(int index) const override { Q_UNUSED(index); return SortColumnId::Invalid; }
    int columnIndexFromSortColumnId(TrackModel::SortColumnId sortColumn) const override { Q_UNUSED(sortColumn); return -1; }
    bool updateTrackGenre(Track* pTrack, const QString& genre) const override { Q_UNUSED(pTrack); Q_UNUSED(genre); return false; }
#if defined(__EXTRA_METADATA__)
    bool updateTrackMood(Track* pTrack, const QString& mood) const override { Q_UNUSED(pTrack); Q_UNUSED(mood); return false; }
#endif

  private slots:
    void replyFinished(QNetworkReply* reply);

  private:
    void fetch(const QString& query);

    QNetworkAccessManager m_network;
    QVector<SoundCloudTrack> m_tracks;
    QString m_currentSearch;
};

#endif // MIXXX_SOUNDCLOUDTRACKMODEL_H
