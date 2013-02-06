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
#include "Plug.h"
#include <algorithm>


Gear::Gear(GstElement* element) :
_ready(false),
_element(element)
{
  ASSERT_ERROR( element );
  GstIterator* it;
  GstIteratorResult result = GST_ITERATOR_OK;
  GValue value = G_VALUE_INIT;

  it = gst_element_iterate_sink_pads(_element);

  while ( (result = gst_iterator_next(it, &value)) == GST_ITERATOR_OK)
  {
    GstPad* pad = GST_PAD_CAST(g_value_peek_pointer(&value));
    std::cout << gst_pad_get_name(pad) << std::endl;
    addPlug(new PlugIn<AbstractType>(pad, this, gst_pad_get_name(pad), true, new AbstractType()));
    gst_object_unref(pad);
    g_value_unset(&value);
  }
  gst_iterator_free(it);

  it = gst_element_iterate_src_pads(_element);
  while ( (result = gst_iterator_next(it, &value)) == GST_ITERATOR_OK)
  {
    GstPad* pad = GST_PAD_CAST(g_value_peek_pointer(&value));
    std::cout << gst_pad_get_name(pad) << std::endl;
    addPlug(new PlugOut<AbstractType>(pad, this, gst_pad_get_name(pad), true, new AbstractType()));
    gst_object_unref(pad);
    g_value_unset(&value);
  }
  gst_iterator_free(it);

  gst_object_ref(GST_OBJECT(_element));
}

Gear::~Gear()
{    
  for (std::list<AbstractPlug*>::iterator it=_plugs.begin(); it != _plugs.end(); ++it)
    delete (*it);
  gst_object_unref(GST_OBJECT(_element));
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
//  std::cout << "__________________________________________" << std::endl;
//  std::cout << _type << std::endl;
//  std::cout << "------------------------------------------" << std::endl;

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


void Gear::getInputs(std::list<AbstractPlug*> &inputs) const
{
  inputs.clear();
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
  {
    if ((*it)->inOut() == IN )
      inputs.push_back(*it);
  }
}

void Gear::getOutputs(std::list<AbstractPlug*> &outputs) const
{
  outputs.clear();
  for (std::list<AbstractPlug*>::const_iterator it=_plugs.begin(); it != _plugs.end(); ++it)
  {
    if ((*it)->inOut() == OUT)
      outputs.push_back(*it);
  }
}

AbstractPlug* Gear::getInput(std::string name) const
{
  std::list<AbstractPlug*> inputs;
  getInputs(inputs);

  int (*pf)(int)=tolower;
  std::string nameAlower=name;
  transform(nameAlower.begin(), nameAlower.end(), nameAlower.begin(), pf);

  for (std::list<AbstractPlug*>::const_iterator it = inputs.begin();it!=inputs.end();++it)
  {
    std::string nameBlower=(*it)->name();
    transform(nameBlower.begin(), nameBlower.end(), nameBlower.begin(), pf);

    if (nameAlower == nameBlower)
      return(*it);
  }

  return NULL;
}

AbstractPlug* Gear::getOutput(std::string name) const
{
  std::list<AbstractPlug*> outputs;
  getOutputs(outputs);

  int (*pf)(int)=tolower;
  std::string nameAlower=name;
  transform(nameAlower.begin(), nameAlower.end(), nameAlower.begin(), pf);

  for (std::list<AbstractPlug*>::const_iterator it = outputs.begin();it!=outputs.end();++it)
  {
    std::string nameBlower=(*it)->name();
    transform(nameBlower.begin(), nameBlower.end(), nameBlower.begin(), pf);

    if (nameAlower == nameBlower)
      return(*it);
  }

  return NULL;
}
