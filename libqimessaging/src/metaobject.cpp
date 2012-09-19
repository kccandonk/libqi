/*
**  Copyright (C) 2012 Aldebaran Robotics
**  See COPYING for the license
*/
#include <qimessaging/metaobject.hpp>
#include "metaobject_p.hpp"
#include <boost/algorithm/string/predicate.hpp>

namespace qi {

  qi::atomic<long> MetaObjectPrivate::uid = 1;

  MetaObjectPrivate::MetaObjectPrivate(const MetaObjectPrivate &rhs)
  {
    (*this) = rhs;
  }

  MetaObjectPrivate&  MetaObjectPrivate::operator=(const MetaObjectPrivate &rhs)
  {
    _methodsNameToIdx = rhs._methodsNameToIdx;
    _methods = rhs._methods;
    _eventsNameToIdx = rhs._eventsNameToIdx;
    _events = rhs._events;
    _nextNumber = rhs._nextNumber;
    return (*this);
  }

  std::vector<qi::MetaMethod> MetaObjectPrivate::findMethod(const std::string &name)
  {
    boost::recursive_mutex::scoped_lock sl(_mutexMethod);
    std::vector<qi::MetaMethod>           ret;
    MetaObject::MethodMap::iterator it;
    std::string cname(name);
    cname += "::";

    for (it = _methods.begin(); it != _methods.end(); ++it) {
      qi::MetaMethod &mm = it->second;
      if (boost::starts_with(mm.signature(), cname))
        ret.push_back(mm);
    }
    return ret;
  }

  std::vector<MetaSignal> MetaObjectPrivate::findSignal(const std::string &name)
  {
    boost::recursive_mutex::scoped_lock sl(_mutexEvent);
    std::vector<MetaSignal>           ret;
    MetaObject::SignalMap::iterator it;
    std::string cname(name);
    cname += "::";

    for (it = _events.begin(); it != _events.end(); ++it) {
      MetaSignal &mm = it->second;
      if (boost::starts_with(mm.signature(), cname))
        ret.push_back(mm);
    }
    return ret;
  }

  void MetaObjectPrivate::refreshCache()
  {
    {
      boost::recursive_mutex::scoped_lock sl(_mutexMethod);
      _methodsNameToIdx.clear();
      for (MetaObject::MethodMap::iterator i = _methods.begin();
        i != _methods.end(); ++i)
      _methodsNameToIdx[i->second.signature()] = i->second.uid();
    }
    {
      boost::recursive_mutex::scoped_lock sl(_mutexEvent);
      _eventsNameToIdx.clear();
      for (MetaObject::SignalMap::iterator i = _events.begin();
        i != _events.end(); ++i)
      _eventsNameToIdx[i->second.signature()] = i->second.uid();
    }
  }



  MetaObject::MetaObject()
  {
    _p = new MetaObjectPrivate();
  }

  MetaObject::MetaObject(const MetaObject &other)
  {
    _p = new MetaObjectPrivate();
    *_p = *(other._p);
  }

  MetaObject& MetaObject::operator=(const MetaObject &other)
  {
    *_p = *(other._p);
    return (*this);
  }

  MetaObject::~MetaObject()
  {
    delete _p;
  }

  MetaMethod *MetaObject::method(unsigned int id) {
    boost::recursive_mutex::scoped_lock sl(_p->_mutexMethod);
    MethodMap::iterator i = _p->_methods.find(id);
    if (i == _p->_methods.end())
      return 0;
    return &i->second;
  }

  const MetaMethod *MetaObject::method(unsigned int id) const {
    boost::recursive_mutex::scoped_lock sl(_p->_mutexMethod);
    MethodMap::const_iterator i = _p->_methods.find(id);
    if (i == _p->_methods.end())
      return 0;
    return &i->second;
  }

  MetaSignal *MetaObject::signal(unsigned int id) {
    boost::recursive_mutex::scoped_lock sl(_p->_mutexEvent);
    SignalMap::iterator i = _p->_events.find(id);
    if (i == _p->_events.end())
      return 0;
    return &i->second;
  }

  const MetaSignal *MetaObject::signal(unsigned int id) const {
    boost::recursive_mutex::scoped_lock sl(_p->_mutexEvent);
    SignalMap::const_iterator i = _p->_events.find(id);
    if (i == _p->_events.end())
      return 0;
    return &i->second;
  }

  int MetaObject::methodId(const std::string &name) const
  {
    return _p->methodId(name);
  }

  int MetaObject::signalId(const std::string &name) const
  {
    return _p->signalId(name);
  }

  MetaObject::MethodMap MetaObject::methodMap() const {
    return _p->_methods;
  }

  MetaObject::SignalMap MetaObject::signalMap() const {
    return _p->_events;
  }

  std::vector<qi::MetaMethod> MetaObject::findMethod(const std::string &name) const
  {
    return _p->findMethod(name);
  }

  std::vector<MetaSignal> MetaObject::findSignal(const std::string &name) const
  {
    return _p->findSignal(name);
  }



  qi::ODataStream &operator<<(qi::ODataStream &stream, const MetaObject &meta) {
    stream << meta._p->_methods;
    stream << meta._p->_events;
    stream << meta._p->_nextNumber;
    return stream;
  }

  qi::IDataStream &operator>>(qi::IDataStream &stream, MetaObject &meta) {
    stream >> meta._p->_methods;
    stream >> meta._p->_events;
    stream >> meta._p->_nextNumber;
    meta._p->refreshCache();
    return stream;
  }

  qi::SignatureStream &operator&(qi::SignatureStream &stream, const MetaObject &meta) {
    stream & meta._p->_methods;
    stream & meta._p->_events;
    stream & meta._p->_nextNumber;
    return stream;
  }


}
