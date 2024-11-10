#pragma once

#include <cstdlib>
#include <new>

/// @brief cache aligned memory allocator
/// @tparam T type for allocator
template <typename T>
struct AlignedAllocator {
	using value_type = T;

	AlignedAllocator() = default;

	template <typename U>
	constexpr AlignedAllocator(const AlignedAllocator<U>&) noexcept { }

	T* allocate(std::size_t n) {
		void* ptr = std::aligned_alloc(64, n * sizeof(T));
		if(!ptr) {
			throw std::bad_alloc();
		}
		return static_cast<T*>(ptr);
	}

	void deallocate(T* p, std::size_t) noexcept {
		std::free(p);
	}

	bool operator==(const AlignedAllocator&) const noexcept {
		return true; // All instances of this allocator type are considered equal
	}

	bool operator!=(const AlignedAllocator&) const noexcept {
		return false; // If `==` returns true, `!=` should return false
	}
};