/* -*- c -*- */
#if !defined INCLUDED_yuck_h_
#define INCLUDED_yuck_h_

#include <stddef.h>

#define YUCK_OPTARG_NONE	((void*)0x1U)

enum yuck_cmds_e {
	/* value used when no command was specified */
	BATON_CMD_NONE = 0U,

	/* actual commands */
	BATON_CMD_ENABLE,
	BATON_CMD_DISABLE,
	BATON_CMD_READ,
	BATON_CMD_SET_ID,
	BATON_CMD_GET_ID,
	BATON_CMD_GET_FIRMWARE_VERSION,
	BATON_CMD_TOGGLE_BITFIELD,
	BATON_CMD_READ_BITFIELD,
	
	/* convenience identifiers */
	YUCK_NOCMD = BATON_CMD_NONE,
	YUCK_NCMDS = BATON_CMD_READ_BITFIELD
};



typedef union yuck_u yuck_t;


/* convenience structure for `enable' */
struct yuck_cmd_enable_s {
	enum yuck_cmds_e cmd;

	/* left-over arguments, the command string is never a part of this */
	size_t nargs;
	char **args;

	/* help is handled automatically */
	/* version is handled automatically */
	char *device_arg;

};

/* convenience structure for `disable' */
struct yuck_cmd_disable_s {
	enum yuck_cmds_e cmd;

	/* left-over arguments, the command string is never a part of this */
	size_t nargs;
	char **args;

	/* help is handled automatically */
	/* version is handled automatically */
	char *device_arg;

};

/* convenience structure for `read' */
struct yuck_cmd_read_s {
	enum yuck_cmds_e cmd;

	/* left-over arguments, the command string is never a part of this */
	size_t nargs;
	char **args;

	/* help is handled automatically */
	/* version is handled automatically */
	char *device_arg;

};

/* convenience structure for `set_id' */
struct yuck_cmd_set_id_s {
	enum yuck_cmds_e cmd;

	/* left-over arguments, the command string is never a part of this */
	size_t nargs;
	char **args;

	/* help is handled automatically */
	/* version is handled automatically */
	char *device_arg;

};

/* convenience structure for `get_id' */
struct yuck_cmd_get_id_s {
	enum yuck_cmds_e cmd;

	/* left-over arguments, the command string is never a part of this */
	size_t nargs;
	char **args;

	/* help is handled automatically */
	/* version is handled automatically */
	char *device_arg;

};

/* convenience structure for `get_firmware_version' */
struct yuck_cmd_get_firmware_version_s {
	enum yuck_cmds_e cmd;

	/* left-over arguments, the command string is never a part of this */
	size_t nargs;
	char **args;

	/* help is handled automatically */
	/* version is handled automatically */
	char *device_arg;

};

/* convenience structure for `toggle_bitfield' */
struct yuck_cmd_toggle_bitfield_s {
	enum yuck_cmds_e cmd;

	/* left-over arguments, the command string is never a part of this */
	size_t nargs;
	char **args;

	/* help is handled automatically */
	/* version is handled automatically */
	char *device_arg;

};

/* convenience structure for `read_bitfield' */
struct yuck_cmd_read_bitfield_s {
	enum yuck_cmds_e cmd;

	/* left-over arguments, the command string is never a part of this */
	size_t nargs;
	char **args;

	/* help is handled automatically */
	/* version is handled automatically */
	char *device_arg;

};


union yuck_u {
	/* generic struct */
	struct {
		enum yuck_cmds_e cmd;

		/* left-over arguments,
		 * the command string is never a part of this */
		size_t nargs;
		char **args;

		/* slots common to all commands */
		/* help is handled automatically */
		/* version is handled automatically */
		char *device_arg;
	};

	/* depending on CMD at most one of the following structs is filled in
	 * if CMD is YUCK_NONE no slots of this union must be accessed */
	struct yuck_cmd_enable_s enable;
	struct yuck_cmd_disable_s disable;
	struct yuck_cmd_read_s read;
	struct yuck_cmd_set_id_s set_id;
	struct yuck_cmd_get_id_s get_id;
	struct yuck_cmd_get_firmware_version_s get_firmware_version;
	struct yuck_cmd_toggle_bitfield_s toggle_bitfield;
	struct yuck_cmd_read_bitfield_s read_bitfield;
};


