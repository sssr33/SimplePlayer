#include "HTime.h"

#include <exception>

namespace H {
	void InitializeSYSTEMTIME(SYSTEMTIME& obj) {
		obj.wYear = 0;
		obj.wSecond = 0;
		obj.wMonth = 0;
		obj.wMinute = 0;
		obj.wMilliseconds = 0;
		obj.wHour = 0;
		obj.wDayOfWeek = 0;
		obj.wDay = 0;
	}

	uint64_t Time::GetTimeMsRelative() {
#if defined(HAVE_VISUAL_STUDIO)
		return GetTickCount64();
#else
#error Add impl
#endif
	}

	uint64_t Time::UnixTimeToLib(uint64_t unixTime) {
		uint64_t libTime = Time::Y1970Ticks + (unixTime * Time::HNSResolution);
		return libTime;
	}

	uint64_t Time::LibTimeToUnix(uint64_t libTime) {
		uint64_t unixTime = (libTime - Time::Y1970Ticks) / Time::HNSResolution;
		return unixTime;
	}

	uint64_t Time::GetCurrentLibTime() {
		typedef std::ratio<1, static_cast<int64_t>(Time::HNSResolution)> hns;
		typedef std::chrono::duration<int64_t, hns> hundred_nanoseconds;
		uint64_t libTime = 0;
		auto nowTp = std::chrono::system_clock::now();
		uint64_t nowSysHns = static_cast<uint64_t>(std::chrono::duration_cast<hundred_nanoseconds>(nowTp.time_since_epoch()).count());

		libTime = Time::GetSysClockEpochTicks() + nowSysHns;

		return libTime;
	}

	uint64_t Time::CeilLibTimeToSeconds(uint64_t libTime) {
		uint64_t millis = libTime % Time::HNSResolution;

		libTime -= millis;

		if (millis != 0) {
			libTime += Time::HNSResolution;
		}

		return libTime;
	}

	uint64_t Time::FileTimeToLib(FILETIME fileTime) {
		uint64_t tmpFileTime = static_cast<uint64_t>(fileTime.dwLowDateTime) | (static_cast<uint64_t>(fileTime.dwHighDateTime) << 32);
		uint64_t libTime = Time::Y1601Ticks + tmpFileTime;
		return libTime;
	}

	FILETIME Time::LibTimeToFile(uint64_t libTime) {
		uint64_t tmpFileTime = libTime - Time::Y1601Ticks;
		FILETIME fileTime;

		fileTime.dwLowDateTime = static_cast<DWORD>(tmpFileTime & 0x00000000FFFFFFFF);
		fileTime.dwHighDateTime = static_cast<DWORD>((tmpFileTime >> 32) & 0x00000000FFFFFFFF);

		return fileTime;
	}

	uint64_t Time::UniversalTimeToLib(int64_t universalTime) {
		uint64_t libTime = static_cast<uint64_t>(static_cast<int64_t>(Time::Y1601Ticks) + universalTime);
		return libTime;
	}

	int64_t Time::LibTimeToUniversal(uint64_t libTime) {
		int64_t universalTime = static_cast<int64_t>(libTime - Time::Y1601Ticks);
		return universalTime;
	}

	uint64_t Time::LibTimeToLocal(uint64_t libTime) {
		uint64_t localLibTime = 0;
		SYSTEMTIME st = { 0, 0, 0, 0, 0, 0, 0, 0 },
			st_utc = { 0, 0, 0, 0, 0, 0, 0, 0 };
		auto ft = Time::LibTimeToFile(libTime);

		auto res = FileTimeToSystemTime(&ft, &st_utc);
		if (res) {
			res = SystemTimeToTzSpecificLocalTime(nullptr, &st_utc, &st);

			if (res) {
				res = SystemTimeToFileTime(&st, &ft);
			}
			else {
				std::exception("exception in LibTimeToLocal");
			}
		}
		else {
			std::exception("exception in LibTimeToLocal");
		}

		if (res) {
			localLibTime = Time::FileTimeToLib(ft);
		}
		else {
			std::exception("exception in LibTimeToLocal");
		}

		return localLibTime;
	}

	uint64_t Time::LibTimeToGlobal(uint64_t localLibTime) {
		uint64_t libTime = 0;
		SYSTEMTIME st = { 0, 0, 0, 0, 0, 0, 0, 0 },
			st_utc = { 0, 0, 0, 0, 0, 0, 0, 0 };
		auto ft = Time::LibTimeToFile(localLibTime);

		auto res = FileTimeToSystemTime(&ft, &st);
		if (res) {
			res = TzSpecificLocalTimeToSystemTime(nullptr, &st, &st_utc);

			if (res) {
				res = SystemTimeToFileTime(&st_utc, &ft);
			}
			else {
				std::exception("exception in LibTimeToLocal");
			}
		}
		else {
			std::exception("exception in LibTimeToLocal");
		}

		if (res) {
			libTime = Time::FileTimeToLib(ft);
		}
		else {
			std::exception("exception in LibTimeToLocal");
		}

		return libTime;
	}

	uint64_t Time::GetSysClockEpochTicks() {
		static uint64_t SysClockEpochTicks = 0;

		if (SysClockEpochTicks == 0) {
			std::chrono::system_clock::time_point epochTp;
			tm epochTm;
			auto epochTimeT = std::chrono::system_clock::to_time_t(epochTp);
			gmtime_s(&epochTm, &epochTimeT);
			uint64_t epochFullTicks = Time::YearTicks * (epochTm.tm_year + 1900);
			uint64_t epochLeapTicks = epochFullTicks % Time::DayTicks;
			epochFullTicks -= epochLeapTicks;

			epochFullTicks += Time::DayTicks * epochTm.tm_yday;
			epochFullTicks += Time::HourTicks * epochTm.tm_hour;
			epochFullTicks += Time::MinuteTicks * epochTm.tm_min;
			epochFullTicks += Time::HNSResolution * epochTm.tm_sec;

			SysClockEpochTicks = epochFullTicks;
		}

		return SysClockEpochTicks;
	}
}