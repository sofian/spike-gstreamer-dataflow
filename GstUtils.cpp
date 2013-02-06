/*
 * GstUtils.cpp
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

#include "GstUtils.h"

void GstUtils::safeAddElement(GstPipeline* pipeline, GstElement* element)
{
}

void GstUtils::safeRemoveElement(GstPipeline* pipeline, GstElement* element)
{
}

void GstUtils::safeConnect(GstPad* src, GstPad* sink, gulong* blockingProbeId)
{
  gst_pad_link(src, sink);

  // Remove blocking pad if there is one.
  // XXX: Y a peut-etre possibilite d'une confusion ici si le AbstractPlug est un PlugIn (ie. sink)
  if (blockingProbeId && *blockingProbeId != 0 && gst_pad_is_blocking(src))
  {
    // XXX: rien ne garantiti que le blocking probe id ne soit pas justement zero
    // XXX: Y a peut-etre possibilite d'une confusion ici si le AbstractPlug est un PlugIn (ie. sink)
    ASSERT_WARNING(gst_pad_is_blocked(src));
    gst_pad_remove_probe(src, *blockingProbeId);
  }
}
#include <unistd.h>

void GstUtils::safeDisconnect(GstPad* src, GstPad* sink, gulong* blockingProbeId)
{
  if (gst_pad_get_peer(src) != sink)
    return;

  std::cout << "Wait" << std::endl;
  GstPipeline* pipeline = Engine::instance().pipeline();

  //ASSERT_ERROR( gst_pad_get_peer(src) == sink );
  GstState state;
  ASSERT_ERROR( gst_element_get_state(GST_ELEMENT(pipeline), &state, NULL, GST_CLOCK_TIME_NONE) != GST_STATE_CHANGE_FAILURE);

  if (state == GST_STATE_PLAYING) {
    if (!gst_pad_is_blocking(src)) {
      std::cout << "Block probe" << std::endl;
      gulong id = gst_pad_add_probe (src, (GstPadProbeType)GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM,
                                     GstUtils::padProbeCallback, (gpointer) sink, NULL);
      if (blockingProbeId)
        *blockingProbeId = id;
    }
  }
  else
  {
    std::cout << "Not playing!!" << std::endl;
    gst_pad_unlink(src, sink);
    blockingProbeId = 0;
  }
  //while (gst_pad_get_peer(src) == sink) usleep(1000);
  std::cout << "OK" << std::endl;
}

GstPadProbeReturn GstUtils::padProbeCallback (GstPad * src, GstPadProbeInfo * info, gpointer data)
{
//  GST_DEBUG_OBJECT (src, "pad is blocked now");

//  while (true) usleep(1000);
  NOTICE("FUIIIIIKK");
  gst_pad_unlink(src, (GstPad*)data);

  return GST_PAD_PROBE_OK;
}
