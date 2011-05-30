#ifdef LUACONSOLE
#include <powder.h>
#include <console.h>
#include <luaconsole.h>

lua_State *l;
void luacon_open(){
	const static struct luaL_reg tptluaapi [] = {
		{"test", &luatpt_test},
		{"drawtext", &luatpt_drawtext},
		{"create", &luatpt_create},
		{"set_pause", &luatpt_setpause},
		{"toggle_pause", &luatpt_togglepause},
		{"set_console", &luatpt_setconsole},
		{"log", &luatpt_log},
		{"reset_pressure", &luatpt_reset_pressure},
		{"reset_velocity", &luatpt_reset_velocity},
		{"reset_spark", &luatpt_reset_spark},
		{"set_property", &luatpt_set_property},
		{"get_property", &luatpt_get_property},
		{"drawpixel", &luatpt_drawpixel},
		{"drawrect", &luatpt_drawrect},
		{"fillrect", &luatpt_fillrect},
		{"textwidth", &luatpt_textwidth},
		{"get_name", &luatpt_get_name},
		{"set_shortcuts", &luatpt_set_shortcuts},
		{"delete", &luatpt_delete},
		{NULL,NULL}
	};

	l = lua_open();
	luaL_openlibs(l);
	luaL_openlib(l, "tpt", tptluaapi, 0);
}
int luacon_step(){
	//Nothing here yet
	return 0;
}
int luacon_keypress(char key){
	//Nothing here yet
	return 0;
}
int luacon_eval(char *command){
	return luaL_dostring (l, command);
}
char *luacon_geterror(){
	char *error = lua_tostring(l, -1);
	if(error==NULL || !error[0]){
		error = "failed to execute";	
	}
	return error;
}
void luacon_close(){
	lua_close(l);
}
int process_command_lua(pixel *vid_buf, char *console, char *console_error)
{
	int commandret;
	char console2[15];
	char console3[15];
	char console4[15];
	char console5[15];
	//sprintf(console_error, "%s", console);
	if (console && strcmp(console, "")!=0 && strncmp(console, " ", 1)!=0)
	{
		sscanf(console,"%14s %14s %14s %14s", console2, console3, console4, console5);
		if (strcmp(console2, "quit")==0)
		{
			return -1;
		} else {
			commandret = luacon_eval(console);
			if (commandret)
				strcpy(console_error, luacon_geterror());
		}
	}
	return 1;
}
//Being TPT interface methods:
int luatpt_test(lua_State* l)
{
    int testint = 0;
	testint = luaL_optint(l, 1, 0);
	printf("Test successful, got %d\n", testint);
	return 0;
}
int luatpt_drawtext(lua_State* l)
{
    char *string;
	int textx, texty, textred, textgreen, textblue, textalpha;
	textx = luaL_optint(l, 1, 0);
	texty = luaL_optint(l, 2, 0);
	string = luaL_optstring(l, 3, "");
	textred = luaL_optint(l, 4, 255);
	textgreen = luaL_optint(l, 5, 255);
	textblue = luaL_optint(l, 6, 255);
	textalpha = luaL_optint(l, 7, 255);
	if(vid_buf!=NULL){
		drawtext(vid_buf, textx, texty, string, textred, textgreen, textblue, textalpha);
		return 0;	
	}
	return -1;
}

int luatpt_create(lua_State* l)
{
	lua_pushstring(l, "not implemented");
	lua_error(l);
	//TODO: Implement luatpt_create (Create a particle)
	return -1;
}

int luatpt_setpause(lua_State* l)
{
	int pausestate;
	pausestate = luaL_optint(l, 1, 0);
	sys_pause = (pausestate==0?0:1);
	return 0;
}

int luatpt_togglepause(lua_State* l)
{
	sys_pause=!sys_pause;
	return 0;
}

int luatpt_setconsole(lua_State* l)
{
	int consolestate;
	consolestate = luaL_optint(l, 1, 0);
	console_mode = (consolestate==0?0:1);
	return 0;
}

int luatpt_log(lua_State* l)
{
	char *buffer;
	buffer = luaL_optstring(l, 1, "");
	strcpy(console_error, buffer);
	return 0;
}

int luatpt_reset_pressure(lua_State* l)
{
	int nx, ny;
	int x1, y1, width, height;
	float value;
	x1 = abs(luaL_optint(l, 1, 0));
	y1 = abs(luaL_optint(l, 2, 0));
	width = abs(luaL_optint(l, 3, XRES/CELL));
	height = abs(luaL_optint(l, 4, YRES/CELL));
	value = (float)luaL_optint(l, 5, 0.0f);
	if(value > 256.0f)
		value = 256.0f;
	else if(value < -256.0f)
		value = -256.0f;	
	
	if(x1 > (XRES/CELL)-1)
		x1 = (XRES/CELL)-1;
	if(y1 > (YRES/CELL)-1) 
		y1 = (YRES/CELL)-1;	
	if(x1+width > (XRES/CELL)-1)
		width = (XRES/CELL)-x1;
	if(y1+height > (YRES/CELL)-1)
		height = (YRES/CELL)-y1;
	for (nx = x1; nx<x1+width; nx++)
		for (ny = y1; ny<y1+height; ny++)
		{
			pv[ny][nx] = value;
		}
	return 0;
}

