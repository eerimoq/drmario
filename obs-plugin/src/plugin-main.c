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

#include "plugin-macros.generated.h"

OBS_DECLARE_MODULE();

struct test_filter {
    obs_source_t *source;
    gs_effect_t *whatever;
};

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

    tf->source = source;
    tf->whatever = gs_effect_create_from_file(effect_file, NULL);
    bfree(effect_file);
    if (!tf->whatever) {
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

static struct obs_source_info test_filter = {
    .id = "dr_mario",
    .type = OBS_SOURCE_TYPE_FILTER,
    .output_flags = OBS_SOURCE_VIDEO,
    .get_name = filter_getname,
    .create = filter_create,
    .destroy = filter_destroy,
    .video_render = filter_render,
};

bool obs_module_load(void)
{
    blog(LOG_INFO,
         "Dr mario plugin loaded successfully (version %s)",
         PLUGIN_VERSION);
    obs_register_source(&test_filter);
    return true;
}

void obs_module_unload()
{
    blog(LOG_INFO, "Dr mario plugin unloaded");
}
