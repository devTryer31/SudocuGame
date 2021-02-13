#pragma once
#include <mutex>

template <class T>
class Synchronized final{
public:

	struct Synced_item final{
		T& value_;
		std::lock_guard<std::mutex> locker_guard_;
	};

	Synchronized(T&& value) :value_(value) {}

	Synchronized() = default;
	Synchronized(const Synchronized& source) = delete;
	void operator=(const Synchronized& source) = delete;

	Synced_item get_access() {
		return { value_, std::lock_guard(locker_) };
	}

private:
	T value_;
	std::mutex locker_;
};

