/*
Dr. Mario
Copyright (C) 2022 erik.moqvist@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
//#include "plugin-macros.generated.h"

#define SETTING_ENABLED_TEXT "Style enabled"
#define SETTING_ENABLED_NAME "enabled"
#define SETTING_ALPHA_TEXT "Style ratio"
#define SETTING_ALPHA_NAME "alpha"
#define SETTING_STYLE_FILE_NAME "Style"
#define SETTING_STYLE_FILE_TEXT "style"

//OBS_DECLARE_MODULE();

struct test_filter {
    obs_source_t *source;
    gs_effect_t *whatever;
};

static obs_properties_t *filter_get_properties(void *data)
{
    UNUSED_PARAMETER(data);

    obs_properties_t *props = obs_properties_create();

    obs_properties_add_bool(props,
                            SETTING_ENABLED_NAME,
                            SETTING_ENABLED_TEXT);
    obs_properties_add_float(props,
                             SETTING_ALPHA_NAME,
                             SETTING_ALPHA_TEXT,
                             0,
                             1,
                             0.01);
    obs_properties_add_path(props,
                            SETTING_STYLE_FILE_NAME,
                            SETTING_STYLE_FILE_TEXT,
                            OBS_PATH_FILE,
                            "*.jpg",
                            NULL);

    return props;
}

static void filter_get_defaults(obs_data_t *settings)
{
    obs_data_set_default_bool(settings, SETTING_ENABLED_NAME, false);
    obs_data_set_default_double(settings, SETTING_ALPHA_NAME, 1.0);
    obs_data_set_default_string(settings, SETTING_STYLE_FILE_NAME, "hello.jpg");
}

static void filter_update(void *data, obs_data_t *settings)
{
    UNUSED_PARAMETER(data);

    blog(LOG_INFO,
         "%s: %d",
         SETTING_ENABLED_NAME,
         (int)obs_data_get_bool(settings, SETTING_ENABLED_NAME));
    blog(LOG_INFO,
         "%s: %f",
         SETTING_ALPHA_NAME,
         obs_data_get_double(settings, SETTING_ALPHA_NAME));
    blog(LOG_INFO,
         "%s: %s",
         SETTING_STYLE_FILE_NAME,
         obs_data_get_string(settings, SETTING_STYLE_FILE_NAME));
}

static const char *filter_getname(void *unused)
{
    UNUSED_PARAMETER(unused);
    return "Dr. Mario";
}

static void filter_destroy(void *data)
{
    struct test_filter *tf = data;

    if (tf) {
        obs_enter_graphics();

        gs_effect_destroy(tf->whatever);
        bfree(tf);

        obs_leave_graphics();
    }
}

static void *filter_create(obs_data_t *settings, obs_source_t *source)
{
    UNUSED_PARAMETER(settings);

    struct test_filter *tf = bzalloc(sizeof(struct test_filter));
    char *effect_file;

    obs_enter_graphics();

    effect_file = obs_module_file("test.effect");
    blog(LOG_INFO, "test.effect: %p", effect_file);
    tf->source = source;
    tf->whatever = gs_effect_create_from_file(effect_file, NULL);
    bfree(effect_file);
    if (!tf->whatever) {
        blog(LOG_INFO, "no test.effect");
        filter_destroy(tf);
        tf = NULL;
    }

    obs_leave_graphics();

    return tf;
}

static void filter_render(void *data, gs_effect_t *effect)
{
    UNUSED_PARAMETER(effect);

    struct test_filter *tf = data;

    if (!obs_source_process_filter_begin(tf->source,
                                         GS_RGBA,
                                         OBS_ALLOW_DIRECT_RENDERING)) {
        return;
    }

    obs_source_process_filter_end(tf->source, tf->whatever, 0, 0);
}

static struct obs_source_frame* filter_video(void* data,
                                             struct obs_source_frame* frame)
{
    UNUSED_PARAMETER(data);

    // blog(LOG_INFO,
    //      "Timestamp: %llu, format: %d %d",
    //      (unsigned long long)frame->timestamp,
    //      frame->format,
    //      VIDEO_FORMAT_UYVY);

    for (unsigned int i = 0; i < frame->height * frame->width; i++) {
        frame->data[0][i] = 0;
    }

    return frame;
}

static void filter_remove(void* data, obs_source_t* parent)
{
    UNUSED_PARAMETER(data);
    UNUSED_PARAMETER(parent);
}

struct obs_source_info drmario_filter = {
    .id = "dr_mario",
    .type = OBS_SOURCE_TYPE_FILTER,
    // .output_flags = OBS_SOURCE_VIDEO,
    // .output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW,
    .output_flags = OBS_SOURCE_ASYNC_VIDEO,
    .get_name = filter_getname,
    .create = filter_create,
    .destroy = filter_destroy,
    // .video_render = filter_render,
    .get_defaults = filter_get_defaults,
    .get_properties = filter_get_properties,
    .update = filter_update,
    .filter_video = filter_video,
    .filter_remove = filter_remove
};
