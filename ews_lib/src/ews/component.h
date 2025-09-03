#pragma once

#include <wx/wx.h>
#include "ews_utils.h"

#include "EventHandler.h"

#define OBJECT_CONVERSION_TYPE_VERIFICATION_NONE 0
#define OBJECT_CONVERSION_TYPE_VERIFICATION_ON_GET (1 << 0)
#define OBJECT_CONVERSION_TYPE_VERIFICATION_ON_SET (1 << 1)
#define OBJECT_CONVERSION_TYPE_VERIFICATION_ALWAYS (OBJECT_CONVERSION_TYPE_VERIFICATION_ON_GET | OBJECT_CONVERSION_TYPE_VERIFICATION_ON_SET)

#define OBJECT_CONVERSION_TYPE_VERIFICATION OBJECT_CONVERSION_TYPE_VERIFICATION_ON_SET

int EWS_Component_GarbageCollection(lua_State* L);

class Component {
  public:
    virtual ~Component();

public:
    template<typename T>
    T* get_internal_object_type() {
      if (!this)
        return nullptr; // check if this is being ran on a null object. If it is, it is likely the result of a parent parameter being passed as nil from lua. in this case, continue to run without crashing, and return a nullptr instead.
#if OBJECT_CONVERSION_TYPE_VERIFICATION & OBJECT_CONVERSION_TYPE_VERIFICATION_ON_GET
      T* obj = dynamic_cast<T*>(this->internal_component);
      if (obj == nullptr)
        __debugbreak(); // get_internal_object_type has been called for a type that does not inherit from the needed object.
      return obj;
#else
      return (T*)this->internal_component;
#endif
    }

public:
    template<typename T>
    void set_internal_object_type(T* new_obj) {
#if OBJECT_CONVERSION_TYPE_VERIFICATION & OBJECT_CONVERSION_TYPE_VERIFICATION_ON_SET
      wxObject* obj = dynamic_cast<wxObject*>(new_obj);
      if (obj == nullptr)
        __debugbreak(); // set_internal_object_type has been called for a non-wxWidgets object
      this->internal_component = obj;
#else
      this->internal_component = (wxObject*)new_obj;
#endif
    }

private:
    wxObject* internal_component;
};
