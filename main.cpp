#include <gst/gst.h>
#include <unistd.h>
#include <signal.h>

bool stop = false;
void interrupt(int sig) {
  stop = true;
}

int main(int argc, char** argv) {
  signal(SIGINT, interrupt);
  signal(SIGTERM, interrupt);

  gst_init(&argc, &argv);

  GstPipeline* pipeline = (GstPipeline*)gst_pipeline_new("drone");
  GstElement* source    = gst_element_factory_make("audiotestsrc",  "source");
  GstElement* sink      = gst_element_factory_make("autoaudiosink", "sink");

  if (!pipeline || !source || !sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    return -1;
  }

  gst_bin_add(GST_BIN(pipeline), source);
  gst_bin_add(GST_BIN(pipeline), sink);

  GstPad* sourceOut = gst_element_get_static_pad(source, "src");
  GstPad* sinkIn    = gst_element_get_static_pad(sink, "sink");

  gst_pad_link(sourceOut, sinkIn);

//  gst_object_unref (GST_OBJECT (sourceOut));
//  gst_object_unref (GST_OBJECT (sinkIn));

  g_print ("Now playing\n");
  gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_PLAYING);

  while (!stop) {
    usleep(1);
  }

  gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_PAUSED);

  gst_object_unref (GST_OBJECT (pipeline));

  return 0;
}
