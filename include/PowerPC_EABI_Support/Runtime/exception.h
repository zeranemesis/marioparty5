#ifndef _EXCEPTION
#define _EXCEPTION

namespace std {
class exception {
public:
	exception() { }
	// Both virtuals are declared but defined in the runtime library.  MWCC
	// otherwise emits its own out-of-line copies here, and that is what put
	// two functions and 0x54 extra bytes in Gecko_ExceptionPPC.o: the target
	// references __dt__Q23std9exceptionFv, what__Q23std9exceptionCFv and
	// __vt__Q23std9exception as undefined, so none of the three belong here.
	virtual ~exception();
	virtual const char* what() const;
};

class bad_exception : public exception {
public:
	bad_exception() { }
	virtual ~bad_exception() { }
	virtual const char* what() const { return "bad_exception"; }
};

typedef void (*unexpected_handler)();
unexpected_handler set_unexpected(unexpected_handler handler);
void unexpected();

typedef void (*terminate_handler)();
terminate_handler set_terminate(terminate_handler handler);
void terminate();

} // namespace std

using std::bad_exception;
using std::exception;
using std::set_terminate;
using std::set_unexpected;
using std::terminate;
using std::terminate_handler;
using std::unexpected;
using std::unexpected_handler;

#endif
