#pragma once

#include <ppl.h>

class critical_section_lock{
public:
	critical_section_lock(Concurrency::critical_section &cs)
		: cs(&cs)
	{
		if (this->cs){
			this->cs->lock();
		}
	}

	critical_section_lock(Concurrency::critical_section *cs)
		: cs(cs)
	{
		if (this->cs){
			this->cs->lock();
		}
	}

	critical_section_lock(const critical_section_lock &other) = delete;

	critical_section_lock(critical_section_lock &&other)
		: cs(other.cs){
		other.cs = nullptr;
	}

	~critical_section_lock(){
		if (this->cs){
			this->cs->unlock();
		}
	}

	critical_section_lock &operator=(const critical_section_lock &other) = delete;

	critical_section_lock &operator=(critical_section_lock &&other){
		if (this != &other){
			this->cs = other.cs;
			other.cs = nullptr;
		}

		return *this;
	}

private:
	Concurrency::critical_section *cs;
};