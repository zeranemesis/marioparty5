/*
 * Deterministic trigonometry for the game (src/port/detmath.c), force-included into the game code and Aurora's
 * matrix library on PC (CMake).
 *
 * The C library's sin/cos/atan2... differ in their last bit between the MSVC CRT and Android's bionic, and the
 * game's logic branches on them (m506: a different frand count, a desync between a PC and a phone in netplay).
 * These use only + - * / and sqrt, exact in IEEE 754, so every platform computes the same bits.
 */
#ifndef MP5_DETMATH_H
#define MP5_DETMATH_H

#include <math.h>
#ifdef __cplusplus
extern "C" {
#endif

double Mp5_sin(double x);
double Mp5_cos(double x);
double Mp5_tan(double x);
double Mp5_atan(double x);
double Mp5_atan2(double y, double x);
double Mp5_asin(double x);
double Mp5_acos(double x);
float Mp5_sinf(float x);
float Mp5_cosf(float x);
float Mp5_tanf(float x);
float Mp5_atanf(float x);
float Mp5_atan2f(float y, float x);
float Mp5_asinf(float x);
float Mp5_acosf(float x);
float Mp5_fmaf(float a, float b, float c);

#ifdef __cplusplus
}
/* std::sin(x) becomes std::Mp5_sin(x) under the macros below */
namespace std {
using ::Mp5_sin; using ::Mp5_cos; using ::Mp5_tan; using ::Mp5_atan; using ::Mp5_atan2; using ::Mp5_asin; using ::Mp5_acos;
using ::Mp5_sinf; using ::Mp5_cosf; using ::Mp5_tanf; using ::Mp5_atanf; using ::Mp5_atan2f; using ::Mp5_asinf;
using ::Mp5_acosf; using ::Mp5_fmaf;
}
#endif

#ifndef MP5_DETMATH_IMPL
/* function-like: only calls are renamed, not `using ::sin`; variadic for K&R declarations such as `double atan2();` */
#define sin(...) Mp5_sin(__VA_ARGS__)
#define cos(...) Mp5_cos(__VA_ARGS__)
#define tan(...) Mp5_tan(__VA_ARGS__)
#define atan(...) Mp5_atan(__VA_ARGS__)
#define atan2(...) Mp5_atan2(__VA_ARGS__)
#define asin(...) Mp5_asin(__VA_ARGS__)
#define acos(...) Mp5_acos(__VA_ARGS__)
#define sinf(...) Mp5_sinf(__VA_ARGS__)
#define cosf(...) Mp5_cosf(__VA_ARGS__)
#define tanf(...) Mp5_tanf(__VA_ARGS__)
#define atanf(...) Mp5_atanf(__VA_ARGS__)
#define atan2f(...) Mp5_atan2f(__VA_ARGS__)
#define asinf(...) Mp5_asinf(__VA_ARGS__)
#define acosf(...) Mp5_acosf(__VA_ARGS__)
/* ppc_rsqrte (PSVECMag/PSVECNormalize) ends with fmaf: fused in hardware on arm64, emulated by the MSVC CRT */
#define fmaf(...) Mp5_fmaf(__VA_ARGS__)
#endif

#endif
