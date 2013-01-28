/*
 * GstUtils.h
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

#ifndef GSTUTILS_H_
#define GSTUTILS_H_

#include <gst/gst.h>
#include "error.h"

class GstUtils {
public:
  static void safeAddElement(GstPipeline* pipeline, GstElement* element);
  static void safeRemoveElement(GstPipeline* pipeline, GstElement* element);
  static void safeConnect(GstPad* src, GstPad* sink, gulong* blockingProbeId=0);

  // Puts a push blocking pad probe on #src# and returns its id.
  static void safeDisconnect(GstPad* src, GstPad* sink, gulong* blockingProbeId=0);

  static GstPadProbeReturn padProbeCallback (GstPad * src, GstPadProbeInfo * info, gpointer data);

};

#endif /* GSTUTILS_H_ */