static __attribute__((nonnull(1))) int
yuck_parse(yuck_t*, int argc, char *argv[]);
static __attribute__((nonnull(1))) void yuck_free(yuck_t*);

static __attribute__((nonnull(1))) void yuck_auto_help(const yuck_t*);
static __attribute__((nonnull(1))) void yuck_auto_usage(const yuck_t*);
static __attribute__((nonnull(1))) void yuck_auto_version(const yuck_t*);

/* some hooks */
#if defined yuck_post_help
static __attribute__((nonnull(1))) void yuck_post_help(const yuck_t*);
#endif	/* yuck_post_help */

#if defined yuck_post_usage
static __attribute__((nonnull(1))) void yuck_post_usage(const yuck_t*);
#endif	/* yuck_post_usage */

#if defined yuck_post_version
static __attribute__((nonnull(1))) void yuck_post_version(const yuck_t*);
#endif	/* yuck_post_version */

#endif	/* INCLUDED_yuck_h_ */
/* -*- c -*- */
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#if defined HAVE_VERSION_H
# include "version.h"
#endif	/* HAVE_VERSION_H */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#if defined __INTEL_COMPILER
# pragma warning (push)
# pragma warning (disable:177)
# pragma warning (disable:111)
# pragma warning (disable:3280)
#elif defined __GNUC__
# if __GNUC__ > 4 || __GNUC__ == 4 &&  __GNUC_MINOR__ >= 6
#  pragma GCC diagnostic push
# endif	 /* GCC version */
# pragma GCC diagnostic ignored "-Wunused-label"
# pragma GCC diagnostic ignored "-Wunused-variable"
# pragma GCC diagnostic ignored "-Wunused-function"
# pragma GCC diagnostic ignored "-Wshadow"
#endif	/* __INTEL_COMPILER */


static inline bool
yuck_streqp(const char *s1, const char *s2)
{
	return !strcmp(s1, s2);
}

/* for multi-args */
static inline char**
yuck_append(char **array, size_t n, char *val)
{
	if (!(n % 16U)) {
		/* resize */
		void *tmp = realloc(array, (n + 16U) * sizeof(*array));
		if (tmp == NULL) {
			free(array);
			return NULL;
		}
		/* otherwise make it persistent */
		array = tmp;
	}
	array[n] = val;
	return array;
}

static enum yuck_cmds_e yuck_parse_cmd(const char *cmd)
{
	if (0) {
		;
	} else if (yuck_streqp(cmd, "enable")) {
		return BATON_CMD_ENABLE;
	} else if (yuck_streqp(cmd, "disable")) {
		return BATON_CMD_DISABLE;
	} else if (yuck_streqp(cmd, "read")) {
		return BATON_CMD_READ;
	} else if (yuck_streqp(cmd, "set_id")) {
		return BATON_CMD_SET_ID;
	} else if (yuck_streqp(cmd, "get_id")) {
		return BATON_CMD_GET_ID;
	} else if (yuck_streqp(cmd, "get_firmware_version")) {
		return BATON_CMD_GET_FIRMWARE_VERSION;
	} else if (yuck_streqp(cmd, "toggle_bitfield")) {
		return BATON_CMD_TOGGLE_BITFIELD;
	} else if (yuck_streqp(cmd, "read_bitfield")) {
		return BATON_CMD_READ_BITFIELD;
	} else {
		/* error here? */
		fprintf(stderr, "baton: invalid command `%s'\n\
Try `--help' for a list of commands.\n", cmd);
	}
	return (enum yuck_cmds_e)-1;
}


