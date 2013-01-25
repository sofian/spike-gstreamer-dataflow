/* Gear.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
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

#include "Gear.h"

Gear::Gear(std::string type, std::string name) :
_type(type),
_name(name),
_ready(false)
{
}

Gear::~Gear()
{    
  for (std::list<AbstractPlug*>::iterator it=_plugs.begin(); it != _plugs.end(); ++it)
    delete (*it);
}

void Gear::prePlay()
{
  internalPrePlay();
}

void Gear::postPlay()
{
  internalPostPlay();
}

void Gear::init()
{
  std::cout << "__________________________________________" << std::endl;
  std::cout << _type << std::endl;
  std::cout << "------------------------------------------" << std::endl;

  for (std::list<AbstractPlug*>::iterator it=_plugs.begin(); it != _plugs.end(); ++it)
    (*it)->init();

  //call the virtual method
  internalInit();
}


/**
 * Calls the destructor for the plug and remove it 
 * from the gear plugs
 * 
 * @param plug   the plug to delete
 */
void Gear::deletePlug(AbstractPlug *plug)
{
  delete plug;
  _plugs.remove(plug);
}


/**
 * Adds the specified AbstractPlug to the Gear. 
 * This method will fail if a plug with the same name already
 * exist for this gear.
 * 
 * @param plug   the plug to add
 * 
 * @return Success : pointer on the specified plug
 *         Error : null
 * @see AbstractPlug
 */
AbstractPlug* Gear::addPlug(AbstractPlug* plug)
{    
	if (!plug)
		return NULL;

//	if (!isPlugNameUnique(plug->name()))
//			return NULL;
/*  
  //if plug is input, add only plug for main type
  if (plug->inOut()==IN)
    _plugs.push_back(plug);  
  else
    //add main plug and plugs for its subType
    addPlugAndSubPlugs(plug,0);  
*/  

  _plugs.push_back(plug);  

  return plug;    
}

