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

#define SETTING_ENABLED_NAME "enabled"
#define SETTING_ENABLED_TEXT "Enabled"

struct test_filter {
    bool enabled;
    obs_source_t *source;
    gs_effect_t *whatever;
};

static obs_properties_t *filter_get_properties(void *data)
{
    UNUSED_PARAMETER(data);

    obs_properties_t *props = obs_properties_create();

    obs_properties_add_bool(props, SETTING_ENABLED_NAME, SETTING_ENABLED_TEXT);

    return props;
}

static void filter_get_defaults(obs_data_t *settings)
{
    obs_data_set_default_bool(settings, SETTING_ENABLED_NAME, true);
}

static void filter_update(void *data, obs_data_t *settings)
{
    struct test_filter *tf = data;

    tf->enabled = obs_data_get_bool(settings, SETTING_ENABLED_NAME);
}

static const char *filter_getname(void *unused)
{
    UNUSED_PARAMETER(unused);

    return "Dr. Mario";
}

static void *filter_create(obs_data_t *settings, obs_source_t *source)
{
    UNUSED_PARAMETER(settings);

    struct test_filter *tf = bzalloc(sizeof(struct test_filter));

    tf->enabled = true;
    tf->source = source;

    return tf;
}

static void filter_destroy(void *data)
{
    struct test_filter *tf = data;

    bfree(tf);
}

static struct obs_source_frame* filter_video(void* data,
                                             struct obs_source_frame* frame)
{
    struct test_filter *tf = data;

    if (tf->enabled) {
        switch (frame->format) {

        case VIDEO_FORMAT_YUY2:
        case VIDEO_FORMAT_UYVY:
            for (unsigned int i = 0; i < frame->height * frame->width; i++) {
                frame->data[0][i] = 0;
            }
            break;

        case VIDEO_FORMAT_NV12:
            blog(LOG_INFO, "nv12");

            for (unsigned int i = 0; i < frame->height; i++) {
                frame->data[0][i] = 0;
            }
            break;

        default:
            blog(LOG_INFO, "unsupported format: %d", frame->format);
            break;
        }
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
    .output_flags = OBS_SOURCE_ASYNC_VIDEO,
    .get_name = filter_getname,
    .create = filter_create,
    .destroy = filter_destroy,
    .get_defaults = filter_get_defaults,
    .get_properties = filter_get_properties,
    .update = filter_update,
    .filter_video = filter_video,
    .filter_remove = filter_remove
};
