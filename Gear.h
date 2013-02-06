/* Gear.h
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

#ifndef GEAR_INCLUDED
#define GEAR_INCLUDED

#include <gst/gst.h>

#include <string>
#include <list>
#include <vector>
#include <map>

#include "AbstractPlug.h"

class AbstractPlug;

/**
 * Gear is the atomic processing unit of the dataflow and the base class for all gears.
 * 
 * 
 */
class Gear
{
public:
  Gear(GstElement* element);
  virtual ~Gear();

  void init();
  void prePlay();    
  void postPlay();

  virtual void run() {}

  void getInputs(std::list<AbstractPlug*> & inputs) const;
  void getOutputs(std::list<AbstractPlug*> &outputs) const;

  AbstractPlug* getInput(std::string name) const;
  AbstractPlug* getOutput(std::string name) const;

  //  void name(std::string vname){_name=vname;}

//  const std::string& type() const {return _type;};
  const std::string name() const { return std::string(GST_ELEMENT_NAME(_element)); }

  bool ready(){return _ready;}

protected:

  virtual void internalInit(){}
  
  virtual void internalPrePlay(){}  
  virtual void internalPostPlay(){}
  
  //friend bool AbstractPlug::connect(AbstractPlug *plug);
  //friend bool AbstractPlug::disconnect(AbstractPlug *plug);

  AbstractPlug* addPlug(AbstractPlug* plug);
  void deletePlug(AbstractPlug *plug);

  std::list<AbstractPlug*> _plugs;    

protected:

//  std::string _type;
//  std::string _name;//! unique name of this gear in a schema

  bool _ready;

  // In the end it won't necessarily be one gear = one element but this is what we do for now
  GstElement* _element;

  friend class Engine;
};

#endif
