#include <gst/gst.h>
#include <unistd.h>
#include <signal.h>

#include "Gear.h"
#include "Engine.h"

bool stop = false;
void interrupt(int sig) {
  stop = true;
}

void delayMillis(unsigned long millis) {
  while (millis--)
    usleep(1000);
}

int main(int argc, char** argv) {
  signal(SIGINT, interrupt);
  signal(SIGTERM, interrupt);

  gst_init(&argc, &argv);

  Engine& engine = Engine::instance();

  GstElement* source    = gst_element_factory_make("audiotestsrc",  "source");
  GstElement* sink      = gst_element_factory_make("autoaudiosink", "sink");

  Gear* sourceGear = new Gear(source);
  Gear* sinkGear   = new Gear(sink);

  engine.addGear(sourceGear);
  engine.addGear(sinkGear);

  AbstractPlug* out = sourceGear->getOutput("src");
  AbstractPlug* in  = sinkGear->getInput("sink");
  out->connect(in);

//  gst_object_unref (GST_OBJECT (sourceOut));
//  gst_object_unref (GST_OBJECT (sinkIn));

  g_print ("Now playing\n");
  engine.play();

  for (int i=0; i<100; i++)
  {
    out->disconnect(in);
    out->connect(in);
  }

  // Marchera pas. Pour arranger il faut checker ceci:
  // http://gstreamer.freedesktop.org/data/doc/gstreamer/head/manual/html/section-dynamic-pipelines.html
  while (!stop) {
    delayMillis(1000);
    out->disconnect(in);
    delayMillis(1000);
    out->connect(in);
  }

  engine.pause();

  return 0;
}