int luatpt_reset_velocity(lua_State* l)
{
	int nx, ny;
	int x1, y1, width, height;
	x1 = abs(luaL_optint(l, 1, 0));
	y1 = abs(luaL_optint(l, 2, 0));
	width = abs(luaL_optint(l, 3, XRES/CELL));
	height = abs(luaL_optint(l, 4, YRES/CELL));
	if(x1 > (XRES/CELL)-1)
		x1 = (XRES/CELL)-1;
	if(y1 > (YRES/CELL)-1) 
		y1 = (YRES/CELL)-1;	
	if(x1+width > (XRES/CELL)-1)
		width = (XRES/CELL)-x1;
	if(y1+height > (YRES/CELL)-1)
		height = (YRES/CELL)-y1;
	for (nx = x1; nx<x1+width; nx++)
		for (ny = y1; ny<y1+height; ny++)
		{
			vx[ny][nx] = 0;
			vy[ny][nx] = 0;
		}
	return 0;
}

int luatpt_reset_spark(lua_State* l)
{
	int i;
	for (i=0; i<NPART; i++)
	{
		if (parts[i].type==PT_SPRK)
		{
			parts[i].type = parts[i].ctype;
			parts[i].life = 4;
		}
	}
	return 0;
}

int luatpt_set_property(lua_State* l)
{
	lua_pushstring(l, "not implemented");
	lua_error(l);
	//TODO: Implement luatpt_set_property
	return -1;
}

int luatpt_get_property(lua_State* l)
{
	int i;
	char *prop;
	prop = luaL_optstring(l, 1, "");
	i = luaL_optint(l, 2, 0);
	if (i < 0 || i >= NPART)
		return -1;
	if (parts[i].type)
	{
		if (strcmp(prop,"type")==0){
			lua_pushinteger(l, parts[i].type);
			return 1;
		}
		if (strcmp(prop,"life")==0){
			lua_pushinteger(l, parts[i].life);
			return 1;
		}
		if (strcmp(prop,"ctype")==0){
			lua_pushinteger(l, parts[i].ctype);
			return 1;
		}
		if (strcmp(prop,"temp")==0){
			lua_pushinteger(l, parts[i].temp);
			return 1;
		}
		if (strcmp(prop,"tmp")==0){
			lua_pushinteger(l, parts[i].tmp);
			return 1;
		}
		if (strcmp(prop,"vy")==0){
			lua_pushnumber(l, (double)parts[i].vy);
			return 1;
		}
		if (strcmp(prop,"vx")==0){
			lua_pushnumber(l, (double)parts[i].vx);
			return 1;
		}
		if (strcmp(prop,"x")==0){
			lua_pushinteger(l, parts[i].x);
			return 1;
		}
		if (strcmp(prop,"y")==0){
			lua_pushinteger(l, parts[i].y);
			return 1;
		}
	}

	return -1;
}

int luatpt_drawpixel(lua_State* l)
{
	int x, y, r, g, b, a;
	x = luaL_optint(l, 1, 0);
	y = luaL_optint(l, 2, 0);
	r = luaL_optint(l, 3, 255);
	g = luaL_optint(l, 4, 255);
	b = luaL_optint(l, 5, 255);
	a = luaL_optint(l, 6, 255);

	if(x > XRES || y > YRES || x < 0 || y < 0)
		return -1;
	if (vid_buf!=NULL)
	{
		drawpixel(vid_buf, x, y, r, g, b, a);
		return 0;
	}
	return -1;

}

int luatpt_drawrect(lua_State* l)
{
	int x, y, w, h, r, g, b, a;
	x = luaL_optint(l, 1, 0);
	y = luaL_optint(l, 2, 0);
	w = luaL_optint(l, 3, 10);
	h = luaL_optint(l, 4, 10);
	r = luaL_optint(l, 5, 255);
	g = luaL_optint(l, 6, 255);
	b = luaL_optint(l, 7, 255);
	a = luaL_optint(l, 8, 255);

	if(x > XRES || y > YRES || x < 0 || y < 0)
		return -1;
	if (vid_buf!=NULL)
	{
		drawrect(vid_buf, x, y, w, h, r, g, b, a);
		return 0;
	}
	return -1;
}

int luatpt_fillrect(lua_State* l)
{
	int x,y,w,h,r,g,b,a;
	x = luaL_optint(l, 1, 0);
	y = luaL_optint(l, 2, 0);
	w = luaL_optint(l, 3, 10);
	h = luaL_optint(l, 4, 10);
	r = luaL_optint(l, 5, 255);
	g = luaL_optint(l, 6, 255);
	b = luaL_optint(l, 7, 255);
	a = luaL_optint(l, 8, 255);

	if(x > XRES || y > YRES || x < 0 || y < 0)
		return -1;
	if (vid_buf!=NULL)
	{
		fillrect(vid_buf, x, y, w, h, r, g, b, a);
		return 0;
	}
	return -1;
}

int luatpt_textwidth(lua_State* l)
{
	char * string;
	int strwidth = 0;
	string = luaL_optstring(l, 1, "");
	strwidth = textwidth(string);
	lua_pushinteger(l, strwidth);
	return 1;
}

int luatpt_get_name(lua_State* l)
{
	if (svf_login){
		lua_pushstring(l, "");
		return 1;
	}
	lua_pushstring(l, "");
	return 1;
}

int luatpt_set_shortcuts(lua_State* l)
{
	int state;
	state = luaL_optint(l, 1, 0);
	sys_shortcuts = (state==0?0:1);
	return 0;
}

int luatpt_delete(lua_State* l)
{
	lua_pushstring(l, "not implemented");
	lua_error(l);
	//TODO: Implement luatpt_delete (Delete particle)
	return -1;
}
#endif