#include <gst/gst.h>
#include <unistd.h>
#include <signal.h>

#include "Gear.h"
#include "Engine.h"

bool stop = false;
void interrupt(int sig) {
  stop = true;
}

int main(int argc, char** argv) {
  signal(SIGINT, interrupt);
  signal(SIGTERM, interrupt);

  gst_init(&argc, &argv);

  Engine* engine = new Engine();

  GstElement* source    = gst_element_factory_make("audiotestsrc",  "source");
  GstElement* sink      = gst_element_factory_make("autoaudiosink", "sink");

  Gear* sourceGear = new Gear(source);
  Gear* sinkGear   = new Gear(sink);

  engine->addGear(sourceGear);
  engine->addGear(sinkGear);

  AbstractPlug* out = sourceGear->getOutput("src");
  AbstractPlug* in  = sinkGear->getInput("sink");
  out->connect(in);

//  gst_object_unref (GST_OBJECT (sourceOut));
//  gst_object_unref (GST_OBJECT (sinkIn));

  g_print ("Now playing\n");
  engine->play();

  while (!stop) {
    usleep(1);
  }

  engine->pause();

  return 0;
}
