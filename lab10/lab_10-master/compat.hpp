/**
 * \file: compat.hpp
 *
 * Néhány, C++11-től elavulttá (deprecated) minősített nyelvi elem
 * illetve működés kompatibilis megvalósítása
 *
 *
 */
#ifndef COMPAT_HPP
#define COMPAT_HPP

#if __cplusplus < 201103L
  // C++2003 vagy régebbi ///////////////////////////

  /// THROW_NOTHING makróval verziófüggetlenül lehet megadni, ha egy függvény nem dobhat kivételt
  #define THROW_NOTHING throw ()
  /// THROW_ANY makróval verziófüggetlenül lehet megadni, ha egy függvény dobhat kivételt
  #define THROW_SOMETHING(...) throw (__VA_ARGS__)
#else
  // C++11 vagy újabb ////////////////////////////////

  /// THROW_NOTHING makróval verziófüggetlenül lehet megadni, ha egy függvény nem dobhat kivételt
  #define THROW_NOTHING noexcept
  /// THROW_ANY makróval verziófüggetlenül lehet megadni, ha egy függvény dobhat kivételt
  #define THROW_SOMETHING(...) noexcept(false)


  namespace std {
    /// Ronda az std-be piszkolni, de a makró trükközés miatt kell.
    /// segédsablon a bind1st-höz.
    template <typename F, typename T>
    std::function<bool (T) > Bind1st(F f, T arg) {
      return std::bind(f, arg, std::placeholders::_1);
    }

    /// segédsablon a bind2nd-hoz
    template <typename F, typename T>
    std::function<bool (T) > Bind2nd(F f, T arg) {
      return std::bind(f, std::placeholders::_1, arg);
    }
  }

  /// bind1st, és bind2nd lecserélése a nagybetűs sablonra
  #define bind1st(a, b) Bind1st(a,b)
  #define bind2nd(a, b) Bind2nd(a,b)

#endif

#endif // COMPAT_HPP
