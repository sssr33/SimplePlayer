#include "DebugScopedWatch.h"

#include "libhelpers\HSystem.h"

DebugScopedWatch::DebugScopedWatch(double maxSeconds, const std::string &message) 
	: maxSeconds(maxSeconds), message(message)
{
	this->watch.Start();
}

DebugScopedWatch::~DebugScopedWatch() {
	double elapsed = this->watch.Elapsed<double>();
	if (elapsed > this->maxSeconds) {
		H::System::DebugOutput("(time " + std::to_string(elapsed) + "s)" + this->message);
	}
}