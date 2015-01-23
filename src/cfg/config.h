/* vifm
 * Copyright (C) 2001 Ken Steen.
 * Copyright (C) 2011 xaizek.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef VIFM__CFG__CONFIG_H__
#define VIFM__CFG__CONFIG_H__

#include <stddef.h> /* size_t */

#include "../ui/ui.h"
#include "../utils/fs_limits.h"
#include "../color_scheme.h"
#include "../types.h"
#include "hist.h"

/* Name of help file in plain text format. */
#define VIFM_HELP "vifm-help.txt"

/* Name help file in Vim-documentation format. */
#define VIFM_VIM_HELP "vifm-app.txt"

/* Name of directory in main configuration directory that contains scripts
 * implicitly included into $PATH for Vifm only. */
#define SCRIPTS_DIR "scripts"

typedef enum
{
	DD_ROOT_PARENT    = 1 << 0,
	DD_NONROOT_PARENT = 1 << 1,
	NUM_DOT_DIRS      =      2
}
DotDirs;

/* Indexes for cfg.decorations. */
enum
{
	DECORATION_PREFIX, /* The symbol, which is prepended to file name. */
	DECORATION_SUFFIX, /* The symbol, which is appended to file name. */
};

typedef struct config_t
{
	char home_dir[PATH_MAX]; /* ends with a slash */
	char config_dir[PATH_MAX];
	/* This one should be set using set_trash_dir() function. */
	char trash_dir[PATH_MAX];
	char log_file[PATH_MAX];
	char *vi_command;
	int vi_cmd_bg;
	char *vi_x_command;
	int vi_x_cmd_bg;
	int use_trash;

	/* Whether support of terminal multiplexers is enabled. */
	int use_term_multiplexer;

	int use_vim_help;
	int history_len;

	int auto_execute;
	int show_one_window;
	int use_iec_prefixes;
	int wrap_quick_view;
	char *time_format;
	char *fuse_home; /* This one should be set using set_fuse_home() function. */

	/* History of command-line commands. */
	hist_t cmd_hist;
	/* History of search patterns. */
	hist_t search_hist;
	/* History of prompt input. */
	hist_t prompt_hist;
	/* History of local filter patterns. */
	hist_t filter_hist;

	col_scheme_t cs;

	int undo_levels; /* Maximum number of changes that can be undone. */
	int sort_numbers; /* Natural sort of (version) numbers within text. */
	int follow_links; /* Follow links on l or Enter. */
	int confirm; /* Ask user about permanent deletion of files. */
	int fast_run;
	int wild_menu;
	int ignore_case;
	int smart_case;
	int hl_search;
	int vifm_info;
	int auto_ch_pos;
	char *shell;
	int scroll_off;
	int gdefault;
	int scroll_bind;
	int wrap_scan;
	int inc_search;
	int selection_is_primary; /* For yy, dd and DD: act on selection not file. */
	int tab_switches_pane; /* Whether <tab> is switch pane or history forward. */
	int use_system_calls; /* Prefer performing operations with system calls. */
	int tab_stop;
	char *ruler_format;
	char *status_line;
	int lines; /* Terminal height in lines. */
	int columns; /* Terminal width in characters. */
	/* Controls displaying of dot directories.  Combination of DotDirs flags. */
	int dot_dirs;
	char decorations[FILE_TYPE_COUNT][2]; /* Prefixes and suffixes of files. */
	int trunc_normal_sb_msgs; /* Truncate normal status bar messages if needed. */
	int filter_inverted_by_default; /* Default inversion value for :filter. */
	char *apropos_prg; /* apropos tool calling pattern. */
	char *find_prg; /* find tool calling pattern. */
	char *grep_prg; /* grep tool calling pattern. */
	char *locate_prg; /* locate tool calling pattern. */

	/* Comma-separated list of file system types which are slow to respond. */
	char *slow_fs_list;

	/* Coma separated list of places to look for relative path to directories. */
	char *cd_path;

	/* Whether there should be reserved single character width space before and
	 * after file list column inside a view. */
	int filelist_col_padding;

	/* Whether side borders are visible (separator in the middle isn't
	 * affected). */
	int side_borders_visible;

	/* Whether statusline is visible. */
	int display_statusline;

	/* Per line pattern for borders. */
	char *border_filler;

	/* Whether directory path should always be resolved to real path (all symbolic
	 * link expanded). */
	int chase_links;

	int timeout_len;     /* Maximum period on waiting for the input. */
	int min_timeout_len; /* Minimum period on waiting for the input. */
}
config_t;

extern config_t cfg;

/* Initializes cfg global variable with initial values.  Re-initialization is
 * not supported. */
void init_config(void);

/* Searches for configuration file and directories, stores them and ensures
 * existence of some of them.  This routine is separated from init_config() to
 * allow logging of path discovery. */
void set_config_paths(void);

/* Sources vifmrc file (pointed to by the $MYVIFMRC). */
void source_config(void);

/* Returns non-zero on error. */
int source_file(const char filename[]);

/* Checks whether vifmrc file (pointed to by the $MYVIFMRC) has old format.
 * Returns non-zero if so, otherwise zero is returned. */
int is_old_config(void);

/* Checks whether color scheme are stored in obsolete single-file format.
 * Returns non-zero if so, otherwise zero is returned. */
int are_old_color_schemes(void);

/* Gets editor invocation command.  Sets *bg to indicate whether the command
 * should be executed in background.  Returns pointer to a string from
 * configuration variables. */
const char * get_vicmd(int *bg);

/* Changes size of all histories. */
void resize_history(size_t new_len);

/* Sets value of cfg.fuse_home.  Returns non-zero in case of error, otherwise
 * zero is returned. */
int set_fuse_home(const char new_value[]);

/* Sets whether support of terminal multiplexers is enabled. */
void set_use_term_multiplexer(int use_term_multiplexer);

/* Frees memory previously allocated for specified history items. */
void free_history_items(const history_t history[], size_t len);

/* Saves command to command history. */
void save_command_history(const char command[]);

/* Saves pattern to search history. */
void save_search_history(const char pattern[]);

/* Saves input to prompt history. */
void save_prompt_history(const char input[]);

/* Saves input to local filter history. */
void save_filter_history(const char pattern[]);

/* Gets the most recently used search pattern.  Returns the pattern or empty
 * string if search history is empty. */
const char * cfg_get_last_search_pattern(void);

/* Sets shell invocation command. */
void cfg_set_shell(const char shell[]);

#endif /* VIFM__CFG__CONFIG_H__ */

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */
