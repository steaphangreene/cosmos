enum {PAGE_INVALID = 0,
	PAGE_ROOT,
	PAGE_NEW,
	PAGE_LOAD,
	PAGE_NET,
	PAGE_SYSOPT,
	PAGE_GALAXY,
	PAGE_SYSTEM,
	PAGE_PLANET,
	PAGE_DIALOG,
	PAGE_MAX
	};

enum {PANEL_NONE = 0,
	PANEL_ROOT,
	PANEL_NEW,
	PANEL_LOAD,
	PANEL_NET,
	PANEL_SYSOPT,
	PANEL_GAME, //Order important!  All after are in-game!
	PANEL_SYSTEM,
	PANEL_PLANET,
	PANEL_COLONY,
	PANEL_FLEET,
	PANEL_SHIP,
	PANEL_MAX
	};

enum {BUTTON_INVALID = 0,
	BUTTON_CANCEL,
	BUTTON_ACCEPT,
	BUTTON_EXIT,
	BUTTON_TURN,
	BUTTON_NEWPROJECT,
	BUTTON_CANCELPROJECT,
	BUTTON_CANCELBUILD,
	BUTTON_BUILD,
	BUTTON_ABANDON,
	BUTTON_LAND,
	BUTTON_SPLIT,
	BUTTON_OPTIONS,
	BUTTON_RANDOMIZE,
	BUTTON_RESETALL,
	BUTTON_CLEARALL,
	BUTTON_RESUMEGAME,
	BUTTON_NEWGAME,
	BUTTON_LOADGAME,
	BUTTON_NETWORKGAME,
	BUTTON_SYSTEMOPTIONS,
	BUTTON_QUITGAME,
	BUTTON_MAX
	};

void gui_init();
void gui_main();
void do_dialog(const char *, ...)
	__attribute__ ((format (printf, 1, 2)));
