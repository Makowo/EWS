#pragma once

#include <windows.h>

#include "ews_lua.hpp"
#include <HashlistLib.h>

class Vector3
{
public:
  Vector3() : x(0), y(0), z(0) {}
  Vector3(float x, float y) : x(x), y(y), z(0) {}
  Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
public:
  float x, y, z;
};

namespace dsl {
#ifndef DIESEL_PUSHLUA_CALLCONV

#if GAME_RAIDWW2
#define DIESEL_PUSHLUA_CALLCONV __fastcall
#else
#define DIESEL_PUSHLUA_CALLCONV __cdecl
#endif

#endif

  extern int(DIESEL_PUSHLUA_CALLCONV* push_lua_Vector3)(lua_State* L, const Vector3& value);

  void init_push_lua_functions(SignatureScannerFunction scanner);
}


class RAID_CXX14_string {
public:
  union {
    char* _Ptr;
    char _Buf[16];
  };
  size_t size;
  size_t reserved_size;

public:
  std::string get_string() const
  {
    if (size < 16) {
      return std::string(this->_Buf);
    }
    else {
      return std::string(this->_Ptr);
    }
  }
  const char* get_cstring() const
  {
    if (size < 16) {
      return this->_Buf;
    }
    else {
      return this->_Ptr;
    }
  }
};

#if GAME_RAIDWW2
typedef RAID_CXX14_string CPPSTD_string;
#endif

namespace dsl {
  class ThreadManager__Thread {
  public:
    char pad[8];
    CPPSTD_string name;
  };
  class DataStore {
  public:
    virtual ~DataStore();
  };
  template<typename T>
  class SP {
    T* _p;
    unsigned int _c;
  };
  class Archive {
  public:
    CPPSTD_string _name;

    __int64 _pos;
    //__int64 _start;
    __int64 _size;

    __int64 unk1;

    bool _sizable;
    bool _closed;

    __int64 unk2;
    _RTL_CRITICAL_SECTION _cs;

    dsl::SP<DataStore> _store;
  };
  class Transport {
  public:
    virtual ~Transport();
    virtual dsl::Archive* open(dsl::Archive* result, unsigned int db_key);
  };

  class Allocator {
  public:
    virtual ~Allocator();
    virtual void* allocate(size_t size);
    virtual void* allocate_aligned(size_t size, size_t align);
    virtual void deallocate(void* ptr);
  };

  template<typename TFirst, typename TSecond> class Pair {
  public:
    TFirst first;
    TSecond second;

  };
  template<typename T> class Vector {
  public:
    size_t _size;
    size_t _capacity;
    T* _data;
    Allocator* _allocator;

    const T* begin() const { return _data; }
    const T* end() const { return &_data[_size]; }
  };
  template<typename TKey, typename TValue> class SortMap {
  public:
    char _less[sizeof(ptrdiff_t)];
    Vector<Pair<TKey, TValue>> _data;
    bool _is_sorted;


    const Pair<TKey, TValue>* begin() const { return _data.begin(); }
    const Pair<TKey, TValue>* end() const { return _data.end(); }
  };

  class DB {
  public:
    struct DBExtKey {
      dsl::idstring _type;
      dsl::idstring _name;
      unsigned int _properties;
    };

    class Data {
    public:
      SortMap<dsl::idstring, unsigned int> _properties;
      SortMap<DBExtKey, unsigned int> _lookup;
      unsigned int _next_key;
    };

    virtual ~DB();
    void* unknown; // allocator virtual table
    const char* _name;
    Allocator* _allocator;

    CPPSTD_string _root;
    CPPSTD_string _name2;

    Data* _data;
  };
}
struct ResourceID {
  dsl::idstring type;
  dsl::idstring name;
};

