/* AbstractType.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __ABSTRACTTYPE_INCLUDED
#define __ABSTRACTTYPE_INCLUDED

#include <iostream>
#include <string>
#include <vector>

// TYPE_BASE_METHODS(ClassName [, RGB) implementation
//
// Usage:
// TYPE_BASE_METHODS(MyType, (red, green, blue))
// TYPE_BASE_METHODS(MyType) // --> no color() definition, will use that of parent

#define TYPE_BASE_METHODS(ClassName) \
    virtual std::string typeName() const { return #ClassName; } \
    virtual void copyFrom(const AbstractType& t) { (*this) = *(static_cast<const ClassName*>(&t)); } \
    virtual bool isSuperClassOf(const AbstractType& t) const { return (dynamic_cast<const ClassName*>(&t)) != NULL; } \
    virtual AbstractType* clone() const { return new ClassName(*this); }

class AbstractType
{
public:
  AbstractType(){}
  virtual ~AbstractType() {}
  
  TYPE_BASE_METHODS(AbstractType)

  virtual bool isSubClassOf(const AbstractType& t) const {
    return t.isSuperClassOf(*this);
  }

  bool isClassOf(AbstractType &other) const {
    return other.typeName() == typeName();
  }
};


#endif //__ABSTRACTTYPE_INCLUDED