static int yuck_parse(yuck_t tgt[static 1U], int argc, char *argv[])
{
	char *op;
	int i;

	/* we'll have at most this many args */
	memset(tgt, 0, sizeof(*tgt));
	if ((tgt->args = calloc(argc, sizeof(*tgt->args))) == NULL) {
		return -1;
	}
	for (i = 1; i < argc && tgt->nargs < (size_t)-1; i++) {
		op = argv[i];

		switch (*op) {
		case '-':
			/* could be an option */
			switch (*++op) {
			default:
				/* could be glued into one */
				for (; *op; op++) {
					goto shortopt; back_from_shortopt:;
				}
				break;
			case '-':
				if (*++op == '\0') {
					i++;
					goto dashdash; back_from_dashdash:;
					break;
				}
				goto longopt; back_from_longopt:;
				break;
			case '\0':
				goto plain_dash;
			}
			break;
		default:
		plain_dash:
			goto arg; back_from_arg:;
			break;
		}
	}
	if (i < argc) {
		op = argv[i];

		if (*op++ == '-' && *op++ == '-' && !*op) {
			/* another dashdash, filter out */
			i++;
		}
	}
	/* has to be here as the max_pargs condition might drive us here */
	dashdash:
	{
		/* dashdash loop, pile everything on tgt->args
		 * don't check for subcommands either, this is in accordance to
		 * the git tool which won't accept commands after -- */
		for (; i < argc; i++) {
			tgt->args[tgt->nargs++] = argv[i];
		}
	}
	return 0;

	longopt:
	{
		/* split into option and arg part */
		char *arg;

		if ((arg = strchr(op, '=')) != NULL) {
			/* \nul this one out */
			*arg++ = '\0';
		}

		switch (tgt->cmd) {
		default:
			goto BATON_CMD_NONE_longopt; back_from_BATON_CMD_NONE_longopt:;
			break;
		case BATON_CMD_ENABLE:
			goto BATON_CMD_ENABLE_longopt; back_from_BATON_CMD_ENABLE_longopt:;
			break;
		case BATON_CMD_DISABLE:
			goto BATON_CMD_DISABLE_longopt; back_from_BATON_CMD_DISABLE_longopt:;
			break;
		case BATON_CMD_READ:
			goto BATON_CMD_READ_longopt; back_from_BATON_CMD_READ_longopt:;
			break;
		case BATON_CMD_SET_ID:
			goto BATON_CMD_SET_ID_longopt; back_from_BATON_CMD_SET_ID_longopt:;
			break;
		case BATON_CMD_GET_ID:
			goto BATON_CMD_GET_ID_longopt; back_from_BATON_CMD_GET_ID_longopt:;
			break;
		case BATON_CMD_GET_FIRMWARE_VERSION:
			goto BATON_CMD_GET_FIRMWARE_VERSION_longopt; back_from_BATON_CMD_GET_FIRMWARE_VERSION_longopt:;
			break;
		case BATON_CMD_TOGGLE_BITFIELD:
			goto BATON_CMD_TOGGLE_BITFIELD_longopt; back_from_BATON_CMD_TOGGLE_BITFIELD_longopt:;
			break;
		case BATON_CMD_READ_BITFIELD:
			goto BATON_CMD_READ_BITFIELD_longopt; back_from_BATON_CMD_READ_BITFIELD_longopt:;
			break;
		}
		goto back_from_longopt;


		BATON_CMD_NONE_longopt:
		{
			if (0) {
				;
			} else if (yuck_streqp(op, "help")) {
				/* invoke auto action and exit */
				yuck_auto_help(tgt);
				goto success;
			} else if (yuck_streqp(op, "version")) {
				/* invoke auto action and exit */
				yuck_auto_version(tgt);
				goto success;
			} else if (yuck_streqp(op, "device")) {
				tgt->device_arg = arg ?: argv[++i];
			} else {
				/* grml */
				fprintf(stderr, "baton: unrecognized option `--%s'\n", op);
				goto failure;
			xtra_chk:
				if (arg != NULL) {
					fprintf(stderr, "baton: option `--%s' doesn't allow an argument\n", op);
					goto failure;
				}
			}
			goto back_from_BATON_CMD_NONE_longopt;
		}
		BATON_CMD_ENABLE_longopt:
		{
			if (0) {
				;
			} else {
				goto BATON_CMD_NONE_longopt;
			}
			goto back_from_BATON_CMD_ENABLE_longopt;
		}
		BATON_CMD_DISABLE_longopt:
		{
			if (0) {
				;
			} else {
				goto BATON_CMD_NONE_longopt;
			}
			goto back_from_BATON_CMD_DISABLE_longopt;
		}
		BATON_CMD_READ_longopt:
		{
			if (0) {
				;
			} else {
				goto BATON_CMD_NONE_longopt;
			}
			goto back_from_BATON_CMD_READ_longopt;
		}
		BATON_CMD_SET_ID_longopt:
		{
			if (0) {
				;
			} else {
				goto BATON_CMD_NONE_longopt;
			}
			goto back_from_BATON_CMD_SET_ID_longopt;
		}
		BATON_CMD_GET_ID_longopt:
		{
			if (0) {
				;
			} else {
				goto BATON_CMD_NONE_longopt;
			}
			goto back_from_BATON_CMD_GET_ID_longopt;
		}
		BATON_CMD_GET_FIRMWARE_VERSION_longopt:
		{
			if (0) {
				;
			} else {
				goto BATON_CMD_NONE_longopt;
			}
			goto back_from_BATON_CMD_GET_FIRMWARE_VERSION_longopt;
		}
		BATON_CMD_TOGGLE_BITFIELD_longopt:
		{
			if (0) {
				;
			} else {
				goto BATON_CMD_NONE_longopt;
			}
			goto back_from_BATON_CMD_TOGGLE_BITFIELD_longopt;
		}
		BATON_CMD_READ_BITFIELD_longopt:
		{
			if (0) {
				;
			} else {
				goto BATON_CMD_NONE_longopt;
			}
			goto back_from_BATON_CMD_READ_BITFIELD_longopt;
		}
		
	}

	shortopt:
	{
		char *arg = op + 1U;

		switch (tgt->cmd) {
		default:
			goto BATON_CMD_NONE_shortopt; back_from_BATON_CMD_NONE_shortopt:;
			break;
		case BATON_CMD_ENABLE:
			goto BATON_CMD_ENABLE_shortopt; back_from_BATON_CMD_ENABLE_shortopt:;
			break;
		case BATON_CMD_DISABLE:
			goto BATON_CMD_DISABLE_shortopt; back_from_BATON_CMD_DISABLE_shortopt:;
			break;
		case BATON_CMD_READ:
			goto BATON_CMD_READ_shortopt; back_from_BATON_CMD_READ_shortopt:;
			break;
		case BATON_CMD_SET_ID:
			goto BATON_CMD_SET_ID_shortopt; back_from_BATON_CMD_SET_ID_shortopt:;
			break;
		case BATON_CMD_GET_ID:
			goto BATON_CMD_GET_ID_shortopt; back_from_BATON_CMD_GET_ID_shortopt:;
			break;
		case BATON_CMD_GET_FIRMWARE_VERSION:
			goto BATON_CMD_GET_FIRMWARE_VERSION_shortopt; back_from_BATON_CMD_GET_FIRMWARE_VERSION_shortopt:;
			break;
		case BATON_CMD_TOGGLE_BITFIELD:
			goto BATON_CMD_TOGGLE_BITFIELD_shortopt; back_from_BATON_CMD_TOGGLE_BITFIELD_shortopt:;
			break;
		case BATON_CMD_READ_BITFIELD:
			goto BATON_CMD_READ_BITFIELD_shortopt; back_from_BATON_CMD_READ_BITFIELD_shortopt:;
			break;
		}
		goto back_from_shortopt;


		BATON_CMD_NONE_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				fprintf(stderr, "baton: invalid option -%c\n", *op);
				goto failure;



				
			case 'h':
				/* invoke auto action and exit */
				yuck_auto_help(tgt);
				goto success;
				break;
			case 'V':
				/* invoke auto action and exit */
				yuck_auto_version(tgt);
				goto success;
				break;
			case 'd':
				tgt->device_arg = *arg
					? (op += strlen(arg), arg)
					: argv[++i];
				break;
			}
			goto back_from_BATON_CMD_NONE_shortopt;
		}
		BATON_CMD_ENABLE_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				goto BATON_CMD_NONE_shortopt;

				
			}
			goto back_from_BATON_CMD_ENABLE_shortopt;
		}
		BATON_CMD_DISABLE_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				goto BATON_CMD_NONE_shortopt;

				
			}
			goto back_from_BATON_CMD_DISABLE_shortopt;
		}
		BATON_CMD_READ_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				goto BATON_CMD_NONE_shortopt;

				
			}
			goto back_from_BATON_CMD_READ_shortopt;
		}
		BATON_CMD_SET_ID_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				goto BATON_CMD_NONE_shortopt;

				
			}
			goto back_from_BATON_CMD_SET_ID_shortopt;
		}
		BATON_CMD_GET_ID_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				goto BATON_CMD_NONE_shortopt;

				
			}
			goto back_from_BATON_CMD_GET_ID_shortopt;
		}
		BATON_CMD_GET_FIRMWARE_VERSION_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				goto BATON_CMD_NONE_shortopt;

				
			}
			goto back_from_BATON_CMD_GET_FIRMWARE_VERSION_shortopt;
		}
		BATON_CMD_TOGGLE_BITFIELD_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				goto BATON_CMD_NONE_shortopt;

				
			}
			goto back_from_BATON_CMD_TOGGLE_BITFIELD_shortopt;
		}
		BATON_CMD_READ_BITFIELD_shortopt:
		{
			switch (*op) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (op[-1] == '-') {
					/* literal treatment of numeral */
					goto arg;
				}
				/*@fallthrough@*/
			default:
				;
				;
				goto BATON_CMD_NONE_shortopt;

				
			}
			goto back_from_BATON_CMD_READ_BITFIELD_shortopt;
		}
		
	}

	arg:
	{
		if (tgt->cmd || YUCK_NCMDS == 0U) {
			tgt->args[tgt->nargs++] = argv[i];
		} else {
			/* ah, might be an arg then */
			if ((tgt->cmd = yuck_parse_cmd(op)) > YUCK_NCMDS) {
				return -1;
			}
		}
		goto back_from_arg;
	}

	failure:
	{
		exit(EXIT_FAILURE);
	}

	success:
	{
		exit(EXIT_SUCCESS);
	}
}

