/* AbstractPlug.cpp
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

#include "error.h"
#include "AbstractPlug.h"

#include "Gear.h"

AbstractPlug::AbstractPlug(GstPad* pad, Gear* parent, eInOut inOut, std::string name, AbstractType* type, bool mandatory) :
  _abstractType(type),
  _abstractDefaultType(type),
  _parent(parent),
  _mandatory(mandatory),
  _sleeping(false),
  _inOut(inOut),
  _name(name),
//  _exposed(false),
  _pad(pad)
{
  //une plug a besoin d'un parent
  ASSERT_ERROR(parent!=NULL);

  forwardPlug(0);

  gst_object_ref(GST_OBJECT(_pad));
}

AbstractPlug::~AbstractPlug()
{
  disconnectAll();
  gst_object_unref(GST_OBJECT(_pad));
}

bool AbstractPlug::canConnect(AbstractPlug *plug, bool onlyTypeCheck)
{
  if (plug==NULL)
    return false;

  //s'assurer que plug n'est pas deja connecte a nous
//  if (std::find(_connectedPlugs.begin(), _connectedPlugs.end(), plug) != _connectedPlugs.end())
//    return false;

  //avons-nous bien une connection d'un in dans un out ou vice-versa
  if (_inOut == plug->inOut())
    return false;


  // Prevent downcasting.
  if (_inOut == IN)
  {
    if (! plug->abstractType()->isSubClassOf(*_abstractType))
      return false;
  }
  else
  {
    if (! plug->abstractType()->isSuperClassOf(*_abstractType))
      return false;
  }

    //assurer seulement une connection par input
  if (!onlyTypeCheck)
  {
    if (_inOut == IN)
    {
      if (!_connectedPlugs.empty())
        return false;
    } else
    {
      if (!plug->_connectedPlugs.empty())
      {
        return false;
      }
    }
  }
  return true;
}

bool AbstractPlug::connect(AbstractPlug *plug)
//! logique de connection de base
{

  if (!canConnect(plug))
    return false;

  if (this->inOut() == IN)
    gst_pad_link(plug->_pad, _pad);
  else
    gst_pad_link(_pad, plug->_pad);

  //remove exposition
//  if(this->inOut() == IN)
//    exposed(false);

//  if(plug->inOut() == IN)
//    plug->exposed(false);

//  AbstractPlug * deepestPlug = 0;
//  for(deepestPlug = this; deepestPlug->_forwardPlug != 0; deepestPlug = deepestPlug->_forwardPlug) ;
//  if(deepestPlug != this)
//    deepestPlug->_connectedPlugs.push_back(plug);
//
//  AbstractPlug * deepestOtherPlug = 0;
//  for(deepestOtherPlug = plug; deepestOtherPlug->_forwardPlug != 0; deepestOtherPlug = deepestOtherPlug->_forwardPlug) ;
//  if(deepestOtherPlug != plug)
//    deepestOtherPlug->_connectedPlugs.push_back(this);

  //ajouter la nouvelle plug a nos connections
  _connectedPlugs.push_back(plug);
  plug->_connectedPlugs.push_back(this);

//  _parent->onPlugConnected(this, plug);
//  plug->_parent->onPlugConnected(plug, this);

  //laisser la chance au class derive d'executer leur logique supplementaire
  onConnection(plug);
  plug->onConnection(this);

  //tell the gear that a new connection have been created and that sync is needed
//  _parent->unSynch();

//   if(_forwardPlug)
//     _forwardPlug->_parent->unSynch();

  return true;
}

bool AbstractPlug::disconnect(AbstractPlug *plug)
//! logique de deconnection de base
{
  if (!plug)
    return false;

  //on ne peut pas deconnecter une plug qui n'est pas connecte a nous
//  std::list<AbstractPlug*>::iterator it = find(_connectedPlugs.begin(), _connectedPlugs.end(), plug);
//  if (it == _connectedPlugs.end())
//    return false;

//  _parent->onPlugDisconnected(this, plug);
//  plug->_parent->onPlugDisconnected(plug, this);

  //laisser la chance au class derive d'executer leur logique supplementaire
  onDisconnection(plug);
  plug->onDisconnection(this);

  //remove this plug from our connections
  _connectedPlugs.remove(plug);
  AbstractPlug * deepestPlug = 0;
  for(deepestPlug = this; deepestPlug->_forwardPlug != 0; deepestPlug = deepestPlug->_forwardPlug) ;
  if(deepestPlug != this)
    deepestPlug->_connectedPlugs.remove(plug);

  //remove ourself from the other plug connections
  plug->_connectedPlugs.remove(this);
  AbstractPlug * deepestOtherPlug = 0;
  for(deepestOtherPlug = plug; deepestOtherPlug->_forwardPlug != 0; deepestOtherPlug = deepestOtherPlug->_forwardPlug) ;
  if(deepestOtherPlug != plug)
    deepestOtherPlug->_connectedPlugs.remove(this);

  return true;
}

void AbstractPlug::disconnectAll()
{
  while (!_connectedPlugs.empty())
  {
    disconnect(_connectedPlugs.back());
  }
}

int AbstractPlug::connectedPlugs(std::list<AbstractPlug*> &connectedplugs) const
{
  connectedplugs.clear();
  connectedplugs.assign(_connectedPlugs.begin(), _connectedPlugs.end());

  return connectedplugs.size();
}

std::string AbstractPlug::fullName() const
{
  return _parent->name()+"/"+_name;
}

std::string AbstractPlug::shortName(int nbChars) const
{
  std::string abbrev;

  int c=0;
  for (std::string::const_iterator it=_name.begin();it != _name.end() && c < nbChars; ++it, ++c)
  {
    abbrev+=*it;
  }

  return abbrev;
}

/**
* Change the metagear exposition status of the plug.
*
* @param exp The new metagear expose for the plug. The status will remain unchange
*            if the plug is plug <code>IN</code> type and actually connected.
*/
//void AbstractPlug::exposed(bool exp)
//{
//  if((this->inOut() == IN) && (this->connected()))
//  {
//    return;
//  } else
//  {
//    this->_exposed = exp;
//  }
//}

bool AbstractPlug::name(std::string newName)
{
//  if (!_parent->isPlugNameUnique(newName))
//    return false;

  _name = newName;
  return true;
}

//void AbstractPlug::sleeping(bool s)
//{
//  if (s!=_sleeping)
//     _parent->unSynch();
//
//   _sleeping=s;
//}
