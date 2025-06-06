#include "library/soundcloud/soundcloudfeature.h"

#include "controllers/keyboard/keyboardeventfilter.h"
#include "library/library.h"
#include "moc_soundcloudfeature.cpp"
#include "util/logger.h"
#include "widget/wlibrary.h"

namespace {
const mixxx::Logger kLogger("SoundCloudFeature");
}

SoundCloudFeature::SoundCloudFeature(Library* pLibrary, UserSettingsPointer pConfig)
        : LibraryFeature(pLibrary, pConfig, QStringLiteral("soundcloud")),
          m_pSidebarModel(make_parented<TreeItemModel>(this)),
          m_pModel(make_parented<SoundCloudTrackModel>(this)) {
    // Sidebar root item
    auto* root = new TreeItem(tr("SoundCloud"));
    m_pSidebarModel->setRootItem(root);
}

void SoundCloudFeature::activate() {
    kLogger.info() << "Activating SoundCloud feature";
    emit showTrackModel(m_pModel);
}
