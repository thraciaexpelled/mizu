// structs.h - data structures for mizu
// * this file is a part of `mizu`
// * mizu is MIT licensed by its authors
// * it's <<Fri 03 Oct 2025 08:47:30 PM>> here...

#include <stdbool.h>

#ifndef MIZU_SUBCOMMANDS
#define MIZU_SUBCOMMANDS

// this is both a *char[] and enum representation of the subcommands
extern const char *subcommands[];

// index for 'subcommands'
extern int subcommand_index;

// enum representation of 'subcommands'
typedef enum {
	New 	= 0,
	Init 	= 1,
} Subcommand;

// enum for our project types. this makes us able to work with
// checking project type of an project without having to use
// repetitive subroutines like `strcmp()`, to an extent, which 
// could also potentionally reduce performance and kill code
// readability.
typedef enum {
	Program,
	Library,
} ProjectType;

// this defines a pretty general mizu project.
typedef struct {
	const char *name;			// the name of the project.
	ProjectType project_type;	// will be converted into it's symbolic name via a static function.
	const char *license;		// will be defaulted to MIT.
	const char *version;		// will be defaulted to "alpha-0.1".
} subc_new_t;

// the same as subc_new_t, but 'subc_init_t->name' is optional
typedef struct {
	const char *name;			// will be defaulted to the name of the cwd.
	ProjectType project_type;	// will be converted into it's symbolic name via a static function.
	const char *license;		// will be defaulted to MIT.
	const char *version;		// will be defaulted to "alpha-0.1".
} subc_init_t;

#endif // MIZU_SUBCOMMANDS