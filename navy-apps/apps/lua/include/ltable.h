/*
** $Id: ltable.h,v 2.23 2016/12/22 13:08:50 roberto Exp $
** Lua tables (hash)
** See Copyright Notice in lua.h
*/

#ifndef ltable_h
#define ltable_h

#include "lobject.h"

#define gnode(t, i) (&(t)->node[i])
#define gval(n) (&(n)->i_val)
#define gnext(n) ((n)->i_key.nk.next)

/* 'const' to avoid wrong writings that can mess up field 'next' */
<<<<<<< HEAD
#define gkey(n) cast(const TValue*, (&(n)->i_key.tvk))
=======
#define gkey(n) cast(const TValue *, (&(n)->i_key.tvk))
>>>>>>> master

/*
** writable version of 'gkey'; allows updates to individual fields,
** but not to the whole (which has incompatible type)
*/
#define wgkey(n) (&(n)->i_key.nk)

#define invalidateTMcache(t) ((t)->flags = 0)

/* true when 't' is using 'dummynode' as its hash part */
#define isdummy(t) ((t)->lastfree == NULL)

/* allocated size for hash nodes */
#define allocsizenode(t) (isdummy(t) ? 0 : sizenode(t))

/* returns the key, given the value of a table entry */
<<<<<<< HEAD
#define keyfromval(v) (gkey(cast(Node*, cast(char*, (v)) - offsetof(Node, i_val))))

LUAI_FUNC const TValue* luaH_getint(Table* t, lua_Integer key);
LUAI_FUNC void luaH_setint(lua_State* L, Table* t, lua_Integer key, TValue* value);
LUAI_FUNC const TValue* luaH_getshortstr(Table* t, TString* key);
LUAI_FUNC const TValue* luaH_getstr(Table* t, TString* key);
LUAI_FUNC const TValue* luaH_get(Table* t, const TValue* key);
LUAI_FUNC TValue* luaH_newkey(lua_State* L, Table* t, const TValue* key);
LUAI_FUNC TValue* luaH_set(lua_State* L, Table* t, const TValue* key);
LUAI_FUNC Table* luaH_new(lua_State* L);
LUAI_FUNC void luaH_resize(lua_State* L, Table* t, unsigned int nasize, unsigned int nhsize);
LUAI_FUNC void luaH_resizearray(lua_State* L, Table* t, unsigned int nasize);
LUAI_FUNC void luaH_free(lua_State* L, Table* t);
LUAI_FUNC int luaH_next(lua_State* L, Table* t, StkId key);
LUAI_FUNC int luaH_getn(Table* t);

#if defined(LUA_DEBUG)
LUAI_FUNC Node* luaH_mainposition(const Table* t, const TValue* key);
LUAI_FUNC int luaH_isdummy(const Table* t);
=======
#define keyfromval(v)                                                          \
  (gkey(cast(Node *, cast(char *, (v)) - offsetof(Node, i_val))))

LUAI_FUNC const TValue *luaH_getint(Table *t, lua_Integer key);
LUAI_FUNC void luaH_setint(lua_State *L, Table *t, lua_Integer key,
                           TValue *value);
LUAI_FUNC const TValue *luaH_getshortstr(Table *t, TString *key);
LUAI_FUNC const TValue *luaH_getstr(Table *t, TString *key);
LUAI_FUNC const TValue *luaH_get(Table *t, const TValue *key);
LUAI_FUNC TValue *luaH_newkey(lua_State *L, Table *t, const TValue *key);
LUAI_FUNC TValue *luaH_set(lua_State *L, Table *t, const TValue *key);
LUAI_FUNC Table *luaH_new(lua_State *L);
LUAI_FUNC void luaH_resize(lua_State *L, Table *t, unsigned int nasize,
                           unsigned int nhsize);
LUAI_FUNC void luaH_resizearray(lua_State *L, Table *t, unsigned int nasize);
LUAI_FUNC void luaH_free(lua_State *L, Table *t);
LUAI_FUNC int luaH_next(lua_State *L, Table *t, StkId key);
LUAI_FUNC int luaH_getn(Table *t);

#if defined(LUA_DEBUG)
LUAI_FUNC Node *luaH_mainposition(const Table *t, const TValue *key);
LUAI_FUNC int luaH_isdummy(const Table *t);
>>>>>>> master
#endif

#endif
