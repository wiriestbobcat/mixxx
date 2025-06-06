#ifndef MIXXX_SOUNDCLOUDFEATURE_H
#define MIXXX_SOUNDCLOUDFEATURE_H

#include "library/libraryfeature.h"
#include "library/treeitemmodel.h"
#include "library/soundcloud/soundcloudtrackmodel.h"
#include "util/parented_ptr.h"

class SoundCloudFeature : public LibraryFeature {
    Q_OBJECT
  public:
    SoundCloudFeature(Library* pLibrary, UserSettingsPointer pConfig);
    ~SoundCloudFeature() override = default;

    QVariant title() override { return QStringLiteral("SoundCloud"); }

    TreeItemModel* sidebarModel() const override { return m_pSidebarModel; }

  public slots:
    void activate() override;

  private:
    parented_ptr<TreeItemModel> m_pSidebarModel;
    parented_ptr<SoundCloudTrackModel> m_pModel;
};

#endif // MIXXX_SOUNDCLOUDFEATURE_H