static void yuck_free(yuck_t tgt[static 1U])
{
	if (tgt->args != NULL) {
		/* free despite const qualifier */
		free(tgt->args);
	}
	/* free mulargs */
	switch (tgt->cmd) {
		void *ptr;
	default:
		break;
	case BATON_CMD_NONE:
;
;
;
		break;
	case BATON_CMD_ENABLE:
		break;
	case BATON_CMD_DISABLE:
		break;
	case BATON_CMD_READ:
		break;
	case BATON_CMD_SET_ID:
		break;
	case BATON_CMD_GET_ID:
		break;
	case BATON_CMD_GET_FIRMWARE_VERSION:
		break;
	case BATON_CMD_TOGGLE_BITFIELD:
		break;
	case BATON_CMD_READ_BITFIELD:
		break;
	}
	return;
}

static void yuck_auto_usage(const yuck_t src[static 1U])
{
	switch (src->cmd) {
	default:
	YUCK_NOCMD:
		puts("Usage: baton [OPTION]... COMMAND\n\
\n\
Baton - Library and command line interface for Numato USB Relay modules\n\
");
		break;

	case BATON_CMD_ENABLE:
		puts("Usage: baton enable [RELAY]...\n\
\n\
Enable relay number RELAY\n\
");
		break;

	case BATON_CMD_DISABLE:
		puts("Usage: baton disable [RELAY]...\n\
\n\
Disable relay number RELAY\n\
");
		break;

	case BATON_CMD_READ:
		puts("Usage: baton read [RELAY]...\n\
\n\
Get current relay status of relay number RELAY\n\
");
		break;

	case BATON_CMD_SET_ID:
		puts("Usage: baton set_id [ID]...\n\
\n\
Set module ID to ID\n\
");
		break;

	case BATON_CMD_GET_ID:
		puts("Usage: baton get_id\n\
\n\
Get current module ID\n\
");
		break;

	case BATON_CMD_GET_FIRMWARE_VERSION:
		puts("Usage: baton get_firmware_version\n\
\n\
Get module's firmware get_firmware_version\n\
");
		break;

	case BATON_CMD_TOGGLE_BITFIELD:
		puts("Usage: baton toggle_bitfield [BITFIELD]...\n\
\n\
Toggle multiple relays with a 16-bit bitfield\n\
");
		break;

	case BATON_CMD_READ_BITFIELD:
		puts("Usage: baton read_bitfield\n\
\n\
Get current relay status of multiple relays as a 16-bit bitfield\n\
");
		break;

	}

#if defined yuck_post_usage
	yuck_post_usage(src);
#endif	/* yuck_post_usage */
	return;
}

