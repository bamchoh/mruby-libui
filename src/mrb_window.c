#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mrb_libui.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mrb_window.h"

typedef struct {
	int id;
	uiWindow *mainwin;
	mrb_state *mrb;
	mrb_value block;
} mrb_libui_context;

static const struct mrb_data_type mrb_libui_context_type = {
  "mrb_libui_context", mrb_free,
};

static int onClosing(uiWindow *w, void *data)
{
	mrb_libui_context *mrb_libui_ctx;
	mrb_libui_ctx = (mrb_libui_context *)data;

	if(mrb_libui_ctx != NULL) {
		if (mrb_libui_ctx->mrb != NULL) {
			mrb_yield_argv(mrb_libui_ctx->mrb, mrb_libui_ctx->block, 0, NULL);
		}
	} else {
		uiQuit();
	}

	return 1;
}

static mrb_libui_context *mrb_libui_get_context(mrb_state *mrb, mrb_value self, const char *ctx_flag)
{
	mrb_libui_context *c;
	mrb_value context;

	context = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, ctx_flag));
	Data_Get_Struct(mrb, context, &mrb_libui_context_type, c);

	if(!c)
		mrb_raise(mrb, E_RUNTIME_ERROR, "get mrb_libui_context failed");

	return c;
}

static void mrb_libui_init_on_closing(mrb_state *mrb, mrb_value self)
{
	mrb_libui_context *mrb_libui_ctx = mrb_libui_get_context(mrb, self, "mrb_libui_context");

	uiWindowOnClosing(mrb_libui_ctx->mainwin, onClosing, NULL);
}

static mrb_value mrb_libui_new_window(mrb_state *mrb, mrb_value self)
{
	int len, w, h, has_menu;
  char *title;
  mrb_get_args(mrb, "sii|i", &title, &len, &w, &h, &has_menu);

	mrb_libui_context *mrb_libui_ctx = malloc(sizeof(mrb_libui_context));

	mrb_libui_ctx->id = 1;
	mrb_libui_ctx->mainwin = uiNewWindow(title, w, h, has_menu);

	mrb_iv_set(mrb
			, self
			, mrb_intern_cstr(mrb, "mrb_libui_context")
			, mrb_obj_value(Data_Wrap_Struct(mrb
					, mrb->object_class
					, &mrb_libui_context_type
					, (void *)mrb_libui_ctx)
			)
	);

	mrb_libui_init_on_closing(mrb, self);

	return self;
}

static mrb_value mrb_libui_show_window(mrb_state *mrb, mrb_value self)
{
	mrb_libui_context *mrb_libui_ctx = mrb_libui_get_context(mrb, self, "mrb_libui_context");

	uiControlShow(uiControl(mrb_libui_ctx->mainwin));

	return mrb_true_value();
}

static mrb_value mrb_libui_window_on_closing(mrb_state *mrb, mrb_value self)
{
	mrb_value b;
	mrb_libui_context *mrb_libui_ctx = mrb_libui_get_context(mrb, self, "mrb_libui_context");

  mrb_get_args(mrb, "&", &(mrb_libui_ctx->block));

	mrb_libui_ctx->mrb = mrb;

	uiWindowOnClosing(mrb_libui_ctx->mainwin, onClosing, mrb_libui_ctx);

	return mrb_true_value();
}

void mrb_libui_window_init(mrb_state *mrb, struct RClass *libui) {
    struct RClass *window = mrb_define_class_under(mrb, libui, "Window", mrb->object_class);
    mrb_define_method(mrb, window, "initialize", mrb_libui_new_window, MRB_ARGS_REQ(3) | MRB_ARGS_OPT(1));
    mrb_define_method(mrb, window, "show", mrb_libui_show_window, MRB_ARGS_NONE());
    mrb_define_method(mrb, window, "on_closing", mrb_libui_window_on_closing, MRB_ARGS_REQ(1));
}

