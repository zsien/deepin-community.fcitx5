#include "org_kde_plasma_window_management.h"
#include <cassert>
#include "org_kde_plasma_window.h"
namespace fcitx::wayland {
const struct org_kde_plasma_window_management_listener
    OrgKdePlasmaWindowManagement::listener = {
        [](void *data, org_kde_plasma_window_management *wldata,
           uint32_t state) {
            auto *obj = static_cast<OrgKdePlasmaWindowManagement *>(data);
            assert(*obj == wldata);
            { return obj->showDesktopChanged()(state); }
        },
        [](void *data, org_kde_plasma_window_management *wldata, uint32_t id) {
            auto *obj = static_cast<OrgKdePlasmaWindowManagement *>(data);
            assert(*obj == wldata);
            { return obj->window()(id); }
        },
        [](void *data, org_kde_plasma_window_management *wldata,
           wl_array *ids) {
            auto *obj = static_cast<OrgKdePlasmaWindowManagement *>(data);
            assert(*obj == wldata);
            { return obj->stackingOrderChanged()(ids); }
        },
        [](void *data, org_kde_plasma_window_management *wldata,
           const char *uuids) {
            auto *obj = static_cast<OrgKdePlasmaWindowManagement *>(data);
            assert(*obj == wldata);
            { return obj->stackingOrderUuidChanged()(uuids); }
        },
        [](void *data, org_kde_plasma_window_management *wldata, uint32_t id,
           const char *uuid) {
            auto *obj = static_cast<OrgKdePlasmaWindowManagement *>(data);
            assert(*obj == wldata);
            { return obj->windowWithUuid()(id, uuid); }
        },
};
OrgKdePlasmaWindowManagement::OrgKdePlasmaWindowManagement(
    org_kde_plasma_window_management *data)
    : version_(org_kde_plasma_window_management_get_version(data)),
      data_(data) {
    org_kde_plasma_window_management_set_user_data(*this, this);
    org_kde_plasma_window_management_add_listener(
        *this, &OrgKdePlasmaWindowManagement::listener, this);
}
void OrgKdePlasmaWindowManagement::destructor(
    org_kde_plasma_window_management *data) {
    { return org_kde_plasma_window_management_destroy(data); }
}
void OrgKdePlasmaWindowManagement::showDesktop(uint32_t state) {
    return org_kde_plasma_window_management_show_desktop(*this, state);
}
OrgKdePlasmaWindow *
OrgKdePlasmaWindowManagement::getWindow(uint32_t internalWindowId) {
    return new OrgKdePlasmaWindow(
        org_kde_plasma_window_management_get_window(*this, internalWindowId));
}
OrgKdePlasmaWindow *
OrgKdePlasmaWindowManagement::getWindowByUuid(const char *internalWindowUuid) {
    return new OrgKdePlasmaWindow(
        org_kde_plasma_window_management_get_window_by_uuid(
            *this, internalWindowUuid));
}
} // namespace fcitx::wayland