static void yuck_auto_help(const yuck_t src[static 1U])
{
	yuck_auto_usage(src);

	if (src->cmd == YUCK_NOCMD) {
		/* also output a list of commands */
		puts("COMMAND may be one of:\n\
  enable      Enable relay number RELAY\n\
  disable     Disable relay number RELAY\n\
  read        Get current relay status of relay number RELAY\n\
  set_id      Set module ID to ID\n\
  get_id      Get current module ID\n\
  get_firmware_version\n\
              Get module's firmware get_firmware_version\n\
  toggle_bitfield\n\
              Toggle multiple relays with a 16-bit bitfield\n\
  read_bitfield\n\
              Get current relay status of multiple relays as a 16-bit bitfield\n\
");
	}

	/* leave a not about common options */
	if (src->cmd == YUCK_NOCMD) {
		puts("\
Options accepted by all commands:");
	} else {
		puts("\
Common options:\n\
  -h, --help            display this help and exit\n\
  -V, --version         output version information and exit\n\
  -d, --device=DEVICE   Serial device corresponding to the relay module.\n\
");
	}

	switch (src->cmd) {
	default:
	case BATON_CMD_NONE:
		puts("\
  -h, --help            display this help and exit\n\
  -V, --version         output version information and exit\n\
  -d, --device=DEVICE   Serial device corresponding to the relay module.\n\
");
		break;

	case BATON_CMD_ENABLE:
		puts("\
");
		break;

	case BATON_CMD_DISABLE:
		puts("\
");
		break;

	case BATON_CMD_READ:
		puts("\
");
		break;

	case BATON_CMD_SET_ID:
		puts("\
");
		break;

	case BATON_CMD_GET_ID:
		puts("\
");
		break;

	case BATON_CMD_GET_FIRMWARE_VERSION:
		puts("\
");
		break;

	case BATON_CMD_TOGGLE_BITFIELD:
		puts("\
");
		break;

	case BATON_CMD_READ_BITFIELD:
		puts("\
");
		break;

	}

#if defined yuck_post_help
	yuck_post_help(src);
#endif	/* yuck_post_help */

#if defined PACKAGE_BUGREPORT
	puts("\n\
Report bugs to " PACKAGE_BUGREPORT);
#endif	/* PACKAGE_BUGREPORT */
	return;
}

static void yuck_auto_version(const yuck_t src[static 1U])
{
	switch (src->cmd) {
	default:
#if 0

#elif defined package_string
		puts(package_string);
#elif defined package_version
		printf("baton %s\n", package_version);
#elif defined PACKAGE_STRING
		puts(PACKAGE_STRING);
#elif defined PACKAGE_VERSION
		puts("baton " PACKAGE_VERSION);
#elif defined VERSION
		puts("baton " VERSION);
#else  /* !PACKAGE_VERSION, !VERSION */
		puts("baton unknown version");
#endif	/* PACKAGE_VERSION */
		break;
	}

#if defined yuck_post_version
	yuck_post_version(src);
#endif	/* yuck_post_version */
	return;
}

#if defined __INTEL_COMPILER
# pragma warning (pop)
#elif defined __GNUC__
# if __GNUC__ > 4 || __GNUC__ == 4 &&  __GNUC_MINOR__ >= 6
#  pragma GCC diagnostic pop
# endif	 /* GCC version */
#endif	/* __INTEL_COMPILER */
