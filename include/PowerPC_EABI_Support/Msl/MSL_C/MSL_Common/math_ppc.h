/* Plain C in the original: the DOL defines sin, cos and atan2 unmangled and
 * every REL object references them that way. Without this guard a .cpp that
 * pulls in this header emits cos__Fd, which nothing defines, and the REL link
 * fails with "Failed to find symbol cos__Fd in any module". */
#ifdef __cplusplus
extern "C" {
#endif

double acos(double x);
double tan(double);
double sin(double);
double cos(double);
double atan2(double);

#ifdef __cplusplus
}
#endif