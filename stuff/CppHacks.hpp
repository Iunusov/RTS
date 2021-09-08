#pragma once

#define CNOF const noexcept override final
#define NCNOF noexcept override final

#ifdef _MSC_VER
#define INLINE inline __forceinline
#else
#define INLINE inline __attribute__((always_inline))
#endif

//#define LIKELY [[likely]]
//#define UNLIKELY [[unlikely]]

#define LIKELY
#define UNLIKELY
