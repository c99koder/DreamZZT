/*
** Lua binding: zzt
** Generated automatically by tolua++-1.0.92 on Tue Jun  5 20:19:11 2007.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_zzt_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_TUIHyperLink (lua_State* tolua_S)
{
 TUIHyperLink* self = (TUIHyperLink*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUILabel (lua_State* tolua_S)
{
 TUILabel* self = (TUILabel*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUIRadioGroup (lua_State* tolua_S)
{
 TUIRadioGroup* self = (TUIRadioGroup*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUIPasswordInput (lua_State* tolua_S)
{
 TUIPasswordInput* self = (TUIPasswordInput*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUITextInput (lua_State* tolua_S)
{
 TUITextInput* self = (TUITextInput*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_ZZTObject (lua_State* tolua_S)
{
 ZZTObject* self = (ZZTObject*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUINumericInput (lua_State* tolua_S)
{
 TUINumericInput* self = (TUINumericInput*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUICheckBox (lua_State* tolua_S)
{
 TUICheckBox* self = (TUICheckBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUISlider (lua_State* tolua_S)
{
 TUISlider* self = (TUISlider*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUIWindow (lua_State* tolua_S)
{
 TUIWindow* self = (TUIWindow*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUIDirection (lua_State* tolua_S)
{
 TUIDirection* self = (TUIDirection*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Vector (lua_State* tolua_S)
{
 Vector* self = (Vector*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TUIBoardList (lua_State* tolua_S)
{
 TUIBoardList* self = (TUIBoardList*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"TUIHyperLink");
 tolua_usertype(tolua_S,"TUILabel");
 tolua_usertype(tolua_S,"TUIRadioGroup");
 tolua_usertype(tolua_S,"TUIPasswordInput");
 tolua_usertype(tolua_S,"TUIWidget");
 tolua_usertype(tolua_S,"TUITextInput");
 tolua_usertype(tolua_S,"TUIDirection");
 tolua_usertype(tolua_S,"ZZTObject");
 tolua_usertype(tolua_S,"TUISlider");
 tolua_usertype(tolua_S,"TUICheckBox");
 tolua_usertype(tolua_S,"TUINumericInput");
 tolua_usertype(tolua_S,"TUIWindow");
 tolua_usertype(tolua_S,"board_info_node");
 tolua_usertype(tolua_S,"TUIBoardList");
 tolua_usertype(tolua_S,"Vector");
}

/* method: new of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_new00
static int tolua_zzt_Vector_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float ix = ((float)  tolua_tonumber(tolua_S,2,0));
  float iy = ((float)  tolua_tonumber(tolua_S,3,0));
  float iz = ((float)  tolua_tonumber(tolua_S,4,0));
  float iw = ((float)  tolua_tonumber(tolua_S,5,1.0f));
  {
   Vector* tolua_ret = (Vector*)  new Vector(ix,iy,iz,iw);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_new00_local
static int tolua_zzt_Vector_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float ix = ((float)  tolua_tonumber(tolua_S,2,0));
  float iy = ((float)  tolua_tonumber(tolua_S,3,0));
  float iz = ((float)  tolua_tonumber(tolua_S,4,0));
  float iw = ((float)  tolua_tonumber(tolua_S,5,1.0f));
  {
   Vector* tolua_ret = (Vector*)  new Vector(ix,iy,iz,iw);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Vector");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_new01
static int tolua_zzt_Vector_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   Vector* tolua_ret = (Vector*)  new Vector();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_Vector_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_new01_local
static int tolua_zzt_Vector_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   Vector* tolua_ret = (Vector*)  new Vector();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Vector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_Vector_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector__geti00
static int tolua_zzt_Vector__geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   float tolua_ret = (float)  self->operator[](i);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector__eq00
static int tolua_zzt_Vector__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  const Vector* other = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector__add00
static int tolua_zzt_Vector__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  const Vector* other = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->operator+(*other);
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector__sub00
static int tolua_zzt_Vector__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  const Vector* other = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->operator-(*other);
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector__sub01
static int tolua_zzt_Vector__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->operator-();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_Vector__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector__mul00
static int tolua_zzt_Vector__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  float s = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->operator*(s);
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: zero of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_zero00
static int tolua_zzt_Vector_zero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'zero'",NULL);
#endif
  {
   self->zero();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'zero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dot of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_dot00
static int tolua_zzt_Vector_dot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  const Vector* other = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dot'",NULL);
#endif
  {
   float tolua_ret = (float)  self->dot(*other);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cross of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_cross00
static int tolua_zzt_Vector_cross00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  const Vector* other = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cross'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->cross(*other);
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cross'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: length of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_length00
static int tolua_zzt_Vector_length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'length'",NULL);
#endif
  {
   float tolua_ret = (float)  self->length();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'length'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rlength of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_rlength00
static int tolua_zzt_Vector_rlength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rlength'",NULL);
#endif
  {
   float tolua_ret = (float)  self->rlength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rlength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: normalizeSelf of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_normalizeSelf00
static int tolua_zzt_Vector_normalizeSelf00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'normalizeSelf'",NULL);
#endif
  {
   Vector& tolua_ret = (Vector&)  self->normalizeSelf();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Vector");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'normalizeSelf'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: normalize of class  Vector */
