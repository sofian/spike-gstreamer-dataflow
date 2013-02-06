/* AbstractPlug.h
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

#ifndef __ABSTRACTPLUG_INCLUDED
#define __ABSTRACTPLUG_INCLUDED

#include <gst/gst.h>

#include "GstUtils.h"
#include "AbstractType.h"
#include "Gear.h"

#include <string>
#include <iostream> // XXX debug
#include <list>

class Gear;

enum eInOut
{
  IN, OUT
};

class AbstractPlug
{
public:
  AbstractPlug(GstPad* pad, Gear* parent, eInOut inOut, std::string name, AbstractType* type, bool mandatory);
  virtual ~AbstractPlug();

  virtual void init(){};
  bool canStartConnection();
  bool canConnect(AbstractPlug *plug, bool onlyTypeCheck=false);
  bool connect(AbstractPlug *plug);
  bool disconnect(AbstractPlug *plug);
  void disconnectAll();

  bool connected() const { return !_connectedPlugs.empty();};
  //  void sleeping(bool );
  //
  //  bool sleeping(){return _sleeping;}
  virtual bool ready() const =0;	//!behavior defined in input and output plug

  bool mandatory(){return _mandatory;}
  void mandatory(bool v){_mandatory=v;}  

  virtual void onConnection(AbstractPlug*){};//!overloader pour ajouter fonctionnalites APRES une bonne connection
  virtual void onDisconnection(AbstractPlug*){};//!overloader pour ajouter fonctionnalites AVANT deconnection

  AbstractType* abstractType() const { return _abstractType;}
  AbstractType* abstractDefaultType() const { return _abstractDefaultType;}
  AbstractType* abstractHintType() const { return _abstractDefaultType;}
  eInOut inOut() const {return _inOut;};

  int connectedPlugs(std::list<AbstractPlug*> &connectedplugs) const;
  AbstractPlug* firstConnectedPlug() const {return _connectedPlugs.front();}
  int nbConnectedPlugs() const {return _connectedPlugs.size();};
  Gear* parent() const {return _parent;};

  std::string fullName() const;
  std::string shortName(int nbChars) const;
  std::string name() const {return _name;};
  bool name(std::string newName);

  //  bool exposed() const {return _exposed;}
  //  void exposed(bool exp);

  void forwardPlug(AbstractPlug * forwardPlug) { _forwardPlug = forwardPlug; }
  AbstractPlug* forwardPlug(){ return _forwardPlug; }

  //  virtual AbstractPlug* clone(Gear* parent)=0;
protected:
  std::list<AbstractPlug*> _connectedPlugs;
  AbstractType *_abstractType;
  AbstractType *_abstractDefaultType;
  AbstractPlug* _forwardPlug;

  Gear *_parent;
  //! if true, this plug is not absolutly needed (connected,ready) for the parent gear to be ready.
  bool _mandatory;
  bool _sleeping;

private:
  eInOut _inOut;
  std::string _name;

  //  bool _exposed;//! the plug is exposed outside of a metagear

protected:
  GstPad* _pad; // the GStreamer pad that is represented by this plug
  gulong _blockingPadProbeId;
  GstElement* _tee;
  std::list<GstElement*> _queues;
};

#endif //__ABSTRACTPLUG_INCLUDED
