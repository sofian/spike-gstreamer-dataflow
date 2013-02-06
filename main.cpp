#define DEBUG_LEVEL 3

#include <gst/gst.h>
#include <unistd.h>
#include <signal.h>

#include "Gear.h"
#include "Engine.h"

#include <stdlib.h>

bool stop = false;
void interrupt(int sig) {
  stop = true;
  exit(0);
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

  NOTICE("== Creating one source and one sink");
  GstElement* source    = gst_element_factory_make("audiotestsrc", NULL);
  GstElement* sink1      = gst_element_factory_make("autoaudiosink", NULL);

  Gear* sourceGear = new Gear(source);
  Gear* sinkGear1  = new Gear(sink1);

  engine.addGear(sourceGear);
  engine.addGear(sinkGear1);

  AbstractPlug* out = sourceGear->getOutput("src");
  AbstractPlug* in1 = sinkGear1->getInput("sink");
  out->connect(in1);

//  gst_object_unref (GST_OBJECT (sourceOut));
//  gst_object_unref (GST_OBJECT (sinkIn));

//  NOTICE("-- Quick connect/disconnect (no playback)");
//  for (int i=0; i<100; i++)
//  {
//    out->disconnect(in1);
//    out->connect(in1);
//  }
//
//  NOTICE("-- Starting playback");
//  engine.play();
//
//  NOTICE("-- Quick connect/disconnect");
//  for (int i=0; i<100; i++)
//  {
//    out->disconnect(in1);
//    out->connect(in1);
//  }
//
//  NOTICE("-- Audible connect/disconnect");
//
//  // Marchera pas. Pour arranger il faut checker ceci:
//  // http://gstreamer.freedesktop.org/data/doc/gstreamer/head/manual/html/section-dynamic-pipelines.html
//  for (int i=0; i<10; i++)
//  {
//    delayMillis(500);
//    out->disconnect(in1);
//    delayMillis(500);
//    out->connect(in1);
//  }
//
//  engine.pause();

  NOTICE("== Creating one extra sink (testing split)");
//  engine.pause();
  delayMillis(1000);

//  Gear* aconvert   = new Gear( gst_element_factory_make("audioconvert", NULL) );
//  engine.addGear(aconvert);

  Gear* monoscope   = new Gear( gst_element_factory_make("monoscope", NULL) );
  engine.addGear(monoscope);

//  Gear* vconvert   = new Gear( gst_element_factory_make("videoconvert", NULL) );
//  engine.addGear(vconvert);

  Gear* sinkGear2   = new Gear( gst_element_factory_make("ximagesink", NULL) );
  engine.addGear(sinkGear2);

//  out->connect(aconvert->getInput("sink"));
//  aconvert->getOutput("src")->connect(monoscope->getInput("sink"));
//  monoscope->getOutput("src")->connect(vconvert->getInput("sink"));
//  vconvert->getOutput("src")->connect(sinkGear2->getInput("sink"));
  //  vconvert->getOutput("src")->connect(sinkGear2->getInput("sink"));
//  out->disconnect(in1);
  out->connect(monoscope->getInput("sink"));
  monoscope->getOutput("src")->connect(sinkGear2->getInput("sink"));

  NOTICE("-- Starting playback");
  GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(engine.pipeline()), GST_DEBUG_GRAPH_SHOW_ALL, "after_all_copied");
  engine.play();

  while (!stop) { delayMillis(1000); }

  engine.pause();

  return 0;
}
