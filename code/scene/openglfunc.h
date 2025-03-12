#pragma once

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLExtraFunctions>

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

#define OPENGL_FUNCTIONS QOpenGLContext::currentContext()->functions()
#define OPENGL_EXTRA_FUNCTIONS QOpenGLContext::currentContext()->extraFunctions()
