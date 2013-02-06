/*
 * Engine.h
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <gst/gst.h>

#include "error.h"
#include "Gear.h"

class Gear;

class Engine {
private:
  Engine();
  Engine(Engine const&);
  void operator=(Engine const&);
  ~Engine();

public:
  void play();
  void pause();
  void addGear(Gear* gear);
  void removeGear(Gear* gear);

  static Engine& instance();

  GstPipeline* pipeline() const { return _pipeline; }

  // Creates an element and adds it to the pipeline.
  GstElement* createElement(const char* factoryName);
  //  void destroyElement(GstElement* element);

  void _addElement(GstElement* element);
  void _removeElement(GstElement* element);
protected:
  GstPipeline* _pipeline;
  //  GstBus*      _bus;
  //  std::list<Gear*> _gears;
};

#endif /* ENGINE_H_ */
