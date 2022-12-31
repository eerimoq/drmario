/*
Dr. Mario
Copyright (C) 2022 Erik Moqvist <erik.moqvist@gmail.com>

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

#include <memory>
#include <obs-module.h>
#include "plugin_macros.h"
#include "dr_mario_analyzer.hpp"

#define SETTING_ENABLED_NAME "enabled"
#define SETTING_ENABLED_TEXT "Enabled"

#define SETTING_BOTTLE_TOP_NAME "bottle-top"
#define SETTING_BOTTLE_TOP_TEXT "Top"
#define SETTING_BOTTLE_BOTTOM_NAME "bottle-bottom"
#define SETTING_BOTTLE_BOTTOM_TEXT "Bottom"
#define SETTING_BOTTLE_LEFT_NAME "bottle-left"
#define SETTING_BOTTLE_LEFT_TEXT "Left"
#define SETTING_BOTTLE_RIGHT_NAME "bottle-right"
#define SETTING_BOTTLE_RIGHT_TEXT "Right"

#define SETTING_NEXT_PILL_TOP_NAME "next-pill-top"
#define SETTING_NEXT_PILL_TOP_TEXT "Top"
#define SETTING_NEXT_PILL_BOTTOM_NAME "next-pill-bottom"
#define SETTING_NEXT_PILL_BOTTOM_TEXT "Bottom"
#define SETTING_NEXT_PILL_LEFT_NAME "next-pill-left"
#define SETTING_NEXT_PILL_LEFT_TEXT "Left"
#define SETTING_NEXT_PILL_RIGHT_NAME "next-pill-right"
#define SETTING_NEXT_PILL_RIGHT_TEXT "Right"

struct dr_mario_filter {
    bool enabled;
    obs_source_t *source;
    std::shared_ptr<DrMario::Analyzer> analyzer;
};

static obs_properties_t *filter_get_properties(void *data)
{
    UNUSED_PARAMETER(data);

    obs_properties_t *props = obs_properties_create();

    obs_properties_add_bool(props,
                            SETTING_ENABLED_NAME,
                            SETTING_ENABLED_TEXT);

    obs_properties_t *bottle_props = obs_properties_create();

    obs_properties_add_float_slider(bottle_props,
                                    SETTING_BOTTLE_TOP_NAME,
                                    SETTING_BOTTLE_TOP_TEXT,
                                    0.0,
                                    100.0,
                                    0.1);
    obs_properties_add_float_slider(bottle_props,
                                    SETTING_BOTTLE_BOTTOM_NAME,
                                    SETTING_BOTTLE_BOTTOM_TEXT,
                                    0.0,
                                    100.0,
                                    0.1);
    obs_properties_add_float_slider(bottle_props,
                                    SETTING_BOTTLE_LEFT_NAME,
                                    SETTING_BOTTLE_LEFT_TEXT,
                                    0.0,
                                    100.0,
                                    0.1);
    obs_properties_add_float_slider(bottle_props,
                                    SETTING_BOTTLE_RIGHT_NAME,
                                    SETTING_BOTTLE_RIGHT_TEXT,
                                    0.0,
                                    100.0,
                                    0.1);

    obs_properties_add_group(props,
                             "bottle-position",
                             "Bottle position",
                             OBS_GROUP_NORMAL,
                             bottle_props);

    obs_properties_t *next_pill_props = obs_properties_create();

    obs_properties_add_float_slider(next_pill_props,
                                    SETTING_NEXT_PILL_TOP_NAME,
                                    SETTING_NEXT_PILL_TOP_TEXT,
                                    0.0,
                                    100.0,
                                    0.1);
    obs_properties_add_float_slider(next_pill_props,
                                    SETTING_NEXT_PILL_BOTTOM_NAME,
                                    SETTING_NEXT_PILL_BOTTOM_TEXT,
                                    0.0,
                                    100.0,
                                    0.1);
    obs_properties_add_float_slider(next_pill_props,
                                    SETTING_NEXT_PILL_LEFT_NAME,
                                    SETTING_NEXT_PILL_LEFT_TEXT,
                                    0.0,
                                    100.0,
                                    0.1);
    obs_properties_add_float_slider(next_pill_props,
                                    SETTING_NEXT_PILL_RIGHT_NAME,
                                    SETTING_NEXT_PILL_RIGHT_TEXT,
                                    0.0,
                                    100.0,
                                    0.1);

    obs_properties_add_group(props,
                             "next-pill-position",
                             "Next pill position",
                             OBS_GROUP_NORMAL,
                             next_pill_props);
    
    return props;
}

static void filter_get_defaults(obs_data_t *settings)
{
    obs_data_set_default_bool(settings, SETTING_ENABLED_NAME, true);

    obs_data_set_default_double(settings, SETTING_BOTTLE_TOP_NAME, 20.0);
    obs_data_set_default_double(settings, SETTING_BOTTLE_BOTTOM_NAME, 80.0);
    obs_data_set_default_double(settings, SETTING_BOTTLE_LEFT_NAME, 40.0);
    obs_data_set_default_double(settings, SETTING_BOTTLE_RIGHT_NAME, 60.0);

    obs_data_set_default_double(settings, SETTING_NEXT_PILL_TOP_NAME, 20.0);
    obs_data_set_default_double(settings, SETTING_NEXT_PILL_BOTTOM_NAME, 24.0);
    obs_data_set_default_double(settings, SETTING_NEXT_PILL_LEFT_NAME, 70.0);
    obs_data_set_default_double(settings, SETTING_NEXT_PILL_RIGHT_NAME, 78.0);
}

static void filter_update(void *data, obs_data_t *settings)
{
    struct dr_mario_filter *dmf = static_cast<struct dr_mario_filter *>(data);

    dmf->enabled = obs_data_get_bool(settings, SETTING_ENABLED_NAME);
}

static const char *filter_getname(void *unused)
{
    UNUSED_PARAMETER(unused);

    return "Dr. Mario";
}

static void *filter_create(obs_data_t *settings, obs_source_t *source)
{
    UNUSED_PARAMETER(settings);

    struct dr_mario_filter *dmf = static_cast<struct dr_mario_filter *>(
                                      bzalloc(sizeof(struct dr_mario_filter)));

    dmf->enabled = true;
    dmf->source = source;
    dmf->analyzer = std::make_shared<DrMario::Analyzer>();

    return dmf;
}

static void filter_destroy(void *data)
{
    struct dr_mario_filter *dmf = static_cast<struct dr_mario_filter *>(data);

    bfree(dmf);
}

static struct obs_source_frame* filter_video(void* data,
                                             struct obs_source_frame* frame)
{
    struct dr_mario_filter *dmf = static_cast<struct dr_mario_filter *>(data);

    if (dmf->enabled) {
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

void register_dr_mario_filter()
{
    obs_register_source(&drmario_filter);
}
