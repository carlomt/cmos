// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CMOSDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Frame(void *p = 0);
   static void *newArray_Frame(Long_t size, void *p);
   static void delete_Frame(void *p);
   static void deleteArray_Frame(void *p);
   static void destruct_Frame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Frame*)
   {
      ::Frame *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Frame >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Frame", ::Frame::Class_Version(), "Frame.h", 17,
                  typeid(::Frame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Frame::Dictionary, isa_proxy, 4,
                  sizeof(::Frame) );
      instance.SetNew(&new_Frame);
      instance.SetNewArray(&newArray_Frame);
      instance.SetDelete(&delete_Frame);
      instance.SetDeleteArray(&deleteArray_Frame);
      instance.SetDestructor(&destruct_Frame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Frame*)
   {
      return GenerateInitInstanceLocal((::Frame*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Frame*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Seed(void *p = 0);
   static void *newArray_Seed(Long_t size, void *p);
   static void delete_Seed(void *p);
   static void deleteArray_Seed(void *p);
   static void destruct_Seed(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Seed*)
   {
      ::Seed *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Seed >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Seed", ::Seed::Class_Version(), "Seed.h", 10,
                  typeid(::Seed), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Seed::Dictionary, isa_proxy, 4,
                  sizeof(::Seed) );
      instance.SetNew(&new_Seed);
      instance.SetNewArray(&newArray_Seed);
      instance.SetDelete(&delete_Seed);
      instance.SetDeleteArray(&deleteArray_Seed);
      instance.SetDestructor(&destruct_Seed);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Seed*)
   {
      return GenerateInitInstanceLocal((::Seed*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Seed*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_SeedList(void *p = 0);
   static void *newArray_SeedList(Long_t size, void *p);
   static void delete_SeedList(void *p);
   static void deleteArray_SeedList(void *p);
   static void destruct_SeedList(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SeedList*)
   {
      ::SeedList *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SeedList >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SeedList", ::SeedList::Class_Version(), "SeedList.h", 12,
                  typeid(::SeedList), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SeedList::Dictionary, isa_proxy, 4,
                  sizeof(::SeedList) );
      instance.SetNew(&new_SeedList);
      instance.SetNewArray(&newArray_SeedList);
      instance.SetDelete(&delete_SeedList);
      instance.SetDeleteArray(&deleteArray_SeedList);
      instance.SetDestructor(&destruct_SeedList);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SeedList*)
   {
      return GenerateInitInstanceLocal((::SeedList*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SeedList*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Frame::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Frame::Class_Name()
{
   return "Frame";
}

//______________________________________________________________________________
const char *Frame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Frame*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Frame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Frame*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Frame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Frame*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Frame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Frame*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Seed::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Seed::Class_Name()
{
   return "Seed";
}

//______________________________________________________________________________
const char *Seed::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Seed*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Seed::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Seed*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Seed::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Seed*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Seed::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Seed*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SeedList::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SeedList::Class_Name()
{
   return "SeedList";
}

//______________________________________________________________________________
const char *SeedList::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SeedList*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SeedList::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SeedList*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SeedList::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SeedList*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SeedList::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SeedList*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Frame::Streamer(TBuffer &R__b)
{
   // Stream an object of class Frame.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Frame::Class(),this);
   } else {
      R__b.WriteClassBuffer(Frame::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Frame(void *p) {
      return  p ? new(p) ::Frame : new ::Frame;
   }
   static void *newArray_Frame(Long_t nElements, void *p) {
      return p ? new(p) ::Frame[nElements] : new ::Frame[nElements];
   }
   // Wrapper around operator delete
   static void delete_Frame(void *p) {
      delete ((::Frame*)p);
   }
   static void deleteArray_Frame(void *p) {
      delete [] ((::Frame*)p);
   }
   static void destruct_Frame(void *p) {
      typedef ::Frame current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Frame

//______________________________________________________________________________
void Seed::Streamer(TBuffer &R__b)
{
   // Stream an object of class Seed.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Seed::Class(),this);
   } else {
      R__b.WriteClassBuffer(Seed::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Seed(void *p) {
      return  p ? new(p) ::Seed : new ::Seed;
   }
   static void *newArray_Seed(Long_t nElements, void *p) {
      return p ? new(p) ::Seed[nElements] : new ::Seed[nElements];
   }
   // Wrapper around operator delete
   static void delete_Seed(void *p) {
      delete ((::Seed*)p);
   }
   static void deleteArray_Seed(void *p) {
      delete [] ((::Seed*)p);
   }
   static void destruct_Seed(void *p) {
      typedef ::Seed current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Seed

//______________________________________________________________________________
void SeedList::Streamer(TBuffer &R__b)
{
   // Stream an object of class SeedList.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(SeedList::Class(),this);
   } else {
      R__b.WriteClassBuffer(SeedList::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SeedList(void *p) {
      return  p ? new(p) ::SeedList : new ::SeedList;
   }
   static void *newArray_SeedList(Long_t nElements, void *p) {
      return p ? new(p) ::SeedList[nElements] : new ::SeedList[nElements];
   }
   // Wrapper around operator delete
   static void delete_SeedList(void *p) {
      delete ((::SeedList*)p);
   }
   static void deleteArray_SeedList(void *p) {
      delete [] ((::SeedList*)p);
   }
   static void destruct_SeedList(void *p) {
      typedef ::SeedList current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SeedList

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 457,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlESeedgR_Dictionary();
   static void vectorlESeedgR_TClassManip(TClass*);
   static void *new_vectorlESeedgR(void *p = 0);
   static void *newArray_vectorlESeedgR(Long_t size, void *p);
   static void delete_vectorlESeedgR(void *p);
   static void deleteArray_vectorlESeedgR(void *p);
   static void destruct_vectorlESeedgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Seed>*)
   {
      vector<Seed> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Seed>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Seed>", -2, "vector", 457,
                  typeid(vector<Seed>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlESeedgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Seed>) );
      instance.SetNew(&new_vectorlESeedgR);
      instance.SetNewArray(&newArray_vectorlESeedgR);
      instance.SetDelete(&delete_vectorlESeedgR);
      instance.SetDeleteArray(&deleteArray_vectorlESeedgR);
      instance.SetDestructor(&destruct_vectorlESeedgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Seed> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<Seed>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlESeedgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Seed>*)0x0)->GetClass();
      vectorlESeedgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlESeedgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlESeedgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Seed> : new vector<Seed>;
   }
   static void *newArray_vectorlESeedgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Seed>[nElements] : new vector<Seed>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlESeedgR(void *p) {
      delete ((vector<Seed>*)p);
   }
   static void deleteArray_vectorlESeedgR(void *p) {
      delete [] ((vector<Seed>*)p);
   }
   static void destruct_vectorlESeedgR(void *p) {
      typedef vector<Seed> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Seed>

namespace {
  void TriggerDictionaryInitialization_CMOSDict_Impl() {
    static const char* headers[] = {
"Frame.h",
"Seed.h",
"SeedList.h",
0
    };
    static const char* includePaths[] = {
"/Users/Amedeo/root_folder/root-6.06.02/include",
"/Users/Amedeo/root_folder/root-6.06.02/include",
"/Users/Amedeo/repos/cmos/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "CMOSDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Frame.h")))  Frame;
class __attribute__((annotate("$clingAutoload$Frame.h")))  Seed;
class __attribute__((annotate("$clingAutoload$Frame.h")))  SeedList;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "CMOSDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef ROOT_INTERFACE
  #define ROOT_INTERFACE 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Frame", payloadCode, "@",
"Seed", payloadCode, "@",
"SeedList", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CMOSDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CMOSDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CMOSDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CMOSDict() {
  TriggerDictionaryInitialization_CMOSDict_Impl();
}
