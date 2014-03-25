/* Automatically generated from Squeak on (28 April 2009 2:40:06 pm) */

#if defined(WIN32) || defined(_WIN32) || defined(Win32)
 #ifdef __cplusplus
  #define DLLEXPORT extern "C" __declspec(dllexport)
 #else
  #define DLLEXPORT __declspec(dllexport)
 #endif /* C++ */
#else
 #define DLLEXPORT
#endif /* WIN32 */

#include "sqVirtualMachine.h"

/* memory access macros */
#define byteAt(i) (*((unsigned char *) (i)))
#define byteAtput(i, val) (*((unsigned char *) (i)) = val)
#define longAt(i) (*((int *) (i)))
#define longAtput(i, val) (*((int *) (i)) = val)

#include "sqHardware.h"
#include <string.h>


/*** Variables ***/
struct VirtualMachine* interpreterProxy;
const char *moduleName = "HardwarePlugin 16 June 2013 (e)";

/*** Functions ***/
DLLEXPORT void primPinMode(void);
DLLEXPORT void primDigitalWrite(void);
DLLEXPORT int primDigitalRead(void);
DLLEXPORT int primAnalogReadVoltage(void);
DLLEXPORT void primAnalogWrite(void);
DLLEXPORT void primAnalogSetFreq(void);
DLLEXPORT void primAnalogSet(void);
DLLEXPORT int primAnalogGetMaxLevel(void);
DLLEXPORT int setInterpreter(struct VirtualMachine* anInterpreter);

DLLEXPORT void primPinMode(void) {
	int pin = interpreterProxy->stackIntegerValue(1);
	int mode = interpreterProxy->stackIntegerValue(0);
	pinMode(pin, mode);
}

DLLEXPORT void primDigitalWrite(void) {
	int pin = interpreterProxy->stackIntegerValue(1);
	int level = interpreterProxy->stackIntegerValue(0);
	digitalWrite(pin, level);
}

DLLEXPORT int primDigitalRead(void) {
	int pin = interpreterProxy->stackIntegerValue(0);
	int level = digitalRead(pin);
	interpreterProxy->pop(1);
	interpreterProxy->pushBool(level != 0);
	return 0;
}

DLLEXPORT int primAnalogReadVoltage(void) {
	int pin = interpreterProxy->stackIntegerValue(0);
	int level = analogReadVoltage(pin);
	interpreterProxy->pop(1);
	interpreterProxy->pushInteger(level);
	return 0;
}

DLLEXPORT void primAnalogWrite(void) {
	int pin = interpreterProxy->stackIntegerValue(1);
	int level = interpreterProxy->stackIntegerValue(0);
	analogWrite(pin, level);
}

DLLEXPORT void primAnalogSetFreq(void) {
	int pin = interpreterProxy->stackIntegerValue(1);
	int freq = interpreterProxy->stackIntegerValue(0);
	analogSetFreq(pin, freq);
}

DLLEXPORT void primAnalogSet(void) {
	int pin = interpreterProxy->stackIntegerValue(2);
	int freq = interpreterProxy->stackIntegerValue(1);
	int level = interpreterProxy->stackIntegerValue(0);
	analogSetFreq(pin, freq);
	analogWrite(pin, level);
}

DLLEXPORT int primAnalogGetMaxLevel(void) {
	int pin = interpreterProxy->stackIntegerValue(0);
	int level = analogGetMaxLevel(pin);
	interpreterProxy->pop(1);
	interpreterProxy->pushInteger(level);
	return 0;
}

DLLEXPORT int setInterpreter(struct VirtualMachine* anInterpreter) {
        int ok;

        interpreterProxy = anInterpreter;
        ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
        if (ok == 0) {
                return 0;
        }
        ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
        return ok;
}
