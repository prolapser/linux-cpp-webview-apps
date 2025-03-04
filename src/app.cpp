#include "webview.h"
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <glib/gstdio.h>
#include <gio/gio.h>
#include <limits.h>
#include <unistd.h>


constexpr const char* APP_URL = "https://teletype.in/@stablediffusion/SD";
constexpr const char* APP_NAME = "AdblockTest";
constexpr const char* DATA_DIR_NAME = "data";
constexpr const char* ICON_FILE_NAME = "icon.png";

char* get_executable_path() {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    return (count != -1) ? g_path_get_dirname(path) : nullptr;
}

void create_data_dirs(const char* base_path) {
    const gchar* subdirs[] = {
        g_build_filename(base_path, DATA_DIR_NAME, "application-cache", NULL),
        g_build_filename(base_path, DATA_DIR_NAME, "cache", "http", NULL),
        g_build_filename(base_path, DATA_DIR_NAME, "cookies", NULL),
        g_build_filename(base_path, DATA_DIR_NAME, "domstorage", NULL),
        g_build_filename(base_path, DATA_DIR_NAME, "indexeddb", NULL),
        g_build_filename(base_path, DATA_DIR_NAME, "localstorage", NULL),
        g_build_filename(base_path, DATA_DIR_NAME, "service-workers", NULL),
        NULL
    };

    for (int i = 0; subdirs[i] != NULL; i++) {
        g_mkdir_with_parents(subdirs[i], 0755);
        g_free((gpointer)subdirs[i]);
    }
}


constexpr const char* INTERCEPT_JS = R"JS(
    document.addEventListener('click', function(e) {
        if (e.target.tagName === 'A' && e.target.href) {
            window.open(e.target.href, '_blank');
            e.preventDefault();
        }
    }, true);
)JS";

int main() {
    char* exe_dir = get_executable_path();
    if (!exe_dir) {
        g_printerr("ОШИБКА: невозможно определить путь до исполняемого файла!\n");
        return 1;
    }

    char* data_dir = g_build_filename(exe_dir, DATA_DIR_NAME, NULL);
    char* icon_path = g_build_filename(exe_dir, ICON_FILE_NAME, NULL);

    gtk_init(nullptr, nullptr);

    create_data_dirs(exe_dir);

    WebKitWebsiteDataManager *data_manager = webkit_website_data_manager_new(
        "base-data-directory", data_dir,
        "base-cache-directory", g_build_filename(data_dir, "cache", NULL),
        "local-storage-directory", g_build_filename(data_dir, "localstorage", NULL),
        "indexeddb-directory", g_build_filename(data_dir, "indexeddb", NULL),
        "offline-application-cache-directory", g_build_filename(data_dir, "application-cache", NULL),
        "disk-cache-directory", g_build_filename(data_dir, "cache", "http", NULL),
        NULL
    );

    WebKitWebContext *context = webkit_web_context_new_with_website_data_manager(data_manager);

    webkit_web_context_set_web_extensions_directory(context, g_strdup_printf("%s/.config/wyebadblock", g_get_home_dir()));
    webkit_web_context_set_sandbox_enabled(context, FALSE);

    WebKitCookieManager *cookie_manager = webkit_web_context_get_cookie_manager(context);
    webkit_cookie_manager_set_persistent_storage(
        cookie_manager,
        g_build_filename(data_dir, "cookies", "cookies.sqlite", NULL),
        WEBKIT_COOKIE_PERSISTENT_STORAGE_SQLITE
    );

    WebKitWebView *webview = WEBKIT_WEB_VIEW(webkit_web_view_new_with_context(context));

    WebKitUserContentManager *content_manager = webkit_web_view_get_user_content_manager(webview);
    webkit_user_content_manager_add_script(
        content_manager,
        webkit_user_script_new(
            INTERCEPT_JS,
            WEBKIT_USER_CONTENT_INJECT_ALL_FRAMES,
            WEBKIT_USER_SCRIPT_INJECT_AT_DOCUMENT_END,
            NULL,
            NULL
        )
    );

    g_signal_connect(webview, "create", G_CALLBACK(+[](WebKitWebView* webview, WebKitNavigationAction* action, gpointer) {
        const gchar *url = webkit_uri_request_get_uri(webkit_navigation_action_get_request(action));
        g_spawn_command_line_async(g_strdup_printf("xdg-open '%s'", url), NULL);
        return WEBKIT_WEB_VIEW(webkit_web_view_new());
    }), NULL);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), APP_NAME);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_widget_set_size_request(window, 800, 600);

    if (g_file_test(icon_path, G_FILE_TEST_EXISTS)) {
        gtk_window_set_icon_from_file(GTK_WINDOW(window), icon_path, nullptr);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(window, "destroy", G_CALLBACK(+[](GtkWidget* widget, gpointer data) {
        WebKitWebView* webview = WEBKIT_WEB_VIEW(data);
        webkit_web_view_stop_loading(webview);
        webkit_web_context_clear_cache(webkit_web_view_get_context(webview));
    }), webview);

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(webview));
    webkit_web_view_load_uri(webview, APP_URL);
    gtk_widget_show_all(window);

    gtk_main();

    g_object_unref(content_manager);
    g_object_unref(webview);
    g_object_unref(context);
    g_object_unref(data_manager);

    g_free(exe_dir);
    g_free(data_dir);
    g_free(icon_path);

    return 0;
}