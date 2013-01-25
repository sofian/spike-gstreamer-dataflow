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
  //_gears.push_back(gear);

  // State of added element must be the same as current state of pipeline.
  GstState state;
  ASSERT_ERROR( gst_element_get_state(GST_ELEMENT(_pipeline), &state, NULL, GST_CLOCK_TIME_NONE) != GST_STATE_CHANGE_FAILURE);
  gst_element_set_state(gear->_element, state);

  // Add to pipeline.
  gst_bin_add(GST_BIN(_pipeline), gear->_element);
}

void Engine::removeGear(Gear* gear) {
  // _gear.remove(gear);

  // Remove from pipeline.
  // Attention: "Unparenting the element means that the element will be dereferenced,
  // so if the bin holds the only reference to the element, the element will be freed
  // in the process of removing it from the bin. If you want the element to still exist
  // after removing, you need to call gst_object_ref() before removing it from the bin. "
  // Source: http://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/GstBin.html#gst-bin-remove
  gst_bin_remove(GST_BIN(_pipeline), gear->_element);
}
