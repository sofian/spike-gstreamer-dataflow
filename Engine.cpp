/*
 * Engine.cpp
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

#include "Engine.h"

Engine::Engine() : _pipeline(0) {
  _pipeline = (GstPipeline*)gst_pipeline_new("drone");
}

Engine::~Engine() {
  gst_object_unref(_pipeline);
}

void Engine::play() {
  gst_element_set_state (GST_ELEMENT(_pipeline), GST_STATE_PLAYING);

}

void Engine::pause() {
  gst_element_set_state (GST_ELEMENT(_pipeline), GST_STATE_PAUSED);
}

void Engine::addGear(Gear* gear) {
  _gears.push_back(gear);
  gst_bin_add(GST_BIN(_pipeline), gear->_element);
}
