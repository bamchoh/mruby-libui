/*
** mrb_libui.c - LibUI class
**
** Copyright (c) bamchoh 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mrb_libui.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mrb_window.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static mrb_value mrb_libui_init(mrb_state *mrb, mrb_value self)
{
	uiInitOptions o;
	const char *err;
	memset(&o, 0, sizeof (uiInitOptions));
	err = uiInit(&o);
	if (err != NULL) {
		mrb_raisef(mrb, E_RUNTIME_ERROR, "error initializing ui: %S", mrb_str_new_cstr(mrb, err));
		uiFreeInitError(err);
		return mrb_nil_value();
	}

	return mrb_nil_value();
}

static mrb_value mrb_libui_uninit(mrb_state *mrb, mrb_value self)
{
	uiUninit();
	return mrb_nil_value();
}

static mrb_value mrb_libui_main(mrb_state *mrb, mrb_value self)
{
	uiMain();
	return mrb_nil_value();
}

static mrb_value mrb_libui_main_step(mrb_state *mrb, mrb_value self)
{
	int wait;
  mrb_get_args(mrb, "i", &wait);

	if(uiMainStep(wait)) {
		return mrb_true_value();
	} else {
		return mrb_false_value();
	}
}

static mrb_value mrb_libui_quit(mrb_state *mrb, mrb_value self)
{
	uiQuit();
	return mrb_nil_value();
}

void mrb_mruby_libui_gem_init(mrb_state *mrb)
{
    struct RClass *libui = mrb_define_module(mrb, "LibUI");
    mrb_define_class_method(mrb, libui, "init", mrb_libui_init, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, libui, "uninit", mrb_libui_uninit, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, libui, "main", mrb_libui_main, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, libui, "main_step", mrb_libui_main_step, MRB_ARGS_REQ(1));
    mrb_define_class_method(mrb, libui, "quit", mrb_libui_quit, MRB_ARGS_NONE());

    mrb_libui_window_init(mrb, libui);

    // TODO: need to figure out callback sequence for mruby
		// mrb_define_class_method(mrb, libui, "queue_main", mrb_libui_queue_main, MRB_ARGS_NONE());
		// mrb_define_class_method(mrb, libui, "on_should_quit", mrb_libui_on_should_quit, MRB_ARGS_NONE());
    // TODO: it might be unnecessary for mruby
    // mrb_define_class_method(mrb, libui, "free_text", mrb_libui_free_text, MRB_ARGS_NONE());

    DONE;
}

void mrb_mruby_libui_gem_final(mrb_state *mrb)
{
}

