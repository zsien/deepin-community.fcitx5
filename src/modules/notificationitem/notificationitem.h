/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _FCITX_MODULES_NOTIFICATIONITEM_NOTIFICATIONITEM_H_
#define _FCITX_MODULES_NOTIFICATIONITEM_NOTIFICATIONITEM_H_

#include <memory>
#include <fcitx/addonmanager.h>
#include "fcitx-config/configuration.h"
#include "fcitx-config/iniparser.h"
#include "fcitx-utils/dbus/servicewatcher.h"
#include "fcitx-utils/i18n.h"
#include "fcitx/addoninstance.h"
#include "fcitx/instance.h"
#include "dbus_public.h"
#include "notificationitem_public.h"

namespace fcitx {

class StatusNotifierItem;
class DBusMenu;

FCITX_CONFIGURATION(NotificationItemConfig,
                    Option<bool> useInputGroup{this, "UseInputGroup",
                                               _("Use InputGroup"), false};
                    Option<bool> useRestart{this, "UseRestart",
                                            _("Use Restart"), true};
                    Option<bool> useExit{this, "UseExit", _("Use Exit"),
                                         false};);

class NotificationItem : public AddonInstance {
public:
    NotificationItem(Instance *instance);
    ~NotificationItem();

    dbus::Bus *globalBus();
    Instance *instance() { return instance_; }

    const NotificationItemConfig &config() const { return config_; }
    const Configuration *getConfig() const override { return &config_; }
    void setConfig(const RawConfig &config) override {
        config_.load(config, true);
        safeSaveAsIni(config_, "conf/notificationitem.conf");
    }
    void reloadConfig() override;
    void setSerivceName(const std::string &newName);
    void setRegistered(bool);
    void registerSNI();
    void enable();
    void disable();
    bool registered() const { return registered_; }
    std::unique_ptr<HandlerTableEntry<NotificationItemCallback>>
    watch(NotificationItemCallback callback);
    void newIcon();
    FCITX_ADDON_DEPENDENCY_LOADER(classicui, instance_->addonManager());

private:
    FCITX_ADDON_DEPENDENCY_LOADER(dbus, instance_->addonManager());
    FCITX_ADDON_EXPORT_FUNCTION(NotificationItem, enable);
    FCITX_ADDON_EXPORT_FUNCTION(NotificationItem, disable);
    FCITX_ADDON_EXPORT_FUNCTION(NotificationItem, watch);
    FCITX_ADDON_EXPORT_FUNCTION(NotificationItem, registered);
    NotificationItemConfig config_;
    Instance *instance_;
    std::unique_ptr<dbus::ServiceWatcher> watcher_;
    std::unique_ptr<dbus::Bus> privateBus_;
    std::unique_ptr<StatusNotifierItem> sni_;
    std::unique_ptr<DBusMenu> menu_;
    std::unique_ptr<dbus::ServiceWatcherEntry> watcherEntry_;
    std::vector<std::unique_ptr<HandlerTableEntry<EventHandler>>>
        eventHandlers_;
    std::unique_ptr<dbus::Slot> pendingRegisterCall_;
    std::string sniWatcherName_;
    int index_ = 0;
    std::string serviceName_;
    bool enabled_ = false;
    bool registered_ = false;
    std::unique_ptr<EventSourceTime> scheduleRegister_;
    HandlerTable<NotificationItemCallback> handlers_;
};

} // namespace fcitx

#endif // _FCITX_MODULES_NOTIFICATIONITEM_NOTIFICATIONITEM_H_