#ifndef TOLUA_DISABLE_tolua_zzt_Vector_normalize00
static int tolua_zzt_Vector_normalize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'normalize'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->normalize();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'normalize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Vector */
#ifndef TOLUA_DISABLE_tolua_get_Vector_x
static int tolua_get_Vector_x(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Vector */
#ifndef TOLUA_DISABLE_tolua_set_Vector_x
static int tolua_set_Vector_x(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Vector */
#ifndef TOLUA_DISABLE_tolua_get_Vector_y
static int tolua_get_Vector_y(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Vector */
#ifndef TOLUA_DISABLE_tolua_set_Vector_y
static int tolua_set_Vector_y(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Vector */
#ifndef TOLUA_DISABLE_tolua_get_Vector_z
static int tolua_get_Vector_z(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Vector */
#ifndef TOLUA_DISABLE_tolua_set_Vector_z
static int tolua_set_Vector_z(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: w of class  Vector */
#ifndef TOLUA_DISABLE_tolua_get_Vector_w
static int tolua_get_Vector_w(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->w);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: w of class  Vector */
#ifndef TOLUA_DISABLE_tolua_set_Vector_w
static int tolua_set_Vector_w(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->w = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_new00
static int tolua_zzt_zzt_ZZTObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int shape = ((int)  tolua_tonumber(tolua_S,5,0));
  int flags = ((int)  tolua_tonumber(tolua_S,6,0));
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  new ZZTObject(type,x,y,shape,flags,name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_new00_local
static int tolua_zzt_zzt_ZZTObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,0));
  int y = ((int)  tolua_tonumber(tolua_S,4,0));
  int shape = ((int)  tolua_tonumber(tolua_S,5,0));
  int flags = ((int)  tolua_tonumber(tolua_S,6,0));
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  new ZZTObject(type,x,y,shape,flags,name);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ZZTObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: opposite of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_opposite00
static int tolua_zzt_zzt_ZZTObject_opposite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  enum direction dir = ((enum direction) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'opposite'",NULL);
#endif
  {
   direction tolua_ret = (direction)  self->opposite(dir);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'opposite'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toward of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_toward00
static int tolua_zzt_zzt_ZZTObject_toward00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ZZTObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  ZZTObject* them = ((ZZTObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toward'",NULL);
#endif
  {
   direction tolua_ret = (direction)  self->toward(them);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toward'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clockwise of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_clockwise00
static int tolua_zzt_zzt_ZZTObject_clockwise00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  direction dir = ((direction) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clockwise'",NULL);
#endif
  {
   direction tolua_ret = (direction)  self->clockwise(dir);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clockwise'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: str_to_direction of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_str_to_direction00
static int tolua_zzt_zzt_ZZTObject_str_to_direction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  std::string s = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'str_to_direction'",NULL);
#endif
  {
   direction tolua_ret = (direction)  self->str_to_direction(s);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'str_to_direction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isValid of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_isValid00
static int tolua_zzt_zzt_ZZTObject_isValid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isValid'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isValid();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isValid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: str_to_color of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_str_to_color00
static int tolua_zzt_zzt_ZZTObject_str_to_color00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  std::string color = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'str_to_color'",NULL);
#endif
  {
   int tolua_ret = (int)  self->str_to_color(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'str_to_color'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: int_to_color of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_int_to_color00
static int tolua_zzt_zzt_ZZTObject_int_to_color00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  int color = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'int_to_color'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->int_to_color(color);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'int_to_color'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: distance of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_distance00
static int tolua_zzt_zzt_ZZTObject_distance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ZZTObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  ZZTObject* them = ((ZZTObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'distance'",NULL);
#endif
  {
   int tolua_ret = (int)  self->distance(them);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'distance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dist_x of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_dist_x00
static int tolua_zzt_zzt_ZZTObject_dist_x00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ZZTObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  ZZTObject* them = ((ZZTObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dist_x'",NULL);
#endif
  {
   int tolua_ret = (int)  self->dist_x(them);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dist_x'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dist_y of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_dist_y00
static int tolua_zzt_zzt_ZZTObject_dist_y00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ZZTObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  ZZTObject* them = ((ZZTObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dist_y'",NULL);
#endif
  {
   int tolua_ret = (int)  self->dist_y(them);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dist_y'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: move of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_move00
static int tolua_zzt_zzt_ZZTObject_move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  direction d = ((direction) (int)  tolua_tonumber(tolua_S,2,0));
  bool trying = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool origin = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'move'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->move(d,trying,origin);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_empty of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_is_empty00
static int tolua_zzt_zzt_ZZTObject_is_empty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  direction d = ((direction) (int)  tolua_tonumber(tolua_S,2,0));
  bool ignorePlayer = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_empty'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_empty(d,ignorePlayer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_empty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create_object of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_create_object00
static int tolua_zzt_zzt_ZZTObject_create_object00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  direction d = ((direction) (int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create_object'",NULL);
#endif
  {
   ZZTObject* tolua_ret = (ZZTObject*)  self->create_object(type,d);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_object'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: type of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_type
static int tolua_get_ZZTObject_type(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->type());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: name of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_name
static int tolua_get_ZZTObject_name(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->name());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: name of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_name
static int tolua_set_ZZTObject_name(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setName(((std::string)  tolua_tocppstring(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: position of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_position
static int tolua_get_ZZTObject_position(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->position(),"Vector");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: position of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_position
static int tolua_set_ZZTObject_position(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Vector",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setPosition(*((Vector*)  tolua_tousertype(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: step of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_step
static int tolua_get_ZZTObject_step(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'step'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->step(),"Vector");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: step of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_step
static int tolua_set_ZZTObject_step(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'step'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Vector",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setStep(*((Vector*)  tolua_tousertype(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: heading of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_heading
static int tolua_get_ZZTObject_heading(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heading'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->heading());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: heading of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_heading
static int tolua_set_ZZTObject_heading(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heading'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setHeading(((direction) (int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: shape of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_shape
static int tolua_get_ZZTObject_shape(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'shape'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->shape());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: shape of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_shape
static int tolua_set_ZZTObject_shape(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'shape'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setShape(((char)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: color of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_color
static int tolua_get_ZZTObject_color(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'color'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->color());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: color of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_color
static int tolua_set_ZZTObject_color(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'color'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setColor(((int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: fg of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_fg
static int tolua_get_ZZTObject_fg(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fg'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->fg());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: fg of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_fg
static int tolua_set_ZZTObject_fg(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fg'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setFg(((int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: bg of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_bg
static int tolua_get_ZZTObject_bg(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bg'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->bg());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: bg of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_bg
static int tolua_set_ZZTObject_bg(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bg'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setBg(((int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: cycle of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_cycle
static int tolua_get_ZZTObject_cycle(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cycle'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->cycle());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: cycle of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_cycle
static int tolua_set_ZZTObject_cycle(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cycle'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setCycle(((short)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tick of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_tick
static int tolua_get_ZZTObject_tick(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tick'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->tick());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tick of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_tick
static int tolua_set_ZZTObject_tick(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tick'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setTick(((int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: updated of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_updated
static int tolua_get_ZZTObject_updated(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'updated'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->updated());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: updated of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_updated
static int tolua_set_ZZTObject_updated(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'updated'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setUpdated(((bool)  tolua_toboolean(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pushed of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_pushed
static int tolua_get_ZZTObject_pushed(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pushed'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->pushed());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pushed of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_pushed
static int tolua_set_ZZTObject_pushed(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pushed'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setPushed(((bool)  tolua_toboolean(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFlag of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_setFlag00
static int tolua_zzt_zzt_ZZTObject_setFlag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  int flag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFlag'",NULL);
#endif
  {
   self->setFlag(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFlag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: flag of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_flag00
static int tolua_zzt_zzt_ZZTObject_flag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  int f = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'flag'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->flag(f);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: flags of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_get_ZZTObject_flags
static int tolua_get_ZZTObject_flags(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'flags'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->flags());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: flags of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_set_ZZTObject_flags
static int tolua_set_ZZTObject_flags(lua_State* tolua_S)
{
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'flags'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->setFlags(((int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: setParam of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_setParam00
static int tolua_zzt_zzt_ZZTObject_setParam00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  int arg = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char val = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setParam'",NULL);
#endif
  {
   self->setParam(arg,val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setParam'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: param of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_param00
static int tolua_zzt_zzt_ZZTObject_param00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  int arg = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'param'",NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->param(arg);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'param'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: message of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_message00
static int tolua_zzt_zzt_ZZTObject_message00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ZZTObject",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  ZZTObject* from = ((ZZTObject*)  tolua_tousertype(tolua_S,2,0));
  std::string msg = ((std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'message'",NULL);
#endif
  {
   self->message(from,msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'message'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: shoot of class  ZZTObject */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_ZZTObject_shoot00
static int tolua_zzt_zzt_ZZTObject_shoot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* self = (ZZTObject*)  tolua_tousertype(tolua_S,1,0);
  direction d = ((direction) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'shoot'",NULL);
#endif
  {
   self->shoot(d);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'shoot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: create_object */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_create_object00
static int tolua_zzt_zzt_create_object00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int type = ((int)  tolua_tonumber(tolua_S,1,0));
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  create_object(type,x,y);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_object'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: create_copy */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_create_copy00
static int tolua_zzt_zzt_create_copy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* source = ((ZZTObject*)  tolua_tousertype(tolua_S,1,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  create_copy(source);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_copy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: size of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_size
static int tolua_get_board_info_node_unsigned_size(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'size'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->size);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: size of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_size
static int tolua_set_board_info_node_unsigned_size(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'size'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->size = ((unsigned short int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: title of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_title
static int tolua_get_board_info_node_title(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'title'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->title);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: title of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_title
static int tolua_set_board_info_node_title(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'title'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->title,tolua_tostring(tolua_S,2,0),50-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: num of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_num
static int tolua_get_board_info_node_num(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'num'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->num);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: num of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_num
static int tolua_set_board_info_node_num(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'num'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->num = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: maxshots of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_maxshots
static int tolua_get_board_info_node_unsigned_maxshots(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'maxshots'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->maxshots);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: maxshots of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_maxshots
static int tolua_set_board_info_node_unsigned_maxshots(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'maxshots'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->maxshots = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dark of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_dark
static int tolua_get_board_info_node_unsigned_dark(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dark'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->dark);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dark of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_dark
static int tolua_set_board_info_node_unsigned_dark(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dark'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dark = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: board_up of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_board_up
static int tolua_get_board_info_node_unsigned_board_up(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'board_up'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->board_up);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: board_up of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_board_up
static int tolua_set_board_info_node_unsigned_board_up(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'board_up'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->board_up = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: board_down of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_board_down
static int tolua_get_board_info_node_unsigned_board_down(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'board_down'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->board_down);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: board_down of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_board_down
static int tolua_set_board_info_node_unsigned_board_down(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'board_down'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->board_down = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: board_left of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_board_left
static int tolua_get_board_info_node_unsigned_board_left(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'board_left'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->board_left);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: board_left of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_board_left
static int tolua_set_board_info_node_unsigned_board_left(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'board_left'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->board_left = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: board_right of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_board_right
static int tolua_get_board_info_node_unsigned_board_right(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'board_right'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->board_right);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: board_right of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_board_right
static int tolua_set_board_info_node_unsigned_board_right(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'board_right'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->board_right = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: reenter of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_reenter
static int tolua_get_board_info_node_unsigned_reenter(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'reenter'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->reenter);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: reenter of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_reenter
static int tolua_set_board_info_node_unsigned_reenter(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'reenter'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->reenter = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: reenter_x of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_reenter_x
static int tolua_get_board_info_node_unsigned_reenter_x(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'reenter_x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->reenter_x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: reenter_x of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_reenter_x
static int tolua_set_board_info_node_unsigned_reenter_x(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'reenter_x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->reenter_x = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: reenter_y of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_reenter_y
static int tolua_get_board_info_node_unsigned_reenter_y(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'reenter_y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->reenter_y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: reenter_y of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_reenter_y
static int tolua_set_board_info_node_unsigned_reenter_y(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'reenter_y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->reenter_y = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: time of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_time
static int tolua_get_board_info_node_unsigned_time(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'time'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->time);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: time of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_time
static int tolua_set_board_info_node_unsigned_time(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'time'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->time = ((unsigned short int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: animatedWater of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_unsigned_animatedWater
static int tolua_get_board_info_node_unsigned_animatedWater(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'animatedWater'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->animatedWater);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: animatedWater of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_unsigned_animatedWater
static int tolua_set_board_info_node_unsigned_animatedWater(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'animatedWater'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->animatedWater = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: message of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_message
static int tolua_get_board_info_node_message(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'message'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->message);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: message of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_message
static int tolua_set_board_info_node_message(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'message'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy(self->message,tolua_tostring(tolua_S,2,0),60-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: msgcount of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_msgcount
static int tolua_get_board_info_node_msgcount(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'msgcount'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->msgcount);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: msgcount of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_msgcount
static int tolua_set_board_info_node_msgcount(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'msgcount'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->msgcount = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: compressed of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_get_board_info_node_compressed
static int tolua_get_board_info_node_compressed(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'compressed'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->compressed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: compressed of class  board_info_node */
#ifndef TOLUA_DISABLE_tolua_set_board_info_node_compressed
static int tolua_set_board_info_node_compressed(lua_State* tolua_S)
{
  board_info_node* self = (board_info_node*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'compressed'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->compressed = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: currentbrd */
#ifndef TOLUA_DISABLE_tolua_get_board_current_ptr
static int tolua_get_board_current_ptr(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)currentbrd,"board_info_node");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: currentbrd */
#ifndef TOLUA_DISABLE_tolua_set_board_current_ptr
static int tolua_set_board_current_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isusertype(tolua_S,2,"board_info_node",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  currentbrd = ((board_info_node*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* function: put */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_board_put00
static int tolua_zzt_zzt_board_put00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ZZTObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ZZTObject* o = ((ZZTObject*)  tolua_tousertype(tolua_S,1,0));
  {
   put(o);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'put'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: remove_from_board */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_board_remove00
static int tolua_zzt_zzt_board_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"board_info_node",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ZZTObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  struct board_info_node* brd = ((struct board_info_node*)  tolua_tousertype(tolua_S,1,0));
  ZZTObject* object = ((ZZTObject*)  tolua_tousertype(tolua_S,2,0));
  {
   remove_from_board(brd,object);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_obj_by_type */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_board_get_obj_by_type00
static int tolua_zzt_zzt_board_get_obj_by_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int type = ((int)  tolua_tonumber(tolua_S,1,0));
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  get_obj_by_type(type,x,y);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
   tolua_pushnumber(tolua_S,(lua_Number)x);
   tolua_pushnumber(tolua_S,(lua_Number)y);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_obj_by_type'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_obj_by_type */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_board_get_obj_by_type01
static int tolua_zzt_zzt_board_get_obj_by_type01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"board_info_node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  struct board_info_node* board = ((struct board_info_node*)  tolua_tousertype(tolua_S,1,0));
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  get_obj_by_type(board,type);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_zzt_board_get_obj_by_type00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_obj_by_color */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_board_get_obj_by_color00
static int tolua_zzt_zzt_board_get_obj_by_color00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"board_info_node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  struct board_info_node* board = ((struct board_info_node*)  tolua_tousertype(tolua_S,1,0));
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  int color = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  get_obj_by_color(board,type,color);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_obj_by_color'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_obj_by_color */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_board_get_obj_by_color01
static int tolua_zzt_zzt_board_get_obj_by_color01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"board_info_node",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  struct board_info_node* board = ((struct board_info_node*)  tolua_tousertype(tolua_S,1,0));
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  int fg = ((int)  tolua_tonumber(tolua_S,3,0));
  int bg = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  get_obj_by_color(board,type,fg,bg);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_zzt_board_get_obj_by_color00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_obj_by_name */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_board_get_obj_by_name00
static int tolua_zzt_zzt_board_get_obj_by_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"board_info_node",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  struct board_info_node* board = ((struct board_info_node*)  tolua_tousertype(tolua_S,1,0));
  std::string name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   ZZTObject* tolua_ret = (ZZTObject*)  get_obj_by_name(board,name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ZZTObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_obj_by_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: set_msg */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_set_msg00
static int tolua_zzt_zzt_status_set_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* text = ((char*)  tolua_tostring(tolua_S,1,0));
  {
   set_msg(text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_msg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: take_ammo */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_take_ammo00
static int tolua_zzt_zzt_status_take_ammo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   take_ammo(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'take_ammo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: give_ammo */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_give_ammo00
static int tolua_zzt_zzt_status_give_ammo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   give_ammo(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'give_ammo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: take_health */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_take_health00
static int tolua_zzt_zzt_status_take_health00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   take_health(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'take_health'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: give_health */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_give_health00
static int tolua_zzt_zzt_status_give_health00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   give_health(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'give_health'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: take_torch */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_take_torch00
static int tolua_zzt_zzt_status_take_torch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   take_torch(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'take_torch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: give_torch */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_give_torch00
static int tolua_zzt_zzt_status_give_torch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   give_torch(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'give_torch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: take_gems */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_take_gems00
static int tolua_zzt_zzt_status_take_gems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   take_gems(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'take_gems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: give_gems */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_give_gems00
static int tolua_zzt_zzt_status_give_gems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   give_gems(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'give_gems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: take_score */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_take_score00
static int tolua_zzt_zzt_status_take_score00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   take_score(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'take_score'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: give_score */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_give_score00
static int tolua_zzt_zzt_status_give_score00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   give_score(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'give_score'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: take_time */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_take_time00
static int tolua_zzt_zzt_status_take_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   take_time(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'take_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: give_time */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_status_give_time00
static int tolua_zzt_zzt_status_give_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int count = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   give_time(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'give_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: debug */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_debug00
static int tolua_zzt_zzt_debug00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* text = ((char*)  tolua_tostring(tolua_S,1,0));
  {
   debug(text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'debug'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBg of class  TUIWidget */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIWidget_setBg00
static int tolua_zzt_zzt_TUIWidget_setBg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TUIWidget",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TUIWidget* self = (TUIWidget*)  tolua_tousertype(tolua_S,1,0);
  unsigned char bg = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBg'",NULL);
#endif
  {
   self->setBg(bg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUILabel */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUILabel_new00
static int tolua_zzt_zzt_TUILabel_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUILabel",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool bold = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool ANSI = ((bool)  tolua_toboolean(tolua_S,4,false));
  {
   TUILabel* tolua_ret = (TUILabel*)  new TUILabel(text,bold,ANSI);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUILabel");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUILabel */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUILabel_new00_local
static int tolua_zzt_zzt_TUILabel_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUILabel",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool bold = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool ANSI = ((bool)  tolua_toboolean(tolua_S,4,false));
  {
   TUILabel* tolua_ret = (TUILabel*)  new TUILabel(text,bold,ANSI);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUILabel");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUITextInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUITextInput_new00
static int tolua_zzt_zzt_TUITextInput_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUITextInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  bool centered = ((bool)  tolua_toboolean(tolua_S,4,false));
  {
   TUITextInput* tolua_ret = (TUITextInput*)  new TUITextInput(label,&text,centered);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUITextInput");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUITextInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUITextInput_new00_local
static int tolua_zzt_zzt_TUITextInput_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUITextInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  bool centered = ((bool)  tolua_toboolean(tolua_S,4,false));
  {
   TUITextInput* tolua_ret = (TUITextInput*)  new TUITextInput(label,&text,centered);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUITextInput");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIPasswordInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIPasswordInput_new00
static int tolua_zzt_zzt_TUIPasswordInput_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIPasswordInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   TUIPasswordInput* tolua_ret = (TUIPasswordInput*)  new TUIPasswordInput(label,&text);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIPasswordInput");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIPasswordInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIPasswordInput_new00_local
static int tolua_zzt_zzt_TUIPasswordInput_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIPasswordInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   TUIPasswordInput* tolua_ret = (TUIPasswordInput*)  new TUIPasswordInput(label,&text);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIPasswordInput");
   tolua_pushcppstring(tolua_S,(const char*)text);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUICheckBox */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUICheckBox_new00
static int tolua_zzt_zzt_TUICheckBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUICheckBox",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool checked = ((bool)  tolua_toboolean(tolua_S,3,0));
  {
   TUICheckBox* tolua_ret = (TUICheckBox*)  new TUICheckBox(text,&checked);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUICheckBox");
   tolua_pushboolean(tolua_S,(bool)checked);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUICheckBox */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUICheckBox_new00_local
static int tolua_zzt_zzt_TUICheckBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUICheckBox",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool checked = ((bool)  tolua_toboolean(tolua_S,3,0));
  {
   TUICheckBox* tolua_ret = (TUICheckBox*)  new TUICheckBox(text,&checked);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUICheckBox");
   tolua_pushboolean(tolua_S,(bool)checked);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_new00
static int tolua_zzt_zzt_TUIRadioGroup_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int selected = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   TUIRadioGroup* tolua_ret = (TUIRadioGroup*)  new TUIRadioGroup(text,&selected);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIRadioGroup");
   tolua_pushnumber(tolua_S,(lua_Number)selected);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_new00_local
static int tolua_zzt_zzt_TUIRadioGroup_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int selected = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   TUIRadioGroup* tolua_ret = (TUIRadioGroup*)  new TUIRadioGroup(text,&selected);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIRadioGroup");
   tolua_pushnumber(tolua_S,(lua_Number)selected);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_new01
static int tolua_zzt_zzt_TUIRadioGroup_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned short selected = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  {
   TUIRadioGroup* tolua_ret = (TUIRadioGroup*)  new TUIRadioGroup(text,&selected);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIRadioGroup");
   tolua_pushnumber(tolua_S,(lua_Number)selected);
  }
 }
 return 2;
tolua_lerror:
 return tolua_zzt_zzt_TUIRadioGroup_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_new01_local
static int tolua_zzt_zzt_TUIRadioGroup_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned short selected = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  {
   TUIRadioGroup* tolua_ret = (TUIRadioGroup*)  new TUIRadioGroup(text,&selected);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIRadioGroup");
   tolua_pushnumber(tolua_S,(lua_Number)selected);
  }
 }
 return 2;
tolua_lerror:
 return tolua_zzt_zzt_TUIRadioGroup_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_new02
static int tolua_zzt_zzt_TUIRadioGroup_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float selected = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   TUIRadioGroup* tolua_ret = (TUIRadioGroup*)  new TUIRadioGroup(text,&selected);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIRadioGroup");
   tolua_pushnumber(tolua_S,(lua_Number)selected);
  }
 }
 return 2;
tolua_lerror:
 return tolua_zzt_zzt_TUIRadioGroup_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_new02_local
static int tolua_zzt_zzt_TUIRadioGroup_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float selected = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   TUIRadioGroup* tolua_ret = (TUIRadioGroup*)  new TUIRadioGroup(text,&selected);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIRadioGroup");
   tolua_pushnumber(tolua_S,(lua_Number)selected);
  }
 }
 return 2;
tolua_lerror:
 return tolua_zzt_zzt_TUIRadioGroup_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_new03
static int tolua_zzt_zzt_TUIRadioGroup_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned char* selected = ((unsigned char*)  tolua_tostring(tolua_S,3,0));
  {
   TUIRadioGroup* tolua_ret = (TUIRadioGroup*)  new TUIRadioGroup(text,selected);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIRadioGroup");
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_zzt_TUIRadioGroup_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_new03_local
static int tolua_zzt_zzt_TUIRadioGroup_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned char* selected = ((unsigned char*)  tolua_tostring(tolua_S,3,0));
  {
   TUIRadioGroup* tolua_ret = (TUIRadioGroup*)  new TUIRadioGroup(text,selected);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIRadioGroup");
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_zzt_TUIRadioGroup_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: add of class  TUIRadioGroup */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIRadioGroup_add00
static int tolua_zzt_zzt_TUIRadioGroup_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TUIRadioGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TUIRadioGroup* self = (TUIRadioGroup*)  tolua_tousertype(tolua_S,1,0);
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
  {
   self->add(text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIBoardList */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIBoardList_new00
static int tolua_zzt_zzt_TUIBoardList_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIBoardList",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned char* board = ((unsigned char*)  tolua_tostring(tolua_S,3,0));
  {
   TUIBoardList* tolua_ret = (TUIBoardList*)  new TUIBoardList(text,board);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIBoardList");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIBoardList */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIBoardList_new00_local
static int tolua_zzt_zzt_TUIBoardList_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIBoardList",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned char* board = ((unsigned char*)  tolua_tostring(tolua_S,3,0));
  {
   TUIBoardList* tolua_ret = (TUIBoardList*)  new TUIBoardList(text,board);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIBoardList");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIDirection */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIDirection_new00
static int tolua_zzt_zzt_TUIDirection_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIDirection",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  Vector* d = ((Vector*)  tolua_tousertype(tolua_S,3,0));
  {
   TUIDirection* tolua_ret = (TUIDirection*)  new TUIDirection(text,d);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIDirection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIDirection */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIDirection_new00_local
static int tolua_zzt_zzt_TUIDirection_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIDirection",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  Vector* d = ((Vector*)  tolua_tousertype(tolua_S,3,0));
  {
   TUIDirection* tolua_ret = (TUIDirection*)  new TUIDirection(text,d);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIDirection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUINumericInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUINumericInput_new00
static int tolua_zzt_zzt_TUINumericInput_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUINumericInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int num = ((int)  tolua_tonumber(tolua_S,3,0));
  int min = ((int)  tolua_tonumber(tolua_S,4,0));
  int max = ((int)  tolua_tonumber(tolua_S,5,0));
  int step = ((int)  tolua_tonumber(tolua_S,6,1));
  {
   TUINumericInput* tolua_ret = (TUINumericInput*)  new TUINumericInput(text,&num,min,max,step);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUINumericInput");
   tolua_pushnumber(tolua_S,(lua_Number)num);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUINumericInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUINumericInput_new00_local
static int tolua_zzt_zzt_TUINumericInput_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUINumericInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int num = ((int)  tolua_tonumber(tolua_S,3,0));
  int min = ((int)  tolua_tonumber(tolua_S,4,0));
  int max = ((int)  tolua_tonumber(tolua_S,5,0));
  int step = ((int)  tolua_tonumber(tolua_S,6,1));
  {
   TUINumericInput* tolua_ret = (TUINumericInput*)  new TUINumericInput(text,&num,min,max,step);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUINumericInput");
   tolua_pushnumber(tolua_S,(lua_Number)num);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUINumericInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUINumericInput_new01
static int tolua_zzt_zzt_TUINumericInput_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUINumericInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned short int num = ((unsigned short int)  tolua_tonumber(tolua_S,3,0));
  int min = ((int)  tolua_tonumber(tolua_S,4,0));
  int max = ((int)  tolua_tonumber(tolua_S,5,0));
  int step = ((int)  tolua_tonumber(tolua_S,6,1));
  {
   TUINumericInput* tolua_ret = (TUINumericInput*)  new TUINumericInput(text,&num,min,max,step);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUINumericInput");
   tolua_pushnumber(tolua_S,(lua_Number)num);
  }
 }
 return 2;
tolua_lerror:
 return tolua_zzt_zzt_TUINumericInput_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUINumericInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUINumericInput_new01_local
static int tolua_zzt_zzt_TUINumericInput_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUINumericInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned short int num = ((unsigned short int)  tolua_tonumber(tolua_S,3,0));
  int min = ((int)  tolua_tonumber(tolua_S,4,0));
  int max = ((int)  tolua_tonumber(tolua_S,5,0));
  int step = ((int)  tolua_tonumber(tolua_S,6,1));
  {
   TUINumericInput* tolua_ret = (TUINumericInput*)  new TUINumericInput(text,&num,min,max,step);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUINumericInput");
   tolua_pushnumber(tolua_S,(lua_Number)num);
  }
 }
 return 2;
tolua_lerror:
 return tolua_zzt_zzt_TUINumericInput_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUINumericInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUINumericInput_new02
static int tolua_zzt_zzt_TUINumericInput_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUINumericInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float num = ((float)  tolua_tonumber(tolua_S,3,0));
  int min = ((int)  tolua_tonumber(tolua_S,4,0));
  int max = ((int)  tolua_tonumber(tolua_S,5,0));
  int step = ((int)  tolua_tonumber(tolua_S,6,1));
  {
   TUINumericInput* tolua_ret = (TUINumericInput*)  new TUINumericInput(text,&num,min,max,step);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUINumericInput");
   tolua_pushnumber(tolua_S,(lua_Number)num);
  }
 }
 return 2;
tolua_lerror:
 return tolua_zzt_zzt_TUINumericInput_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUINumericInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUINumericInput_new02_local
static int tolua_zzt_zzt_TUINumericInput_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUINumericInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  float num = ((float)  tolua_tonumber(tolua_S,3,0));
  int min = ((int)  tolua_tonumber(tolua_S,4,0));
  int max = ((int)  tolua_tonumber(tolua_S,5,0));
  int step = ((int)  tolua_tonumber(tolua_S,6,1));
  {
   TUINumericInput* tolua_ret = (TUINumericInput*)  new TUINumericInput(text,&num,min,max,step);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUINumericInput");
   tolua_pushnumber(tolua_S,(lua_Number)num);
  }
 }
 return 2;
tolua_lerror:
 return tolua_zzt_zzt_TUINumericInput_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUINumericInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUINumericInput_new03
static int tolua_zzt_zzt_TUINumericInput_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUINumericInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned char* num = ((unsigned char*)  tolua_tostring(tolua_S,3,0));
  int min = ((int)  tolua_tonumber(tolua_S,4,0));
  int max = ((int)  tolua_tonumber(tolua_S,5,0));
  int step = ((int)  tolua_tonumber(tolua_S,6,1));
  {
   TUINumericInput* tolua_ret = (TUINumericInput*)  new TUINumericInput(text,num,min,max,step);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUINumericInput");
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_zzt_TUINumericInput_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUINumericInput */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUINumericInput_new03_local
static int tolua_zzt_zzt_TUINumericInput_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUINumericInput",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  unsigned char* num = ((unsigned char*)  tolua_tostring(tolua_S,3,0));
  int min = ((int)  tolua_tonumber(tolua_S,4,0));
  int max = ((int)  tolua_tonumber(tolua_S,5,0));
  int step = ((int)  tolua_tonumber(tolua_S,6,1));
  {
   TUINumericInput* tolua_ret = (TUINumericInput*)  new TUINumericInput(text,num,min,max,step);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUINumericInput");
  }
 }
 return 1;
tolua_lerror:
 return tolua_zzt_zzt_TUINumericInput_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUISlider */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUISlider_new00
static int tolua_zzt_zzt_TUISlider_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUISlider",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int slide = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   TUISlider* tolua_ret = (TUISlider*)  new TUISlider(text,&slide);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUISlider");
   tolua_pushnumber(tolua_S,(lua_Number)slide);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUISlider */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUISlider_new00_local
static int tolua_zzt_zzt_TUISlider_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUISlider",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int slide = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   TUISlider* tolua_ret = (TUISlider*)  new TUISlider(text,&slide);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUISlider");
   tolua_pushnumber(tolua_S,(lua_Number)slide);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIHyperLink */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIHyperLink_new00
static int tolua_zzt_zzt_TUIHyperLink_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIHyperLink",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   TUIHyperLink* tolua_ret = (TUIHyperLink*)  new TUIHyperLink(label,text);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIHyperLink");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIHyperLink */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIHyperLink_new00_local
static int tolua_zzt_zzt_TUIHyperLink_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIHyperLink",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string label = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string text = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   TUIHyperLink* tolua_ret = (TUIHyperLink*)  new TUIHyperLink(label,text);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIHyperLink");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TUIWindow */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIWindow_new00
static int tolua_zzt_zzt_TUIWindow_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIWindow",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string title = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,6));
  int y = ((int)  tolua_tonumber(tolua_S,4,3));
  int w = ((int)  tolua_tonumber(tolua_S,5,45));
  int h = ((int)  tolua_tonumber(tolua_S,6,17));
  {
   TUIWindow* tolua_ret = (TUIWindow*)  new TUIWindow(title,x,y,w,h);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TUIWindow");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TUIWindow */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIWindow_new00_local
static int tolua_zzt_zzt_TUIWindow_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TUIWindow",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string title = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int x = ((int)  tolua_tonumber(tolua_S,3,6));
  int y = ((int)  tolua_tonumber(tolua_S,4,3));
  int w = ((int)  tolua_tonumber(tolua_S,5,45));
  int h = ((int)  tolua_tonumber(tolua_S,6,17));
  {
   TUIWindow* tolua_ret = (TUIWindow*)  new TUIWindow(title,x,y,w,h);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TUIWindow");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildFromString of class  TUIWindow */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIWindow_buildFromString00
static int tolua_zzt_zzt_TUIWindow_buildFromString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TUIWindow",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TUIWindow* self = (TUIWindow*)  tolua_tousertype(tolua_S,1,0);
  std::string s = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool ANSI = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildFromString'",NULL);
#endif
  {
   self->buildFromString(s,ANSI);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildFromString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doMenu of class  TUIWindow */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIWindow_doMenu00
static int tolua_zzt_zzt_TUIWindow_doMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TUIWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TUIWindow* self = (TUIWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doMenu'",NULL);
#endif
  {
   self->doMenu();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addWidget of class  TUIWindow */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIWindow_addWidget00
static int tolua_zzt_zzt_TUIWindow_addWidget00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TUIWindow",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"TUIWidget",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TUIWindow* self = (TUIWindow*)  tolua_tousertype(tolua_S,1,0);
  TUIWidget* w = ((TUIWidget*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addWidget'",NULL);
#endif
  {
   self->addWidget(w);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addWidget'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLabel of class  TUIWindow */
#ifndef TOLUA_DISABLE_tolua_zzt_zzt_TUIWindow_getLabel00
static int tolua_zzt_zzt_TUIWindow_getLabel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TUIWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TUIWindow* self = (TUIWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLabel'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getLabel();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLabel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_zzt_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Vector","Vector","",tolua_collect_Vector);
  #else
  tolua_cclass(tolua_S,"Vector","Vector","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Vector");
   tolua_function(tolua_S,"new",tolua_zzt_Vector_new00);
   tolua_function(tolua_S,"new_local",tolua_zzt_Vector_new00_local);
   tolua_function(tolua_S,".call",tolua_zzt_Vector_new00_local);
   tolua_function(tolua_S,"new",tolua_zzt_Vector_new01);
   tolua_function(tolua_S,"new_local",tolua_zzt_Vector_new01_local);
   tolua_function(tolua_S,".call",tolua_zzt_Vector_new01_local);
   tolua_function(tolua_S,".geti",tolua_zzt_Vector__geti00);
   tolua_function(tolua_S,".eq",tolua_zzt_Vector__eq00);
   tolua_function(tolua_S,".add",tolua_zzt_Vector__add00);
   tolua_function(tolua_S,".sub",tolua_zzt_Vector__sub00);
   tolua_function(tolua_S,".sub",tolua_zzt_Vector__sub01);
   tolua_function(tolua_S,".mul",tolua_zzt_Vector__mul00);
   tolua_function(tolua_S,"zero",tolua_zzt_Vector_zero00);
   tolua_function(tolua_S,"dot",tolua_zzt_Vector_dot00);
   tolua_function(tolua_S,"cross",tolua_zzt_Vector_cross00);
   tolua_function(tolua_S,"length",tolua_zzt_Vector_length00);
   tolua_function(tolua_S,"rlength",tolua_zzt_Vector_rlength00);
   tolua_function(tolua_S,"normalizeSelf",tolua_zzt_Vector_normalizeSelf00);
   tolua_function(tolua_S,"normalize",tolua_zzt_Vector_normalize00);
   tolua_variable(tolua_S,"x",tolua_get_Vector_x,tolua_set_Vector_x);
   tolua_variable(tolua_S,"y",tolua_get_Vector_y,tolua_set_Vector_y);
   tolua_variable(tolua_S,"z",tolua_get_Vector_z,tolua_set_Vector_z);
   tolua_variable(tolua_S,"w",tolua_get_Vector_w,tolua_set_Vector_w);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"IDLE",IDLE);
  tolua_constant(tolua_S,"LEFT",LEFT);
  tolua_constant(tolua_S,"RIGHT",RIGHT);
  tolua_constant(tolua_S,"UP",UP);
  tolua_constant(tolua_S,"DOWN",DOWN);
  tolua_constant(tolua_S,"SHOOTING",SHOOTING);
  tolua_constant(tolua_S,"BLACK",BLACK);
  tolua_constant(tolua_S,"BLUE",BLUE);
  tolua_constant(tolua_S,"GREEN",GREEN);
  tolua_constant(tolua_S,"CYAN",CYAN);
  tolua_constant(tolua_S,"RED",RED);
  tolua_constant(tolua_S,"MAGENTA",MAGENTA);
  tolua_constant(tolua_S,"YELLOW",YELLOW);
  tolua_constant(tolua_S,"GREY",GREY);
  tolua_constant(tolua_S,"GRAY",GRAY);
  tolua_constant(tolua_S,"WHITE",WHITE);
  tolua_constant(tolua_S,"HIGH_INTENSITY",HIGH_INTENSITY);
  tolua_constant(tolua_S,"UNDERLINE",UNDERLINE);
  tolua_constant(tolua_S,"UNDERSCORE",UNDERSCORE);
  tolua_constant(tolua_S,"BLINK",BLINK);
  tolua_constant(tolua_S,"REVERSE",REVERSE);
  tolua_constant(tolua_S,"INVISIBLE",INVISIBLE);
  tolua_constant(tolua_S,"ZZT_TYPE_COUNT",ZZT_TYPE_COUNT);
  tolua_constant(tolua_S,"ZZT_EMPTY",ZZT_EMPTY);
  tolua_constant(tolua_S,"ZZT_EDGE",ZZT_EDGE);
  tolua_constant(tolua_S,"ZZT_EXPLOSION",ZZT_EXPLOSION);
  tolua_constant(tolua_S,"ZZT_PLAYER",ZZT_PLAYER);
  tolua_constant(tolua_S,"ZZT_AMMO",ZZT_AMMO);
  tolua_constant(tolua_S,"ZZT_TORCH",ZZT_TORCH);
  tolua_constant(tolua_S,"ZZT_GEM",ZZT_GEM);
  tolua_constant(tolua_S,"ZZT_KEY",ZZT_KEY);
  tolua_constant(tolua_S,"ZZT_DOOR",ZZT_DOOR);
  tolua_constant(tolua_S,"ZZT_SCROLL",ZZT_SCROLL);
  tolua_constant(tolua_S,"ZZT_PASSAGE",ZZT_PASSAGE);
  tolua_constant(tolua_S,"ZZT_DUPLICATOR",ZZT_DUPLICATOR);
  tolua_constant(tolua_S,"ZZT_BOMB",ZZT_BOMB);
  tolua_constant(tolua_S,"ZZT_ENERGIZER",ZZT_ENERGIZER);
  tolua_constant(tolua_S,"ZZT_STAR",ZZT_STAR);
  tolua_constant(tolua_S,"ZZT_CONVEYER_CW",ZZT_CONVEYER_CW);
  tolua_constant(tolua_S,"ZZT_CONVEYER_CCW",ZZT_CONVEYER_CCW);
  tolua_constant(tolua_S,"ZZT_BULLET",ZZT_BULLET);
  tolua_constant(tolua_S,"ZZT_WATER",ZZT_WATER);
  tolua_constant(tolua_S,"ZZT_FOREST",ZZT_FOREST);
  tolua_constant(tolua_S,"ZZT_SOLID",ZZT_SOLID);
  tolua_constant(tolua_S,"ZZT_NORMAL",ZZT_NORMAL);
  tolua_constant(tolua_S,"ZZT_BREAKABLE",ZZT_BREAKABLE);
  tolua_constant(tolua_S,"ZZT_BOULDER",ZZT_BOULDER);
  tolua_constant(tolua_S,"ZZT_SLIDER_NS",ZZT_SLIDER_NS);
  tolua_constant(tolua_S,"ZZT_SLIDER_EW",ZZT_SLIDER_EW);
  tolua_constant(tolua_S,"ZZT_FAKE",ZZT_FAKE);
  tolua_constant(tolua_S,"ZZT_INVISIBLE",ZZT_INVISIBLE);
  tolua_constant(tolua_S,"ZZT_BLINK",ZZT_BLINK);
  tolua_constant(tolua_S,"ZZT_TRANSPORTER",ZZT_TRANSPORTER);
  tolua_constant(tolua_S,"ZZT_LINE",ZZT_LINE);
  tolua_constant(tolua_S,"ZZT_RICOCHET",ZZT_RICOCHET);
  tolua_constant(tolua_S,"ZZT_HORIZONTAL_BLINK",ZZT_HORIZONTAL_BLINK);
  tolua_constant(tolua_S,"ZZT_BEAR",ZZT_BEAR);
  tolua_constant(tolua_S,"ZZT_RUFFIAN",ZZT_RUFFIAN);
  tolua_constant(tolua_S,"ZZT_OBJECT",ZZT_OBJECT);
  tolua_constant(tolua_S,"ZZT_SLIME",ZZT_SLIME);
  tolua_constant(tolua_S,"ZZT_SHARK",ZZT_SHARK);
  tolua_constant(tolua_S,"ZZT_SPINNING_GUN",ZZT_SPINNING_GUN);
  tolua_constant(tolua_S,"ZZT_PUSHER",ZZT_PUSHER);
  tolua_constant(tolua_S,"ZZT_LION",ZZT_LION);
  tolua_constant(tolua_S,"ZZT_TIGER",ZZT_TIGER);
  tolua_constant(tolua_S,"ZZT_VERTICAL_BLINK",ZZT_VERTICAL_BLINK);
  tolua_constant(tolua_S,"ZZT_CENTIPEDE_HEAD",ZZT_CENTIPEDE_HEAD);
  tolua_constant(tolua_S,"ZZT_CENTIPEDE_BODY",ZZT_CENTIPEDE_BODY);
  tolua_constant(tolua_S,"ZZT_BLUE_TEXT",ZZT_BLUE_TEXT);
  tolua_constant(tolua_S,"ZZT_GREEN_TEXT",ZZT_GREEN_TEXT);
  tolua_constant(tolua_S,"ZZT_CYAN_TEXT",ZZT_CYAN_TEXT);
  tolua_constant(tolua_S,"ZZT_RED_TEXT",ZZT_RED_TEXT);
  tolua_constant(tolua_S,"ZZT_PURPLE_TEXT",ZZT_PURPLE_TEXT);
  tolua_constant(tolua_S,"ZZT_YELLOW_TEXT",ZZT_YELLOW_TEXT);
  tolua_constant(tolua_S,"ZZT_WHITE_TEXT",ZZT_WHITE_TEXT);
  tolua_module(tolua_S,"zzt",0);
  tolua_beginmodule(tolua_S,"zzt");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ZZTObject","ZZTObject","",tolua_collect_ZZTObject);
   #else
   tolua_cclass(tolua_S,"ZZTObject","ZZTObject","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ZZTObject");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_ZZTObject_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_ZZTObject_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_ZZTObject_new00_local);
    tolua_function(tolua_S,"opposite",tolua_zzt_zzt_ZZTObject_opposite00);
    tolua_function(tolua_S,"toward",tolua_zzt_zzt_ZZTObject_toward00);
    tolua_function(tolua_S,"clockwise",tolua_zzt_zzt_ZZTObject_clockwise00);
    tolua_function(tolua_S,"str_to_direction",tolua_zzt_zzt_ZZTObject_str_to_direction00);
    tolua_function(tolua_S,"isValid",tolua_zzt_zzt_ZZTObject_isValid00);
    tolua_function(tolua_S,"str_to_color",tolua_zzt_zzt_ZZTObject_str_to_color00);
    tolua_function(tolua_S,"int_to_color",tolua_zzt_zzt_ZZTObject_int_to_color00);
    tolua_function(tolua_S,"distance",tolua_zzt_zzt_ZZTObject_distance00);
    tolua_function(tolua_S,"dist_x",tolua_zzt_zzt_ZZTObject_dist_x00);
    tolua_function(tolua_S,"dist_y",tolua_zzt_zzt_ZZTObject_dist_y00);
    tolua_function(tolua_S,"move",tolua_zzt_zzt_ZZTObject_move00);
    tolua_function(tolua_S,"is_empty",tolua_zzt_zzt_ZZTObject_is_empty00);
    tolua_function(tolua_S,"create_object",tolua_zzt_zzt_ZZTObject_create_object00);
    tolua_variable(tolua_S,"type",tolua_get_ZZTObject_type,NULL);
    tolua_variable(tolua_S,"name",tolua_get_ZZTObject_name,tolua_set_ZZTObject_name);
    tolua_variable(tolua_S,"position",tolua_get_ZZTObject_position,tolua_set_ZZTObject_position);
    tolua_variable(tolua_S,"step",tolua_get_ZZTObject_step,tolua_set_ZZTObject_step);
    tolua_variable(tolua_S,"heading",tolua_get_ZZTObject_heading,tolua_set_ZZTObject_heading);
    tolua_variable(tolua_S,"shape",tolua_get_ZZTObject_shape,tolua_set_ZZTObject_shape);
    tolua_variable(tolua_S,"color",tolua_get_ZZTObject_color,tolua_set_ZZTObject_color);
    tolua_variable(tolua_S,"fg",tolua_get_ZZTObject_fg,tolua_set_ZZTObject_fg);
    tolua_variable(tolua_S,"bg",tolua_get_ZZTObject_bg,tolua_set_ZZTObject_bg);
    tolua_variable(tolua_S,"cycle",tolua_get_ZZTObject_cycle,tolua_set_ZZTObject_cycle);
    tolua_variable(tolua_S,"tick",tolua_get_ZZTObject_tick,tolua_set_ZZTObject_tick);
    tolua_variable(tolua_S,"updated",tolua_get_ZZTObject_updated,tolua_set_ZZTObject_updated);
    tolua_variable(tolua_S,"pushed",tolua_get_ZZTObject_pushed,tolua_set_ZZTObject_pushed);
    tolua_function(tolua_S,"setFlag",tolua_zzt_zzt_ZZTObject_setFlag00);
    tolua_function(tolua_S,"flag",tolua_zzt_zzt_ZZTObject_flag00);
    tolua_variable(tolua_S,"flags",tolua_get_ZZTObject_flags,tolua_set_ZZTObject_flags);
    tolua_function(tolua_S,"setParam",tolua_zzt_zzt_ZZTObject_setParam00);
    tolua_function(tolua_S,"param",tolua_zzt_zzt_ZZTObject_param00);
    tolua_function(tolua_S,"message",tolua_zzt_zzt_ZZTObject_message00);
    tolua_function(tolua_S,"shoot",tolua_zzt_zzt_ZZTObject_shoot00);
   tolua_endmodule(tolua_S);
   tolua_function(tolua_S,"create_object",tolua_zzt_zzt_create_object00);
   tolua_function(tolua_S,"create_copy",tolua_zzt_zzt_create_copy00);
   tolua_module(tolua_S,"board",1);
   tolua_beginmodule(tolua_S,"board");
    tolua_constant(tolua_S,"BOARD_X",BOARD_X);
    tolua_constant(tolua_S,"BOARD_Y",BOARD_Y);
    tolua_cclass(tolua_S,"board_info_node","board_info_node","",NULL);
    tolua_beginmodule(tolua_S,"board_info_node");
     tolua_variable(tolua_S,"size",tolua_get_board_info_node_unsigned_size,tolua_set_board_info_node_unsigned_size);
     tolua_variable(tolua_S,"title",tolua_get_board_info_node_title,tolua_set_board_info_node_title);
     tolua_variable(tolua_S,"num",tolua_get_board_info_node_num,tolua_set_board_info_node_num);
     tolua_variable(tolua_S,"maxshots",tolua_get_board_info_node_unsigned_maxshots,tolua_set_board_info_node_unsigned_maxshots);
     tolua_variable(tolua_S,"dark",tolua_get_board_info_node_unsigned_dark,tolua_set_board_info_node_unsigned_dark);
     tolua_variable(tolua_S,"board_up",tolua_get_board_info_node_unsigned_board_up,tolua_set_board_info_node_unsigned_board_up);
     tolua_variable(tolua_S,"board_down",tolua_get_board_info_node_unsigned_board_down,tolua_set_board_info_node_unsigned_board_down);
     tolua_variable(tolua_S,"board_left",tolua_get_board_info_node_unsigned_board_left,tolua_set_board_info_node_unsigned_board_left);
     tolua_variable(tolua_S,"board_right",tolua_get_board_info_node_unsigned_board_right,tolua_set_board_info_node_unsigned_board_right);
     tolua_variable(tolua_S,"reenter",tolua_get_board_info_node_unsigned_reenter,tolua_set_board_info_node_unsigned_reenter);
     tolua_variable(tolua_S,"reenter_x",tolua_get_board_info_node_unsigned_reenter_x,tolua_set_board_info_node_unsigned_reenter_x);
     tolua_variable(tolua_S,"reenter_y",tolua_get_board_info_node_unsigned_reenter_y,tolua_set_board_info_node_unsigned_reenter_y);
     tolua_variable(tolua_S,"time",tolua_get_board_info_node_unsigned_time,tolua_set_board_info_node_unsigned_time);
     tolua_variable(tolua_S,"animatedWater",tolua_get_board_info_node_unsigned_animatedWater,tolua_set_board_info_node_unsigned_animatedWater);
     tolua_variable(tolua_S,"message",tolua_get_board_info_node_message,tolua_set_board_info_node_message);
     tolua_variable(tolua_S,"msgcount",tolua_get_board_info_node_msgcount,tolua_set_board_info_node_msgcount);
     tolua_variable(tolua_S,"compressed",tolua_get_board_info_node_compressed,tolua_set_board_info_node_compressed);
    tolua_endmodule(tolua_S);
    tolua_variable(tolua_S,"current",tolua_get_board_current_ptr,tolua_set_board_current_ptr);
    tolua_function(tolua_S,"put",tolua_zzt_zzt_board_put00);
    tolua_function(tolua_S,"remove",tolua_zzt_zzt_board_remove00);
    tolua_function(tolua_S,"get_obj_by_type",tolua_zzt_zzt_board_get_obj_by_type00);
    tolua_function(tolua_S,"get_obj_by_type",tolua_zzt_zzt_board_get_obj_by_type01);
    tolua_function(tolua_S,"get_obj_by_color",tolua_zzt_zzt_board_get_obj_by_color00);
    tolua_function(tolua_S,"get_obj_by_color",tolua_zzt_zzt_board_get_obj_by_color01);
    tolua_function(tolua_S,"get_obj_by_name",tolua_zzt_zzt_board_get_obj_by_name00);
   tolua_endmodule(tolua_S);
   tolua_module(tolua_S,"status",0);
   tolua_beginmodule(tolua_S,"status");
    tolua_function(tolua_S,"set_msg",tolua_zzt_zzt_status_set_msg00);
    tolua_function(tolua_S,"take_ammo",tolua_zzt_zzt_status_take_ammo00);
    tolua_function(tolua_S,"give_ammo",tolua_zzt_zzt_status_give_ammo00);
    tolua_function(tolua_S,"take_health",tolua_zzt_zzt_status_take_health00);
    tolua_function(tolua_S,"give_health",tolua_zzt_zzt_status_give_health00);
    tolua_function(tolua_S,"take_torch",tolua_zzt_zzt_status_take_torch00);
    tolua_function(tolua_S,"give_torch",tolua_zzt_zzt_status_give_torch00);
    tolua_function(tolua_S,"take_gems",tolua_zzt_zzt_status_take_gems00);
    tolua_function(tolua_S,"give_gems",tolua_zzt_zzt_status_give_gems00);
    tolua_function(tolua_S,"take_score",tolua_zzt_zzt_status_take_score00);
    tolua_function(tolua_S,"give_score",tolua_zzt_zzt_status_give_score00);
    tolua_function(tolua_S,"take_time",tolua_zzt_zzt_status_take_time00);
    tolua_function(tolua_S,"give_time",tolua_zzt_zzt_status_give_time00);
   tolua_endmodule(tolua_S);
   tolua_function(tolua_S,"debug",tolua_zzt_zzt_debug00);
   tolua_cclass(tolua_S,"TUIWidget","TUIWidget","",NULL);
   tolua_beginmodule(tolua_S,"TUIWidget");
    tolua_function(tolua_S,"setBg",tolua_zzt_zzt_TUIWidget_setBg00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUILabel","TUILabel","TUIWidget",tolua_collect_TUILabel);
   #else
   tolua_cclass(tolua_S,"TUILabel","TUILabel","TUIWidget",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUILabel");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUILabel_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUILabel_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUILabel_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUITextInput","TUITextInput","TUIWidget",tolua_collect_TUITextInput);
   #else
   tolua_cclass(tolua_S,"TUITextInput","TUITextInput","TUIWidget",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUITextInput");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUITextInput_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUITextInput_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUITextInput_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUIPasswordInput","TUIPasswordInput","TUITextInput",tolua_collect_TUIPasswordInput);
   #else
   tolua_cclass(tolua_S,"TUIPasswordInput","TUIPasswordInput","TUITextInput",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUIPasswordInput");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIPasswordInput_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIPasswordInput_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIPasswordInput_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUICheckBox","TUICheckBox","TUIWidget",tolua_collect_TUICheckBox);
   #else
   tolua_cclass(tolua_S,"TUICheckBox","TUICheckBox","TUIWidget",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUICheckBox");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUICheckBox_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUICheckBox_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUICheckBox_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUIRadioGroup","TUIRadioGroup","TUIWidget",tolua_collect_TUIRadioGroup);
   #else
   tolua_cclass(tolua_S,"TUIRadioGroup","TUIRadioGroup","TUIWidget",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUIRadioGroup");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIRadioGroup_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIRadioGroup_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIRadioGroup_new00_local);
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIRadioGroup_new01);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIRadioGroup_new01_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIRadioGroup_new01_local);
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIRadioGroup_new02);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIRadioGroup_new02_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIRadioGroup_new02_local);
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIRadioGroup_new03);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIRadioGroup_new03_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIRadioGroup_new03_local);
    tolua_function(tolua_S,"add",tolua_zzt_zzt_TUIRadioGroup_add00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUIBoardList","TUIBoardList","TUIRadioGroup",tolua_collect_TUIBoardList);
   #else
   tolua_cclass(tolua_S,"TUIBoardList","TUIBoardList","TUIRadioGroup",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUIBoardList");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIBoardList_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIBoardList_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIBoardList_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUIDirection","TUIDirection","TUIRadioGroup",tolua_collect_TUIDirection);
   #else
   tolua_cclass(tolua_S,"TUIDirection","TUIDirection","TUIRadioGroup",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUIDirection");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIDirection_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIDirection_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIDirection_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUINumericInput","TUINumericInput","TUIWidget",tolua_collect_TUINumericInput);
   #else
   tolua_cclass(tolua_S,"TUINumericInput","TUINumericInput","TUIWidget",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUINumericInput");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUINumericInput_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUINumericInput_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUINumericInput_new00_local);
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUINumericInput_new01);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUINumericInput_new01_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUINumericInput_new01_local);
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUINumericInput_new02);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUINumericInput_new02_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUINumericInput_new02_local);
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUINumericInput_new03);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUINumericInput_new03_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUINumericInput_new03_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUISlider","TUISlider","TUINumericInput",tolua_collect_TUISlider);
   #else
   tolua_cclass(tolua_S,"TUISlider","TUISlider","TUINumericInput",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUISlider");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUISlider_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUISlider_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUISlider_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUIHyperLink","TUIHyperLink","TUIWidget",tolua_collect_TUIHyperLink);
   #else
   tolua_cclass(tolua_S,"TUIHyperLink","TUIHyperLink","TUIWidget",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUIHyperLink");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIHyperLink_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIHyperLink_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIHyperLink_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"TUIWindow","TUIWindow","",tolua_collect_TUIWindow);
   #else
   tolua_cclass(tolua_S,"TUIWindow","TUIWindow","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"TUIWindow");
    tolua_function(tolua_S,"new",tolua_zzt_zzt_TUIWindow_new00);
    tolua_function(tolua_S,"new_local",tolua_zzt_zzt_TUIWindow_new00_local);
    tolua_function(tolua_S,".call",tolua_zzt_zzt_TUIWindow_new00_local);
    tolua_function(tolua_S,"buildFromString",tolua_zzt_zzt_TUIWindow_buildFromString00);
    tolua_function(tolua_S,"doMenu",tolua_zzt_zzt_TUIWindow_doMenu00);
    tolua_function(tolua_S,"addWidget",tolua_zzt_zzt_TUIWindow_addWidget00);
    tolua_function(tolua_S,"getLabel",tolua_zzt_zzt_TUIWindow_getLabel00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,102,117,110, 99,116,105,111,110, 32,116,112,114,105,110,
    116, 32, 40,116, 44, 32,105,110,100,101,110,116, 44, 32,100,
    111,110,101, 41, 10,100,111,110,101, 32, 61, 32,100,111,110,
    101, 32,111,114, 32,123,125, 10,105,110,100,101,110,116, 32,
     61, 32,105,110,100,101,110,116, 32,111,114, 32, 48, 10,102,
    111,114, 32,107,101,121, 44, 32,118, 97,108,117,101, 32,105,
    110, 32,112, 97,105,114,115, 32, 40,116, 41, 32,100,111, 10,
    105,111, 46,119,114,105,116,101, 32, 40,115,116,114,105,110,
    103, 46,114,101,112, 32, 40, 34, 32, 34, 44, 32,105,110,100,
    101,110,116, 41, 41, 10,105,102, 32,116,121,112,101, 32, 40,
    118, 97,108,117,101, 41, 32, 61, 61, 32, 34,116, 97, 98,108,
    101, 34, 32, 97,110,100, 32,110,111,116, 32,100,111,110,101,
     32, 91,118, 97,108,117,101, 93, 32,116,104,101,110, 10,100,
    111,110,101, 32, 91,118, 97,108,117,101, 93, 32, 61, 32,116,
    114,117,101, 10,105,111, 46,119,114,105,116,101, 32, 40,116,
    111,115,116,114,105,110,103, 32, 40,107,101,121, 41, 44, 32,
     34, 58, 34, 41, 59, 10,116,112,114,105,110,116, 32, 40,118,
     97,108,117,101, 44, 32,105,110,100,101,110,116, 32, 43, 32,
     50, 44, 32,100,111,110,101, 41, 10,101,108,115,101, 10,105,
    111, 46,119,114,105,116,101, 40,116,111,115,116,114,105,110,
    103, 32, 40,107,101,121, 41, 44, 32, 34, 61, 34, 41, 10,112,
    114,105,110,116, 32, 40,118, 97,108,117,101, 41, 10,101,110,
    100, 10,101,110,100, 10,101,110,100, 10,115, 97,118,101,115,
    116, 97,116,101, 32, 61, 32,123, 32,116,104,100, 32, 61, 32,
    110,105,108, 32,125, 10,102,117,110, 99,116,105,111,110, 32,
    115,101,116,117,112, 95,112,101,114,109,115, 40, 41, 10,112,
    101,114,109,115, 32, 61, 32,123, 32, 91, 99,111,114,111,117,
    116,105,110,101, 46,121,105,101,108,100, 93, 32, 61, 32, 49,
     44, 32, 91,109,101, 93, 32, 61, 32, 50, 44, 32, 91,112,108,
     97,121,101,114, 93, 32, 61, 32, 51, 32,125, 10,101,110,100,
     10,102,117,110, 99,116,105,111,110, 32,119, 97,107,101, 40,
     41, 10,105,102, 32,110,111,116, 40,115, 97,118,101,115,116,
     97,116,101, 46,116,104,100, 32, 61, 61, 32,110,105,108, 41,
     32,116,104,101,110, 10, 99,111,100,101, 44,109,115,103, 32,
     61, 32, 99,111,114,111,117,116,105,110,101, 46,114,101,115,
    117,109,101, 40,115, 97,118,101,115,116, 97,116,101, 46,116,
    104,100, 41, 10,105,102, 40, 99,111,100,101, 32, 61, 61, 32,
    102, 97,108,115,101, 41, 32,116,104,101,110, 10,112,114,105,
    110,116, 40,109,115,103, 41, 10,109,101, 46, 99,121, 99,108,
    101, 32, 61, 32, 48, 10,101,110,100, 10,101,110,100, 10,101,
    110,100, 10,102,117,110, 99,116,105,111,110, 32,115,108,101,
    101,112, 40, 99,121, 99,108,101,115, 41, 10, 99,111,114,111,
    117,116,105,110,101, 46,121,105,101,108,100, 40, 41, 10,101,
    110,100, 10,102,117,110, 99,116,105,111,110, 32,115,101,116,
     95,109,115,103, 40,109,101,115,115, 97,103,101, 41, 10,122,
    122,116, 46,115,116, 97,116,117,115, 46,115,101,116, 95,109,
    115,103, 40,109,101,115,115, 97,103,101, 41, 10, 99,111,114,
    111,117,116,105,110,101, 46,121,105,101,108,100, 40, 41, 10,
    101,110,100, 10,102,117,110, 99,116,105,111,110, 32,115,101,
    116, 95, 99,111,108,111,114, 40,102,103, 44, 32, 98,103, 41,
     10,109,101, 46,102,103, 32, 61, 32,102,103, 10,109,101, 46,
     98,103, 32, 61, 32, 98,103, 10, 99,111,114,111,117,116,105,
    110,101, 46,121,105,101,108,100, 40, 41, 10,101,110,100, 10,
    102,117,110, 99,116,105,111,110, 32,109,111,118,101, 40,100,
     41, 10,109,101, 58,109,111,118,101, 40,100, 41, 10, 99,111,
    114,111,117,116,105,110,101, 46,121,105,101,108,100, 40, 41,
     10,101,110,100, 10,102,117,110, 99,116,105,111,110, 32,116,
    114,121, 40,100, 41, 10,108,111, 99, 97,108, 32,116, 32, 61,
     32,109,101, 58,109,111,118,101, 40,100, 44, 32,116,114,117,
    101, 41, 10, 99,111,114,111,117,116,105,110,101, 46,121,105,
    101,108,100, 40, 41, 10,114,101,116,117,114,110, 32,116, 10,
    101,110,100, 10,102,117,110, 99,116,105,111,110, 32,119, 97,
    105,116, 95,102,111,114, 95,109,101,115,115, 97,103,101, 40,
    109,101,115,115, 97,103,101, 41, 10,108,111, 99, 97,108, 32,
    109,115,103, 32, 61, 32,110,105,108, 10,108,111, 99, 97,108,
     32,116,104,101,109, 32, 61, 32,110,105,108, 10,119,104,105,
    108,101, 32,110,111,116, 32, 40,109,115,103, 32, 61, 61, 32,
    109,101,115,115, 97,103,101, 41, 32,100,111, 10,109,115,103,
     32, 61, 32,110,105,108, 10,119,104,105,108,101, 32, 40,109,
    115,103, 32, 61, 61, 32,110,105,108, 41, 32,100,111, 10,116,
    104,101,109, 44,109,115,103, 32, 61, 32,112,111,112, 95,109,
    101,115,115, 97,103,101, 40,109,101, 41, 10, 99,111,114,111,
    117,116,105,110,101, 46,121,105,101,108,100, 40, 41, 10,101,
    110,100, 10,101,110,100, 10,114,101,116,117,114,110, 32,116,
    104,101,109, 10,101,110,100, 10,102,117,110, 99,116,105,111,
    110, 32, 99,104, 97,110,103,101, 40,116,121,112,101, 44, 32,
     99,111,108,111,114, 44, 32,111, 98,106, 95,116,101,109,112,
    108, 97,116,101, 41, 10,108,111, 99, 97,108, 32,111, 98,106,
     10,105,102, 40, 99,111,108,111,114, 32, 61, 61, 32,110,105,
    108, 41, 32,116,104,101,110, 10,111, 98,106, 32, 61, 32,122,
    122,116, 46, 98,111, 97,114,100, 46,103,101,116, 95,111, 98,
    106, 95, 98,121, 95,116,121,112,101, 40,122,122,116, 46, 98,
    111, 97,114,100, 46, 99,117,114,114,101,110,116, 44, 32,116,
    121,112,101, 41, 10,101,108,115,101, 10,111, 98,106, 32, 61,
     32,122,122,116, 46, 98,111, 97,114,100, 46,103,101,116, 95,
    111, 98,106, 95, 98,121, 95, 99,111,108,111,114, 40,122,122,
    116, 46, 98,111, 97,114,100, 46, 99,117,114,114,101,110,116,
     44, 32,116,121,112,101, 44, 32, 99,111,108,111,114, 41, 10,
    101,110,100, 10,119,104,105,108,101, 32,110,111,116, 32, 40,
    111, 98,106, 32, 61, 61, 32,110,105,108, 41, 32,100,111, 10,
    110,101,119,111, 98,106, 32, 61, 32,122,122,116, 46, 99,114,
    101, 97,116,101, 95, 99,111,112,121, 40,111, 98,106, 95,116,
    101,109,112,108, 97,116,101, 41, 10,110,101,119,111, 98,106,
     46,112,111,115,105,116,105,111,110, 32, 61, 32,111, 98,106,
     46,112,111,115,105,116,105,111,110, 10,122,122,116, 46, 98,
    111, 97,114,100, 46,112,117,116, 40,110,101,119,111, 98,106,
     41, 10,105,102, 40, 99,111,108,111,114, 32, 61, 61, 32,110,
    105,108, 41, 32,116,104,101,110, 10,111, 98,106, 32, 61, 32,
    122,122,116, 46, 98,111, 97,114,100, 46,103,101,116, 95,111,
     98,106, 95, 98,121, 95,116,121,112,101, 40,122,122,116, 46,
     98,111, 97,114,100, 46, 99,117,114,114,101,110,116, 44, 32,
    116,121,112,101, 41, 10,101,108,115,101, 10,111, 98,106, 32,
     61, 32,122,122,116, 46, 98,111, 97,114,100, 46,103,101,116,
     95,111, 98,106, 95, 98,121, 95, 99,111,108,111,114, 40,122,
    122,116, 46, 98,111, 97,114,100, 46, 99,117,114,114,101,110,
    116, 44, 32,116,121,112,101, 44, 32, 99,111,108,111,114, 41,
     10,101,110,100, 10,101,110,100, 10,101,110,100, 10,105,102,
     40,115, 97,118,101,115,116, 97,116,101, 46,116,104,100, 32,
     61, 61, 32,110,105,108, 41, 32,116,104,101,110, 10,115, 97,
    118,101,115,116, 97,116,101, 46,116,104,100, 32, 61, 32, 99,
    111,114,111,117,116,105,110,101, 46, 99,114,101, 97,116,101,
     40,102,117,110, 99,116,105,111,110, 32, 40, 41, 10, 99,111,
    114,111,117,116,105,110,101, 46,121,105,101,108,100, 40, 41,
     10,109, 97,105,110, 40, 41, 10,101,110,100, 10, 41, 10,101,
    110,100,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_zzt (lua_State* tolua_S) {
 return tolua_zzt_open(tolua_S);
};
#endif

