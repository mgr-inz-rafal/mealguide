#include "app_main.h"
#include "uib_app_manager.h"

/* app event callbacks */
static widget_class_h widget_app_create(void *user_data);
static void widget_app_terminate(void *user_data);
static int _on_create_cb(widget_context_h context, bundle *content, int w, int h, void *user_data);
static int _on_destroy_cb(widget_context_h context, widget_app_destroy_type_e reason, bundle *content, void *user_data);
static int _on_resume_cb(widget_context_h context, void *user_data);
static int _on_pause_cb(widget_context_h context, void *user_data);
static int _on_update_cb(widget_context_h context, bundle *content, int force, void *user_data);
static int _on_update_resize(widget_context_h context, int w, int h, void *user_data);
static void _on_low_memory_cb(app_event_info_h event_info, void *user_data);
static void _on_low_battery_cb(app_event_info_h event_info, void *user_data);
static void _on_device_orientation_cb(app_event_info_h event_info,
		void *user_data);
static void _on_language_changed_cb(app_event_info_h event_info,
		void *user_data);
static void _on_region_format_changed_cb(app_event_info_h event_info,
		void *user_data);

void nf_hw_back_cb(void* param, Evas_Object * evas_obj, void* event_info) {
	//TODO : user define code
	evas_obj = uib_views_get_instance()->get_window_obj()->app_naviframe;
	elm_naviframe_item_pop(evas_obj);
}

void win_del_request_cb(void *data, Evas_Object *obj, void *event_info) {
	ui_app_exit();
}

Eina_Bool nf_root_it_pop_cb(void* elm_win, Elm_Object_Item *it) {
	elm_win_lower(elm_win);
	return EINA_FALSE;
}

app_data *uib_app_create() {
	return calloc(1, sizeof(app_data));
}

void uib_app_destroy(app_data *user_data) {
	uib_app_manager_get_instance()->free_all_view_context();
	free(user_data);
}

int uib_app_run(app_data *user_data, int argc, char **argv) {
	widget_app_lifecycle_callback_s cbs = { 0, };

	cbs.create = widget_app_create;
	cbs.terminate = widget_app_terminate;

	int ret = widget_app_main(argc, argv, &cbs, user_data);
	if (ret != WIDGET_ERROR_NONE) {
		UIB_DLOG(DLOG_ERROR, LOG_TAG, "widget_app_main() is failed. err = %d",
				ret);
	}

	return ret;
}

static widget_class_h widget_app_create(void *user_data) {
	app_event_handler_h handlers[5] = { NULL, };

	widget_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY],
			APP_EVENT_LOW_BATTERY, _on_low_battery_cb, user_data);
	widget_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY],
			APP_EVENT_LOW_MEMORY, _on_low_memory_cb, user_data);
	widget_app_add_event_handler(
			&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED],
			APP_EVENT_DEVICE_ORIENTATION_CHANGED, _on_device_orientation_cb,
			user_data);
	widget_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED],
			APP_EVENT_LANGUAGE_CHANGED, _on_language_changed_cb, user_data);
	widget_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED],
			APP_EVENT_REGION_FORMAT_CHANGED, _on_region_format_changed_cb,
			user_data);

	widget_instance_lifecycle_callback_s cbs = {
			.create = _on_create_cb,
			.destroy = _on_destroy_cb,
			.pause = _on_pause_cb,
			.resume = _on_resume_cb,
			.update = _on_update_cb,
			.resize = _on_update_resize,
	};

	return widget_app_class_create(cbs, user_data);
}

static
void widget_app_terminate(void *user_data) {
	uib_views_get_instance()->destroy_window_obj();
}

static int _on_create_cb(widget_context_h context, bundle *content, int w, int h, void *user_data) {
	uib_app_manager_get_instance()->initialize(context, w, h);
	return WIDGET_ERROR_NONE;
}

static int _on_destroy_cb(widget_context_h context, widget_app_destroy_type_e reason, bundle *content, void *user_data) {
	return WIDGET_ERROR_NONE;
}

static int _on_resume_cb(widget_context_h context, void *user_data) {
	/* Take necessary actions when widget instance becomes visible. */
	return WIDGET_ERROR_NONE;
}

static int _on_pause_cb(widget_context_h context, void *user_data) {
	/* Take necessary actions when widget instance becomes invisible. */
	return WIDGET_ERROR_NONE;
}

static int _on_update_cb(widget_context_h context, bundle *content, int force,
		void *user_data) {
	/* Take necessary actions when widget instance should be updated. */
	return WIDGET_ERROR_NONE;
}

static int _on_update_resize(widget_context_h context, int w, int h,
		void *user_data) {
	/* Take necessary actions when the size of widget instance was changed. */
	return WIDGET_ERROR_NONE;
}

static void _on_low_battery_cb(app_event_info_h event_info, void *user_data) {
	/* Take necessary actions when the battery is low. */
}

static void _on_low_memory_cb(app_event_info_h event_info, void *user_data) {
	/* Take necessary actions when the system runs low on memory. */
}

static void _on_device_orientation_cb(app_event_info_h event_info,
		void *user_data) {
	/* deprecated APIs */
}

static void _on_language_changed_cb(app_event_info_h event_info,
		void *user_data) {
	/* Take necessary actions is called when language setting changes. */
	uib_views_get_instance()->uib_views_current_view_redraw();
}

static void _on_region_format_changed_cb(app_event_info_h event_info,
		void *user_data) {
	/* Take necessary actions when region format setting changes. */
}

