////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglNormal3iv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglNormal3iv , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglNormal3iv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglNormal3iv :: GEMglNormal3iv	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) {
vMess(arg0, arg1, arg2);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglNormal3iv :: ~GEMglNormal3iv () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglNormal3iv :: render(GemState *state) {
	glNormal3iv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglNormal3iv :: vMess (t_float arg0, t_float arg1, t_float arg2) {	// FUN
	v[0]=(GLint)arg0;
	v[1]=(GLint)arg1;
	v[2]=(GLint)arg2;
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglNormal3iv :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglNormal3iv::vMessCallback),  	gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglNormal3iv :: vMessCallback (void* data, t_floatarg arg0, t_floatarg arg1, t_floatarg arg2) {
	GetMyClass(data)->vMess ( arg0, arg1, arg2);
}